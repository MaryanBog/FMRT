#include "internal/event_handler.hpp"

namespace fmrt
{

bool EventHandler::validate(const StructEvent& E, StateEnvelope& out_env) const noexcept
{
    // ---------------------------------------------------------------------
    // RESET bypasses all stimulus checks. Only dt must be finite.
    // ---------------------------------------------------------------------
    if (E.type == EventType::Reset)
    {
        if (!is_finite(E.dt))
        {
            out_env.status = StepStatus::ERROR;
            out_env.error_category = ErrorCategory::InvalidEvent;
            out_env.error_reason = ERR_INVALID_EVENT;
            return false;
        }

        return true; // RESET always valid
    }

    // ---------------------------------------------------------------------
    // 1) Finite check for UPDATE / GAP / HEARTBEAT
    // ---------------------------------------------------------------------
    if (!E.isFinite())
    {
        out_env.status = StepStatus::ERROR;
        out_env.error_category = ErrorCategory::InvalidEvent;
        out_env.error_reason = ERR_INVALID_EVENT;
        return false;
    }

    // ---------------------------------------------------------------------
    // 2) dt rules
    // ---------------------------------------------------------------------
    if (!E.hasValidDt())
    {
        out_env.status = StepStatus::ERROR;
        out_env.error_category = ErrorCategory::InvalidEvent;
        out_env.error_reason = ERR_INVALID_EVENT;
        return false;
    }

    // ---------------------------------------------------------------------
    // 3) Event semantics
    // ---------------------------------------------------------------------
    switch (E.type)
    {
        case EventType::Update:
            // stimulus already validated by isFinite()
            break;

        case EventType::Gap:
        case EventType::Heartbeat:
            // no stimulus required
            break;

        default:
            out_env.status = StepStatus::ERROR;
            out_env.error_category = ErrorCategory::UnsupportedOperation;
            out_env.error_reason = ERR_UNSUPPORTED;
            return false;
    }

    return true;
}

void EventHandler::canonicalize(StructEvent& E) const noexcept
{
    // GAP & HEARTBEAT → zero stimulus
    if (E.type == EventType::Gap || E.type == EventType::Heartbeat)
    {
        for (auto &v : E.stimulus)
            v = 0.0;
    }

    // RESET → force dt = 0, ignore stimulus
    if (E.type == EventType::Reset)
    {
        E.dt = 0.0;
        for (auto& v : E.stimulus) v = 0.0;
    }

    // Clamp dt for UPDATE / GAP / HEARTBEAT
    if (E.dt < 0.0)  E.dt = 0.0;
    if (E.dt > 1e6)  E.dt = 1e6;
}

} // namespace fmrt
