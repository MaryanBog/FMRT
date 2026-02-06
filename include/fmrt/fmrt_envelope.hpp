#pragma once
//
// FMRT Core V2.2
// fmrt_envelope.hpp
//
// Output envelope returned by FMRT.step().
// Contains:
//   - next structural state X(t+1)
//   - derived metrics
//   - invariant status flags
//   - diagnostics metadata (error codes, status)
//   - event type
//

#include "fmrt_state.hpp"
#include "fmrt_metrics.hpp"
#include "fmrt_invariants.hpp"
#include "fmrt_errors.hpp"
#include "fmrt_types.hpp"

namespace fmrt
{
    struct StateEnvelope
    {
        // ---------------------------------------------------------------------
        // Structural state (next or unchanged if rejected)
        // ---------------------------------------------------------------------
        StructuralState state;

        // ---------------------------------------------------------------------
        // Derived metrics computed by evolution engine
        // ---------------------------------------------------------------------
        DerivedMetrics metrics;

        // ---------------------------------------------------------------------
        // Invariant validation result
        // ---------------------------------------------------------------------
        InvariantStatus invariants;

        // ---------------------------------------------------------------------
        // Diagnostics (step status + error category/reason)
        // ---------------------------------------------------------------------
        StepStatus status = StepStatus::OK;
        ErrorCategory error_category = ErrorCategory::None;

        // Optional human-readable reason (static string)
        const char* error_reason = nullptr;

        // ---------------------------------------------------------------------
        // Event type that produced this output
        // ---------------------------------------------------------------------
        EventType event_type = EventType::Heartbeat;

        // ---------------------------------------------------------------------
        // Validity check: ensures envelope contains no NaN/Inf
        // ---------------------------------------------------------------------
        bool isFinite() const noexcept
        {
            if (!state.isFinite()) return false;
            if (!metrics.isFinite()) return false;
            return true;
        }
    };

} // namespace fmrt
