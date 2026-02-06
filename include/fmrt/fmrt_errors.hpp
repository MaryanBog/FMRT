#pragma once
//
// FMRT Core V2.2
// fmrt_errors.hpp
//
// Deterministic error messages and mappings for FMRT diagnostics.
// No dynamic memory, no allocations, no runtime formatting.
//

#include "fmrt_types.hpp"

namespace fmrt
{
    // -------------------------------------------------------------------------
    // Deterministic, static error messages (do NOT modify at runtime)
    // -------------------------------------------------------------------------

    static constexpr const char* ERR_NONE =
        "no_error";

    static constexpr const char* ERR_INVALID_EVENT =
        "invalid_event";

    static constexpr const char* ERR_INVALID_STATE =
        "invalid_state";

    static constexpr const char* ERR_INVARIANT_VIOLATION =
        "invariant_violation";

    static constexpr const char* ERR_FORBIDDEN_DOMAIN =
        "forbidden_domain_transition";

    static constexpr const char* ERR_NUMERIC_ERROR =
        "numeric_error";

    static constexpr const char* ERR_POST_COLLAPSE =
        "post_collapse_event_rejected";

    static constexpr const char* ERR_UNSUPPORTED =
        "unsupported_operation";

    // -------------------------------------------------------------------------
    // Deterministic mapping ErrorCategory -> static const char*
    // -------------------------------------------------------------------------

    inline constexpr const char* errorCategoryToString(ErrorCategory cat) noexcept
    {
        switch (cat)
        {
            case ErrorCategory::None:              return ERR_NONE;
            case ErrorCategory::InvalidEvent:      return ERR_INVALID_EVENT;
            case ErrorCategory::InvalidState:      return ERR_INVALID_STATE;
            case ErrorCategory::InvariantViolation:return ERR_INVARIANT_VIOLATION;
            case ErrorCategory::ForbiddenDomain:   return ERR_FORBIDDEN_DOMAIN;
            case ErrorCategory::NumericError:      return ERR_NUMERIC_ERROR;
            case ErrorCategory::PostCollapse:      return ERR_POST_COLLAPSE;
            case ErrorCategory::UnsupportedOperation:return ERR_UNSUPPORTED;
        }
        return ERR_UNSUPPORTED; // fallback (never reached)
    }

} // namespace fmrt
