#include <cassert>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_heartbeat()
{
    // Initial state
    StructuralState X;
    X.Delta = {0.05, -0.01, 0.02, 0.0};
    X.Phi   = 0.3;
    X.M     = 0.2;
    X.Kappa = 1.0;
    X.RegimePrev = Regime::ACC;

    // HEARTBEAT event
    StructEvent E;
    E.type = EventType::Heartbeat;
    E.dt   = 1.0;

    StateEnvelope out = FMRT_Step(X, E);

    // Basic correctness
    if (out.status != StepStatus::OK) return 1;
    if (out.event_type != EventType::Heartbeat) return 1;

    // Δ must relax slightly (not equal to previous)
    bool any_changed = false;
    for (std::size_t i = 0; i < DELTA_DIM; ++i)
        if (out.state.Delta[i] != X.Delta[i])
            any_changed = true;

    if (!any_changed) return 1;

    // Φ must decrease (no deformation)
    if (!(out.state.Phi < X.Phi)) return 1;
    if (out.state.Phi < 0.0) return 1;

    // M must increase
    if (out.state.M <= X.M) return 1;

    // κ must decay minimally
    if (!(out.state.Kappa < X.Kappa)) return 1;
    if (out.state.Kappa <= 0.0) return 1;

    // metrics must be finite and valid
    if (out.metrics.det_g <= 0.0) return 1;
    if (out.metrics.tau <= 0.0) return 1;
    if (out.metrics.mu < 0.0 || out.metrics.mu > 1.0) return 1;

    // regime remains ACC
    if (out.metrics.regime != Regime::ACC) return 1;

    return 0;
}
