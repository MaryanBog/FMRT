#pragma once
//
// FMRT Core V2.2
// fmrt_config.hpp
//
// Global compile-time configuration required for deterministic FMRT operation.
// Ensures strict IEEE-754 behavior, forbids fast-math and unsafe FP modes.
//

#include <cfenv>
#include <cmath>

namespace fmrt
{
    // -------------------------------------------------------------------------
    // Compile-time toggle: enable internal FP guards
    // (runtime checks ensuring IEEE-754 strict mode is active)
    // -------------------------------------------------------------------------
    constexpr bool ENABLE_FP_GUARDS = true;

    // -------------------------------------------------------------------------
    // Deny unsafe build modes (fast-math, relaxed FP, extended precision)
    // These conditions will be validated during static analysis.
    // -------------------------------------------------------------------------
#if defined(__FAST_MATH__)
#   error "FMRT ERROR: fast-math (-ffast-math) is forbidden by Static Analysis V2.2"
#endif

#if defined(_M_FP_PRECISE) && !defined(_M_FP_STRICT)
#   error "FMRT ERROR: MSVC must compile with /fp:strict"
#endif

    // -------------------------------------------------------------------------
    // Floating-point environment expectations
    // -------------------------------------------------------------------------
    constexpr int REQUIRED_ROUNDING_MODE = FE_TONEAREST;

    // Compile-time constant: verifying IEEE-754 compliance is required.
    // This constant must match the architecture used.
    // -------------------------------------------------------------------------
    constexpr bool IEEE754_REQUIRED = true;

    // -------------------------------------------------------------------------
    // Utility inline functions for checks (used by fp_guard module)
    // -------------------------------------------------------------------------

    inline bool is_finite(double x) noexcept
    {
        return std::isfinite(x);
    }

    inline bool is_nan(double x) noexcept
    {
        return std::isnan(x);
    }

    inline bool is_inf(double x) noexcept
    {
        return std::isinf(x);
    }

} // namespace fmrt
