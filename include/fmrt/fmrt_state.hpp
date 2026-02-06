#pragma once
//
// FMRT Core V2.2
// fmrt_state.hpp
//
// StructuralState X(t) = (Δ(t), Φ(t), M(t), κ(t))
// This structure is the ONLY representation of the organism state.
// Must remain deterministic, finite, contiguous, and O(1).
//

#include <array>
#include <cstddef>
#include <cstdint>

#include "fmrt_constants.hpp"
#include "fmrt_types.hpp"
#include "fmrt_config.hpp"

namespace fmrt
{
struct StructuralState
{
    std::array<double, DELTA_DIM> Delta {};
    double Phi = 0.0;
    double M = 0.0;
    double Kappa = 1.0;

    // NEW: previous regime, needed for correct invariant behavior
    Regime RegimePrev = Regime::ACC;

    void reset()
    {
        for (auto &v : Delta) v = 0.0;

        Phi        = RESET_PHI;
        M          = 0.0;
        Kappa      = RESET_KAPPA;
        RegimePrev = Regime::ACC;   // NEW: baseline regime
    }

    bool isFinite() const noexcept
    {
        if (!is_finite(Phi) || !is_finite(M) || !is_finite(Kappa))
            return false;

        for (auto v : Delta)
            if (!is_finite(v))
                return false;

        return true;
    }

    bool isLiving() const noexcept { return Kappa > 0.0; }
    bool isCollapsed() const noexcept { return Kappa == 0.0; }
};


} // namespace fmrt
