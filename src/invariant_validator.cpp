#include "internal/invariant_validator.hpp"
#include "fmrt_constants.hpp"
#include "fmrt_config.hpp"

namespace fmrt
{

bool InvariantValidator::validate(
    const StructuralState& X_current,
    const StructuralState& X_next,
    const DerivedMetrics& metrics,
    StateEnvelope& out_env
) const noexcept
{
    InvariantStatus st{};
    st.clear();

    bool ok = true;

    ok &= checkMemory(X_current, X_next, st);
    ok &= checkKappa(X_next, st);
    ok &= checkMetric(X_next, metrics, st);
    ok &= checkTau(X_next, metrics, st);
    ok &= checkMorphology(metrics, st);
    ok &= checkRegime(X_current.RegimePrev, metrics.regime, st);
    ok &= checkCollapse(X_next, metrics, st);
    ok &= checkForbidden(X_next, metrics, st);

    st.all_ok = ok;
    out_env.invariants = st;

    return ok;
}

bool InvariantValidator::checkMemory(
    const StructuralState& X_cur,
    const StructuralState& X_next,
    InvariantStatus& st
) const noexcept
{
    const bool valid =
        is_finite(X_next.M) &&
        X_next.M >= X_cur.M;

    if (valid) st.set(INV_MEMORY);
    return valid;
}

bool InvariantValidator::checkKappa(
    const StructuralState& X_next,
    InvariantStatus& st
) const noexcept
{
    const bool valid =
        is_finite(X_next.Kappa) &&
        X_next.Kappa >= 0.0;

    if (valid) st.set(INV_KAPPA);
    return valid;
}

bool InvariantValidator::checkMetric(
    const StructuralState& X_next,
    const DerivedMetrics& metrics,
    InvariantStatus& st
) const noexcept
{
    bool valid = true;

    if (X_next.Kappa > EPS_KAPPA)
    {
        valid = is_finite(metrics.det_g) &&
                metrics.det_g > 0.0;
    }
    else
    {
        // collapse: det_g must be 0
        valid = metrics.det_g == 0.0;
    }

    if (valid) st.set(INV_METRIC);
    return valid;
}

bool InvariantValidator::checkTau(
    const StructuralState& X_next,
    const DerivedMetrics& metrics,
    InvariantStatus& st
) const noexcept
{
    if (!is_finite(metrics.tau))
        return false;

    bool ok;

    if (X_next.Kappa > EPS_KAPPA)
        ok = (metrics.tau > 0.0);     // живой: τ > 0
    else
        ok = (metrics.tau == 0.0);    // коллапс: τ = 0

    if (ok)
        st.set(INV_TAU);

    return ok;
}

bool InvariantValidator::checkMorphology(
    const DerivedMetrics& metrics,
    InvariantStatus& st
) const noexcept
{
    const bool valid =
        is_finite(metrics.mu) &&
        metrics.mu >= 0.0 &&
        metrics.mu <= 1.0;

    if (valid) st.set(INV_MORPHOLOGY);
    return valid;
}

bool InvariantValidator::checkRegime(
    Regime prev,
    Regime next,
    InvariantStatus& st
) const noexcept
{
    const int p = static_cast<int>(prev);
    const int n = static_cast<int>(next);

    const bool valid = (n >= p);

    if (valid) st.set(INV_REGIME);
    return valid;
}

bool InvariantValidator::checkCollapse(
    const StructuralState& X_next,
    const DerivedMetrics& metrics,
    InvariantStatus& st
) const noexcept
{
    bool valid = true;

    if (X_next.Kappa <= EPS_KAPPA)
    {
        valid =
            metrics.det_g == 0.0 &&
            metrics.tau   == 0.0 &&
            metrics.mu    == 1.0 &&
            metrics.regime == Regime::COL;
    }

    if (valid) st.set(INV_COLLAPSE);
    return valid;
}

bool InvariantValidator::checkForbidden(
    const StructuralState& X_next,
    const DerivedMetrics& metrics,
    InvariantStatus& st
) const noexcept
{
    bool valid = true;

    // state must be finite
    valid &= X_next.isFinite();
    // metrics must be finite
    valid &= metrics.isFinite();

    // kappa cannot be negative
    valid &= (X_next.Kappa >= 0.0);

    // active organism: strict positivity of metric/tau
    if (X_next.Kappa > EPS_KAPPA)
    {
        valid &= (metrics.det_g > 0.0);
        valid &= (metrics.tau > 0.0);
    }

    if (valid) st.set(INV_FORBIDDEN);
    return valid;
}

} // namespace fmrt
