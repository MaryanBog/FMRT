// fmrt_bridge.cpp
#include "fmrt_bridge.h"
#include "fmrt_api.hpp"

#include <cmath>      // std::isfinite
#include <cstdint>

using namespace fmrt;

// persistent FMRT state inside the DLL
static StructuralState g_state;

static inline bool is_finite_double(double x) noexcept
{
    return std::isfinite(x) != 0;
}

static inline bool validate_event(const fmrt_bridge_event* in) noexcept
{
    if (!in) return false;

    // dt must be strictly positive and finite
    if (!is_finite_double(in->dt) || !(in->dt > 0.0))
        return false;

    // stimulus must be finite
    for (int i = 0; i < DELTA_DIM; ++i)
        if (!is_finite_double(in->stimulus[i]))
            return false;

    // type must be within known enum range (defensive)
    // Adjust these bounds if your EventType enum differs.
    // Expected: {UPDATE=0/1, HEARTBEAT=..., GAP=...}
    const std::uint8_t t = in->type;
    if (t > static_cast<std::uint8_t>(EventType::GAP))
        return false;

    // Optional strictness (TIT-like): HEARTBEAT and GAP must have zero stimulus.
    // Uncomment if FMRT expects zero-vector for these.
    /*
    if (t == static_cast<std::uint8_t>(EventType::HEARTBEAT) ||
        t == static_cast<std::uint8_t>(EventType::GAP))
    {
        for (int i = 0; i < DELTA_DIM; ++i)
            if (in->stimulus[i] != 0.0)
                return false;
    }
    */

    return true;
}

FMRT_API void FMRT_CALL FMRT_Reset()
{
    g_state.reset();
}

FMRT_API int FMRT_CALL FMRT_Step(const fmrt_bridge_event* in,
                                 fmrt_bridge_envelope*    out)
{
    if (!in || !out)
        return FMRT_BRIDGE_E_NULLPTR;

    if (!validate_event(in))
        return FMRT_BRIDGE_E_BAD_INPUT;

    // Convert bridge → FMRT event
    StructEvent E{};
    E.type = static_cast<EventType>(in->type);
    E.dt   = in->dt;

    for (int i = 0; i < DELTA_DIM; ++i)
        E.stimulus[i] = in->stimulus[i];

    // Real FMRT step
    StateEnvelope env = fmrt::FMRT_Step(g_state, E);

    // TIT-style: only OK/ERROR/COLLAPSE are allowed to escape as normal.
    // Any other status (e.g., REJECT) is a fatal integration bug.
    if (env.status != StepStatus::OK &&
        env.status != StepStatus::ERROR &&
        env.status != StepStatus::COLLAPSE)
    {
        return FMRT_BRIDGE_E_FATAL_STATUS;
    }

    // update persistent state only on valid steps
    g_state = env.state;

    // Convert FMRT → bridge
    out->status        = static_cast<std::uint8_t>(env.status);
    out->invariants_ok = env.invariants.all_ok ? 1u : 0u;

    // derived metrics
    out->derived[0] = env.metrics.curvature_R;
    out->derived[1] = env.metrics.det_g;
    out->derived[2] = env.metrics.tau;
    out->derived[3] = 0.0; // reserved / ABI-stable slot

    return FMRT_BRIDGE_OK;
}
