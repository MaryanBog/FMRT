#pragma once
//
// FMRT Core V2.2
// fmrt_types.hpp
//
// Core enumerations and compile-time types used across FMRT.
// All types must be deterministic, immutable and platform-independent.
//

#include <cstddef>
#include <cstdint>

namespace fmrt
{
    // -------------------------------------------------------------------------
    // Compile-time constants
    // -------------------------------------------------------------------------

    // Δ-dimensionality (fixed at compile time, must be O(1))
    constexpr std::size_t DELTA_DIM = 4; // Can be changed if needed

    // -------------------------------------------------------------------------
    // Event types
    // -------------------------------------------------------------------------

    enum class EventType : uint8_t
    {
        Update = 0,     // STRUCT_UPDATE
        Gap    = 1,     // STRUCT_GAP
        Heartbeat = 2,  // STRUCT_HEARTBEAT
        Reset  = 3      // STRUCT_RESET
    };

    // -------------------------------------------------------------------------
    // Regime state machine (ACC → DEV → REL → COL)
    // -------------------------------------------------------------------------

    enum class Regime : uint8_t
    {
        ACC = 0,
        DEV = 1,
        REL = 2,
        COL = 3
    };

    // -------------------------------------------------------------------------
    // Morphological class
    // -------------------------------------------------------------------------

    enum class MorphologyClass : uint8_t
    {
        Elastic = 0,
        Plastic = 1,
        Degenerate = 2,
        NearCollapse = 3
    };

    // -------------------------------------------------------------------------
    // Step status
    // -------------------------------------------------------------------------

    enum class StepStatus : uint8_t
    {
        OK = 0,
        ERROR = 1,
        DEAD = 2 // κ = 0 terminal organism
    };

    // -------------------------------------------------------------------------
    // Error categories (FMRT Specification V2.2)
    // -------------------------------------------------------------------------

    enum class ErrorCategory : uint8_t
    {
        None = 0,
        InvalidEvent,
        InvalidState,
        InvariantViolation,
        ForbiddenDomain,
        NumericError,
        PostCollapse,
        UnsupportedOperation
    };

    // -------------------------------------------------------------------------
    // Internal invariant flags bitmask
    // -------------------------------------------------------------------------

    enum InvariantBits : uint32_t
    {
        INV_MEMORY          = 1 << 0,
        INV_KAPPA           = 1 << 1,
        INV_METRIC          = 1 << 2,
        INV_TAU             = 1 << 3,
        INV_REGIME          = 1 << 4,
        INV_MORPHOLOGY      = 1 << 5,
        INV_COLLAPSE        = 1 << 6,
        INV_FORBIDDEN       = 1 << 7,
    };

} // namespace fmrt
