#pragma once
//
// FMRT Core V2.2
// fmrt_api.hpp
//
// Public API for FMRT Core.
// This is the ONLY interface exposed to external systems (SDK, agents).
// FMRT_Step is pure, deterministic, stateless, and side-effect-free.
//
// Transformation:
//     (X(t), E(t)) -> StateEnvelope(t+1)
//

#include "fmrt_state.hpp"
#include "fmrt_event.hpp"
#include "fmrt_envelope.hpp"

namespace fmrt
{
    // -------------------------------------------------------------------------
    // FMRT_Step:
    //   - deterministic
    //   - no hidden state
    //   - no allocations
    //   - no IO, no randomness
    //   - identical outputs across all platforms for identical inputs
    //
    // NEXT:
    //   Implementation is in src/fmrt_api.cpp
    // -------------------------------------------------------------------------
    StateEnvelope FMRT_Step(
        const StructuralState& X,
        const StructEvent& E
    );

} // namespace fmrt
