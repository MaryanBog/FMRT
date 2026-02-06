#include <cstdio>
#include <cmath>
#include <limits>

#include "../bridge/fmrt_bridge.h"
#include "fmrt_types.hpp"

using namespace fmrt;

void test_bridge_numeric_reject_NaN()
{
    std::printf("Running bridge_numeric_reject_NaN...\n");

    fmrt_bridge_event ev{};
    fmrt_bridge_envelope out{};

    ev.type = (uint8_t)EventType::Update;
    ev.dt   = 0.1;

    for (int i = 0; i < 4; ++i)
        ev.stimulus[i] = 0.0;

    ev.stimulus[0] = std::numeric_limits<double>::quiet_NaN();

    FMRT_Reset();

    int rc = FMRT_Step(&ev, &out);

    if (rc != 0)
    {
        std::printf("FAILED (rc=%d)\n", rc);
        return;
    }

    if (out.status == (uint8_t)StepStatus::OK && out.invariants_ok)
    {
        std::printf("FAILED (accepted NaN)\n");
        return;
    }

    std::printf("OK\n");
}
