#pragma once

#include "fmrt_event.hpp"
#include "fmrt_envelope.hpp"
#include "fmrt_errors.hpp"
#include "fmrt_types.hpp"
#include "fmrt_config.hpp"

namespace fmrt
{
    class EventHandler
    {
    public:

        // Stage 1 of FMRT pipeline — pure validation & canonicalization
        bool validate(const StructEvent& E, StateEnvelope& out_env) const noexcept;

        // Stage 2 — normalize event (stimulus, dt, etc.)
        void canonicalize(StructEvent& E) const noexcept;
    };

} // namespace fmrt
