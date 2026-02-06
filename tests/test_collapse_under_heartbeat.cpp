#include <iostream>
#include "fmrt_api.hpp"

using namespace fmrt;

// --------------------------------------------------------------
// Test 7: collapse_under_heartbeat
//
// HEARTBEAT также должен корректно доводить κ до нуля,
// если организм уже находится в предколлапсном состоянии.
//
// Логика:
//   - κ = 1e-12 (живая, но почти мёртвая структура)
//   - Δ огромная → R большое
//   - Φ высокая
//   - event = HEARTBEAT (dt > 0)
//   - Пока κ > 0 → collapse недопустим
//   - При κ == 0 → collapse обязателен и геометрия корректная
// --------------------------------------------------------------

int test_collapse_under_heartbeat()
{
    StructuralState X{};
    X.reset();

    X.Kappa = 1e-12;   // предколлапс
    X.Phi   = 50.0;    // усиление R
    X.M     = 100.0;
    X.Delta = {1000.0, -800.0, 900.0, -750.0}; // экстремальный стресс

    StructEvent e{};
    e.type = EventType::Heartbeat;
    e.dt   = 1.0;

    bool seen_collapse = false;

    for (int i = 0; i < 50; ++i)
    {
        StateEnvelope out = FMRT_Step(X, e);

        if (out.state.Kappa > 0.0)
        {
            // Пока κ > 0 collapse НЕ должен происходить
            if (out.metrics.is_collapse)
            {
                std::cerr << "collapse_under_heartbeat: collapse too early (κ>0)\n";
                return 1;
            }
        }
        else
        {
            // κ == 0 → должен быть collapse
            if (!out.metrics.is_collapse)
            {
                std::cerr << "collapse_under_heartbeat: collapse flag missing at κ==0\n";
                return 1;
            }

            // строгая геометрия collapse
            if (out.metrics.det_g != 0.0)
            {
                std::cerr << "collapse_under_heartbeat: det_g must be 0\n";
                return 1;
            }

            if (out.metrics.tau != 0.0)
            {
                std::cerr << "collapse_under_heartbeat: tau must be 0\n";
                return 1;
            }

            if (out.metrics.mu != 1.0)
            {
                std::cerr << "collapse_under_heartbeat: mu must be 1\n";
                return 1;
            }

            if (out.metrics.regime != Regime::COL)
            {
                std::cerr << "collapse_under_heartbeat: regime must be COL\n";
                return 1;
            }

            seen_collapse = true;
            break;
        }

        X = out.state;
    }

    if (!seen_collapse)
    {
        std::cerr << "collapse_under_heartbeat: collapse did not occur\n";
        return 1;
    }

    return 0; // OK
}
