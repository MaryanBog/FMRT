#include <iostream>
#include <limits>
#include "fmrt_api.hpp"

using namespace fmrt;

int test_numeric_reject_denormals()
{
    std::cout << "Running numeric_reject_denormals...\n";

    // denormal value
    const double d = 1e-320;   // subnormal
    const double nd = -1e-320;

    StructEvent E{};
    E.type = EventType::Update;
    E.dt = 1.0;
    E.stimulus = {0,0,0,0};

    // --- Δ contains denormal ---
    {
        StructuralState X{};
        X.Delta = {d, 0, 0, 0};

        StateEnvelope out = FMRT_Step(X, E);
        
        if (!out.isFinite())
        {
            std::cerr << "numeric_reject_denormals FAILED: output not finite\n";
            return 1;
        }

        if (out.status != StepStatus::ERROR ||
            out.error_category != ErrorCategory::NumericError)
        {
            std::cerr << "numeric_reject_denormals FAILED: denormal in Delta NOT rejected\n";
            return 1;
        }
    }

    // --- Φ contains denormal ---
    {
        StructuralState X{};
        X.Phi = d;
        StateEnvelope out = FMRT_Step(X, E);

        if (!out.isFinite())
        {
            std::cerr << "numeric_reject_denormals FAILED: output not finite\n";
            return 1;
        }

        if (out.status != StepStatus::ERROR)
        {
            std::cerr << "numeric_reject_denormals FAILED: denormal in Phi NOT rejected\n";
            return 1;
        }
    }

    // --- κ contains denormal ---
    {
        StructuralState X{};
        X.Kappa = d;

        StateEnvelope out = FMRT_Step(X, E);

        if (!out.isFinite())
        {
            std::cerr << "numeric_reject_denormals FAILED: output not finite\n";
            return 1;
        }

        if (out.status != StepStatus::ERROR)
        {
            std::cerr << "numeric_reject_denormals FAILED: denormal in Kappa NOT rejected\n";
            return 1;
        }
    }

    // --- M contains denormal ---
    {
        StructuralState X{};
        X.M = nd;

        StateEnvelope out = FMRT_Step(X, E);

        if (!out.isFinite())
        {
            std::cerr << "numeric_reject_denormals FAILED: output not finite\n";
            return 1;
        }

        if (out.error_category != ErrorCategory::NumericError)
        {
            std::cerr << "numeric_reject_denormals FAILED: denormal in M NOT rejected\n";
            return 1;
        }
    }

    std::cout << "numeric_reject_denormals OK\n";
    return 0;
}
