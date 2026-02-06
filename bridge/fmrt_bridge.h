#pragma once
#include <cstdint>

#ifdef _WIN32
#define FMRT_API extern "C" __declspec(dllexport)
#else
#define FMRT_API extern "C"
#endif

struct fmrt_bridge_event
{
    std::uint8_t type;
    double       dt;
    double       stimulus[4];
};

struct fmrt_bridge_envelope
{
    std::uint8_t status;
    bool         invariants_ok;
    double       derived[4];
};

FMRT_API int  __stdcall FMRT_Step(const fmrt_bridge_event* ev,
                                  fmrt_bridge_envelope*    out);

FMRT_API void __stdcall FMRT_Reset();
