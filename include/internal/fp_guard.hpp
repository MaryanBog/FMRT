#pragma once
//
// FMRT Core V2.2
// fp_guard.hpp
//
// Ensures strict IEEE-754 environment, FE_TONEAREST rounding,
// and finiteness checks for all numeric values.
// This module contains ONLY inline constexpr / noexcept logic.
// No allocations, no I/O, no exceptions.
//

#include <cfenv>
#include <cmath>

#include "fmrt_config.hpp"

namespace fmrt
{
    class FpGuard
    {
    public:

        // ---------------------------------------------------------------------
        // verifyEnvironment()
        // Confirms that FP environment matches FMRT requirements.
        // Should be called only in debug or in safe builds — does NOTHING
        // in release, but exists for static analyzers.
        // ---------------------------------------------------------------------
        inline bool verifyEnvironment() const noexcept
        {
#if defined(__GNUC__) || defined(__clang__)
            // Must use FE_TONEAREST always.
            if constexpr (ENABLE_FP_GUARDS)
            {
                if (std::fegetround() != REQUIRED_ROUNDING_MODE)
                    return false;
            }
#endif
            return true;
        }

        // ---------------------------------------------------------------------
        // numericSafe(x)
        // Returns true if x is NOT NaN, NOT Inf, NOT denormal (subnormal).
        // ---------------------------------------------------------------------
        inline bool numericSafe(double x) const noexcept
        {
            if (!std::isfinite(x))
                return false;

            // Detect extremely small denormals that may break deterministic behavior.
            if (x != 0.0 && std::fpclassify(x) == FP_SUBNORMAL)
                return false;

            return true;
        }

        // ---------------------------------------------------------------------
        // safeAssign(dst, src)
        // Assigns src to dst only if it is numeric safe.
        // If not — dst is set to 0 (deterministic fallback).
        // ---------------------------------------------------------------------
        inline void safeAssign(double& dst, double src) const noexcept
        {
            dst = numericSafe(src) ? src : 0.0;
        }
    };

} // namespace fmrt
