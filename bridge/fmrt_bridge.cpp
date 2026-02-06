#include "fmrt_bridge.h"
#include "fmrt_api.hpp"

using namespace fmrt;

// persistent FMRT state
static StructuralState g_state;

extern "C" __declspec(dllexport)
void __stdcall FMRT_Reset()
{
    g_state.reset();
}

extern "C" __declspec(dllexport)
int __stdcall FMRT_Step(const fmrt_bridge_event* in,
                        fmrt_bridge_envelope*    out)
{
    if (!in || !out)
        return -1;

    // ----------------------------
    // Convert bridge → FMRT event
    // ----------------------------
    StructEvent E{};
    E.type = static_cast<EventType>(in->type);
    E.dt   = in->dt;

    for (int i = 0; i < DELTA_DIM; ++i)
        E.stimulus[i] = in->stimulus[i];

    // ----------------------------
    // Real FMRT step
    // ----------------------------
    StateEnvelope env = fmrt::FMRT_Step(g_state, E);

    // update persistent state
    g_state = env.state;

    // ----------------------------
    // Convert FMRT → bridge
    // ----------------------------
    out->status        = (uint8_t)env.status;
    out->invariants_ok = env.invariants.all_ok;

    // derived: FMRT V2.2 has only 3 metrics
    out->derived[0] = env.metrics.curvature_R;
    out->derived[1] = env.metrics.det_g;
    out->derived[2] = env.metrics.tau;
    out->derived[3] = 0.0;    // ABI padding for STIE

    return 0;
}
