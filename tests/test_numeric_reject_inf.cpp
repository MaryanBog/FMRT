#include <cmath>
#include <iostream>
#include "fmrt_api.hpp"

using namespace fmrt;

int test_numeric_reject_INF()
{
    std::cout << "Running numeric_reject_INF...\n";

    // +∞ и -∞
    const double P = std::numeric_limits<double>::infinity();
    const double N = -std::numeric_limits<double>::infinity();

    StructEvent E{};
    E.type = EventType::Update;
    E.dt = 1.0;
    E.stimulus = {0,0,0,0};

    {
        StructuralState X{};
        X.Delta = {P, 0, 0, 0};
        StateEnvelope out = FMRT_Step(X, E);

        if (out.status != StepStatus::ERROR ||
            out.error_category != ErrorCategory::NumericError ||
            !out.isFinite())
        {
            std::cerr << "numeric_reject_INF FAILED: INF in Δ not rejected\n";
            return 1;
        }
    }

    {
        StructuralState X{};
        X.Phi = N;
        StateEnvelope out = FMRT_Step(X, E);

        if (out.status != StepStatus::ERROR ||
            out.error_category != ErrorCategory::NumericError ||
            !out.isFinite())
        {
            std::cerr << "numeric_reject_INF FAILED: INF in Φ not rejected\n";
            return 1;
        }
    }

    {
        StructuralState X{};
        X.Kappa = P;
        StateEnvelope out = FMRT_Step(X, E);

        if (out.status != StepStatus::ERROR ||
            out.error_category != ErrorCategory::NumericError ||
            !out.isFinite())
        {
            std::cerr << "numeric_reject_INF FAILED: INF in κ not rejected\n";
            return 1;
        }
    }

    {
        StructuralState X{};
        X.M = N;
        StateEnvelope out = FMRT_Step(X, E);

        if (out.status != StepStatus::ERROR ||
            out.error_category != ErrorCategory::NumericError ||
            !out.isFinite())
        {
            std::cerr << "numeric_reject_INF FAILED: INF in M not rejected\n";
            return 1;
        }
    }

    std::cout << "numeric_reject_INF OK\n";
    return 0;
}
