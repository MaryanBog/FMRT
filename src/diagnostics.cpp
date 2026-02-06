#pragma once
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

        // OK envelope (normal accepted step)
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

        // Error envelope (invalid step — invariants or numeric checks failed)
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
            out_env.error_reason   = (reason ? reason : errorCategoryToString(category));
            out_env.event_type     = event_type;
        }

        // Dead envelope (κ = 0 → terminal)
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
