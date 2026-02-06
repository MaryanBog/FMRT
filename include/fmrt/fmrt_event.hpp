#pragma once
//
// FMRT Core V2.2
// fmrt_event.hpp
//
// Canonical structural event representation E(t).
// FMRT accepts exactly four event types:
//   STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET
//

#include <array>
#include <cstddef>
#include <cstdint>

#include "fmrt_constants.hpp"
#include "fmrt_types.hpp"
#include "fmrt_config.hpp"

namespace fmrt
{
    struct StructEvent
    {
        EventType type = EventType::Heartbeat;    // default: minimal tick
        double dt = 0.0;                          // required for UPDATE/GAP/HEARTBEAT
        std::array<double, DELTA_DIM> stimulus{}; // deformation for UPDATE only
        const char* reason = nullptr;             // RESET optional message

        // ---------------------------------------------------------------------
        // Validate event shape according to FMRT Specification V2.2
        // ---------------------------------------------------------------------
        bool isFinite() const noexcept
        {
            // RESET: stimulus не используется, проверяем только dt
            if (type == EventType::Reset)
                return is_finite(dt);

            if (!is_finite(dt))
                return false;

            for (auto v : stimulus)
                if (!is_finite(v))
                    return false;

            return true;
        }

        bool hasValidDt() const noexcept
        {
            if (type == EventType::Reset)
                return true; // RESET не использует dt в эволюции

            return dt > 0.0;
        }

        // Update event requires finite stimulus
        bool stimulusNeeded() const noexcept
        {
            return type == EventType::Update;
        }
    };

} // namespace fmrt
