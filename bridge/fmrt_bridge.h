// fmrt_bridge.h
#pragma once
#include <cstdint>

#ifdef _WIN32
  #define FMRT_CALL __stdcall
  #define FMRT_API  extern "C" __declspec(dllexport)
#else
  #define FMRT_CALL
  #define FMRT_API  extern "C"
#endif

// IMPORTANT: ABI structs must use fixed-size types only.
// Avoid bool in cross-language boundaries.

struct fmrt_bridge_event
{
    std::uint8_t type;        // must map to fmrt::EventType
    double       dt;          // must be finite and > 0
    double       stimulus[4]; // must be finite; dimension fixed to 4 in this bridge
};

struct fmrt_bridge_envelope
{
    std::uint8_t status;        // must map to fmrt::StepStatus (OK/ERROR/COLLAPSE only)
    std::uint8_t invariants_ok; // 1 = true, 0 = false
    double       derived[4];     // last slot reserved for ABI stability
};

// return codes (bridge-level)
enum : int
{
    FMRT_BRIDGE_OK                = 0,
    FMRT_BRIDGE_E_NULLPTR         = -1,
    FMRT_BRIDGE_E_BAD_INPUT       = -2,
    FMRT_BRIDGE_E_FATAL_STATUS    = -3
};

FMRT_API int  FMRT_CALL FMRT_Step(const fmrt_bridge_event* ev,
                                  fmrt_bridge_envelope*    out);

FMRT_API void FMRT_CALL FMRT_Reset();

