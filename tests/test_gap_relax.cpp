#include <cassert>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_gap_relax()
{
    StructuralState X;
    X.Delta = {0.1, -0.05, 0.02, 0.0};
    X.Phi   = 0.2;
    X.M     = 0.1;
    X.Kappa = 1.0;
    X.RegimePrev = Regime::ACC;

    StructEvent E;
    E.type = EventType::Gap;
    E.dt   = 1.0;

    StateEnvelope out = FMRT_Step(X, E);

    if (out.status != StepStatus::OK) return 1;
    if (out.event_type != EventType::Gap) return 1;

    bool any_changed = false;
    for (std::size_t i = 0; i < DELTA_DIM; ++i)
        if (out.state.Delta[i] != X.Delta[i])
            any_changed = true;

    if (!any_changed) return 1;

    if (!(out.state.Phi < X.Phi)) return 1;
    if (out.state.Phi < 0.0) return 1;

    if (out.state.M <= X.M) return 1;

    if (out.state.Kappa >= X.Kappa) return 1;
    if (out.state.Kappa <= 0.0) return 1;

    if (out.metrics.det_g <= 0.0) return 1;
    if (out.metrics.tau <= 0.0) return 1;
    if (out.metrics.mu < 0.0 || out.metrics.mu > 1.0) return 1;

    if (out.metrics.regime != Regime::ACC) return 1;

    return 0;
}
