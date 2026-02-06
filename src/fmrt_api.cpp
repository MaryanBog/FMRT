//
// FMRT Core V2.2
// fmrt_api.cpp
//

#include "fmrt_api.hpp"

#include "internal/event_handler.hpp"
#include "internal/evolution_engine.hpp"
#include "internal/invariant_validator.hpp"
#include "internal/diagnostics.hpp"
#include "internal/fp_guard.hpp"

#include <cmath>        // fpclassify, FP_SUBNORMAL

namespace fmrt
{
    // Статические, stateless модули
    static const EventHandler       g_event_handler{};
    static const EvolutionEngine    g_evolution{};
    static const InvariantValidator g_validator{};
    static const DiagnosticsLayer   g_diag{};
    static const FpGuard            g_fp{};

    // ------------------ DENORMAL CHECK HELPERS ------------------
    inline bool is_denormal(double x)
    {
        return x != 0.0 && std::fpclassify(x) == FP_SUBNORMAL;
    }

    inline bool has_denormal(const StructuralState& X)
    {
        for (double v : X.Delta)
            if (is_denormal(v)) return true;

        return
            is_denormal(X.Phi)   ||
            is_denormal(X.M)     ||
            is_denormal(X.Kappa);
    }

    inline bool has_denormal(const StructEvent& E)
    {
        if (is_denormal(E.dt)) return true;

        for (double v : E.stimulus)
            if (is_denormal(v)) return true;

        return false;
    }
    // ------------------------------------------------------------

    StateEnvelope FMRT_Step(
        const StructuralState& X,
        const StructEvent&     E_in
    )
    {
        StateEnvelope env{};

        // ---------------------------------------------------------------------
        // 0) FP-окружение (строгий IEEE-754)
        // ---------------------------------------------------------------------
        if constexpr (ENABLE_FP_GUARDS)
        {
            if (!g_fp.verifyEnvironment())
            {
                env.state          = X;
                env.status         = StepStatus::ERROR;
                env.error_category = ErrorCategory::NumericError;
                env.error_reason   = ERR_NUMERIC_ERROR;
                env.event_type     = E_in.type;
                return env;
            }
        }

        // ---------------------------------------------------------------------
        // 1) GLOBAL NUMERIC REJECT (NaN / INF / DENORMALS)
        // ---------------------------------------------------------------------
        if (!X.isFinite() || !E_in.isFinite() ||
            has_denormal(X) || has_denormal(E_in))
        {
            env.status         = StepStatus::ERROR;
            env.error_category = ErrorCategory::NumericError;
            env.error_reason   = ERR_NUMERIC_ERROR;

            // Envelope MUST be finite
            env.state.reset();
            env.metrics = {};
            env.invariants.clear();
            env.invariants.all_ok = false;

            env.event_type = E_in.type;
            return env;
        }

        // ---------------------------------------------------------------------
        // 2) Локальная копия события
        // ---------------------------------------------------------------------
        StructEvent E = E_in;

        // ---------------------------------------------------------------------
        // 3) Валидация события
        // ---------------------------------------------------------------------
        if (!g_event_handler.validate(E, env))
        {
            env.state      = X;
            env.metrics    = DerivedMetrics{};
            env.event_type = E.type;
            return env;
        }

        // ---------------------------------------------------------------------
        // 4) Каноникализация события
        // ---------------------------------------------------------------------
        g_event_handler.canonicalize(E);

        // ---------------------------------------------------------------------
        // 5) Эволюция
        // ---------------------------------------------------------------------
        StructuralState X_next{};
        DerivedMetrics  metrics{};

        g_evolution.evolve(X, E, X_next, metrics);

        // ---------------------------------------------------------------------
        // 5a) RESET — инварианты не проверяются
        // ---------------------------------------------------------------------
        if (E.type == EventType::Reset)
        {
            X_next.RegimePrev = metrics.regime;

            g_diag.buildOkEnvelope(
                X_next,
                metrics,
                E.type,
                env
            );

            InvariantStatus st{};
            st.clear();
            st.all_ok = true;

            env.invariants = st;
            return env;
        }

        // ---------------------------------------------------------------------
        // 6) Инварианты
        // ---------------------------------------------------------------------
        StateEnvelope inv_env{};
        inv_env.event_type = E.type;

        const bool ok = g_validator.validate(
            X,
            X_next,
            metrics,
            inv_env
        );

        if (!ok)
        {
            g_diag.buildErrorEnvelope(
                X,
                DerivedMetrics{},
                E.type,
                ErrorCategory::InvariantViolation,
                ERR_INVARIANT_VIOLATION,
                env
            );

            env.invariants = inv_env.invariants;
            return env;
        }

        // ---------------------------------------------------------------------
        // 7) Принятие шага
        // ---------------------------------------------------------------------
        X_next.RegimePrev = metrics.regime;

        g_diag.buildOkEnvelope(
            X_next,
            metrics,
            E.type,
            env
        );

        env.invariants = inv_env.invariants;

        return env;
    }

} // namespace fmrt
