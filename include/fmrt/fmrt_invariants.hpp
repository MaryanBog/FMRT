#pragma once
//
// FMRT Core V2.2
// fmrt_invariants.hpp
//
// Invariant flags and validation result structure.
// These flags represent the outcome of checking all FMT 3.1 invariants:
//
//  - memory irreversibility
//  - viability non-negativity
//  - metric positivity
//  - temporal density positivity
//  - regime irreversibility
//  - morphology bounds
//  - collapse geometry consistency
//  - forbidden-domain protection
//

#include <cstdint>

#include "fmrt_types.hpp"
#include "fmrt_config.hpp"

namespace fmrt
{
    struct InvariantStatus
    {
        // Bitmask of invariant violations or confirmations.
        // Bits correspond to InvariantBits enum from fmrt_types.hpp
        uint32_t flags = 0;

        // Final indicator: all invariants satisfied?
        bool all_ok = false;

        // ---------------------------------------------------------------------
        // Helpers to set/check bits (pure inline, deterministic)
        // ---------------------------------------------------------------------
        inline void set(uint32_t bit) noexcept
        {
            flags |= bit;
        }

        inline bool check(uint32_t bit) const noexcept
        {
            return (flags & bit) != 0;
        }

        inline void clear() noexcept
        {
            flags = 0;
            all_ok = false;
        }
    };

} // namespace fmrt
