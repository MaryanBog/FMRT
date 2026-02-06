#pragma once
//
// FMRT Core V2.2
// diagnostics.hpp
//
// Diagnostics Layer — final stage of FMRT pipeline.
// Responsibilities:
//   - assemble StateEnvelope
//   - set step_status, error_category, error_reason
//   - attach event_type
//   - NEVER modify structural fields in a non-deterministic way
//
// No allocations, no IO, no time, no randomness.
//

#include "fmrt_envelope.hpp"
#include "fmrt_state.hpp"
#include "fmrt_metrics.hpp"
#include "fmrt_errors.hpp"
#include "fmrt_types.hpp"

namespace fmrt
{
    class DiagnosticsLayer
    {
    public:

        // ---------------------------------------------------------------------
        // buildOkEnvelope:
        //   Used when invariants are satisfied and step is accepted.
        //   Copies X_next and metrics into envelope, marks status = OK.
        // ---------------------------------------------------------------------
        void buildOkEnvelope(
            const StructuralState& X_next,
            const DerivedMetrics& metrics,
            EventType event_type,
            StateEnvelope& out_env
        ) const noexcept
        {
            out_env.state          = X_next;
            out_env.metrics        = metrics;
            out_env.status         = StepStatus::OK;
            out_env.error_category = ErrorCategory::None;
            out_env.error_reason   = ERR_NONE;
            out_env.event_type     = event_type;
        }

        // ---------------------------------------------------------------------
        // buildErrorEnvelope:
        //   Used when step is rejected or any error occurs.
        //   X_preserved is usually X(t) (previous valid state).
        // ---------------------------------------------------------------------
        void buildErrorEnvelope(
            const StructuralState& X_preserved,
            const DerivedMetrics& metrics_preserved,
            EventType event_type,
            ErrorCategory category,
            const char* reason,
            StateEnvelope& out_env
        ) const noexcept
        {
            out_env.state          = X_preserved;
            out_env.metrics        = metrics_preserved;
            out_env.status         = StepStatus::ERROR;
            out_env.error_category = category;
            out_env.error_reason   = (reason != nullptr ? reason : errorCategoryToString(category));
            out_env.event_type     = event_type;
        }

        // ---------------------------------------------------------------------
        // buildDeadEnvelope:
        //   Used when organism is in terminal (collapsed) state κ = 0.
        //   All future non-RESET events should return this envelope unchanged.
        // ---------------------------------------------------------------------
        void buildDeadEnvelope(
            const StructuralState& X_dead,
            const DerivedMetrics& metrics_dead,
            EventType event_type,
            StateEnvelope& out_env
        ) const noexcept
        {
            out_env.state          = X_dead;
            out_env.metrics        = metrics_dead;
            out_env.status         = StepStatus::DEAD;
            out_env.error_category = ErrorCategory::PostCollapse;
            out_env.error_reason   = ERR_POST_COLLAPSE;
            out_env.event_type     = event_type;
        }
    };

} // namespace fmrt
