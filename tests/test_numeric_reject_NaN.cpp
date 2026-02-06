#pragma once
#include <iostream>
#include "fmrt_api.hpp"
using namespace fmrt;

int test_numeric_reject_NaN()
{
    std::cout << "Running numeric_reject_NaN...\n";

    // ---------- 1) Δ contains NaN ----------
    {
        StructuralState X{};
        X.reset();
        X.Delta[0] = std::nan("1");        // FORCE NaN

        StructEvent E{};
        E.type = EventType::Heartbeat;
        E.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, E);

        if (out.status == StepStatus::OK) {
            std::cerr << "numeric_reject_NaN FAILED: Delta NaN was NOT rejected\n";
            return 1;
        }
        if (!out.isFinite()) {
            std::cerr << "numeric_reject_NaN FAILED: output contains NaN\n";
            return 1;
        }
    }

    // ---------- 2) Φ = NaN ----------
    {
        StructuralState X{};
        X.reset();
        X.Phi = std::nan("1");

        StructEvent E{};
        E.type = EventType::Heartbeat;
        E.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, E);

        if (out.status == StepStatus::OK) {
            std::cerr << "numeric_reject_NaN FAILED: Phi NaN was NOT rejected\n";
            return 1;
        }
        if (!out.isFinite()) {
            std::cerr << "numeric_reject_NaN FAILED: output contains NaN\n";
            return 1;
        }
    }

    // ---------- 3) κ = NaN ----------
    {
        StructuralState X{};
        X.reset();
        X.Kappa = std::nan("1");

        StructEvent E{};
        E.type = EventType::Gap;
        E.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, E);

        if (out.status == StepStatus::OK) {
            std::cerr << "numeric_reject_NaN FAILED: Kappa NaN was NOT rejected\n";
            return 1;
        }
        if (!out.isFinite()) {
            std::cerr << "numeric_reject_NaN FAILED: output contains NaN\n";
            return 1;
        }
    }

    // ---------- 4) Event.dt = NaN ----------
    {
        StructuralState X{};
        X.reset();

        StructEvent E{};
        E.type = EventType::Update;
        E.dt   = std::nan("1");

        StateEnvelope out = FMRT_Step(X, E);

        if (out.status == StepStatus::OK) {
            std::cerr << "numeric_reject_NaN FAILED: event dt NaN was NOT rejected\n";
            return 1;
        }
        if (!out.isFinite()) {
            std::cerr << "numeric_reject_NaN FAILED: output contains NaN\n";
            return 1;
        }
    }

    std::cout << "numeric_reject_NaN OK.\n";
    return 0;
}
