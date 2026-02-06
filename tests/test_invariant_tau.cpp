#include <iostream>
#include <cmath>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_tau()
{
    // Живое состояние
    StructuralState X;
    X.Delta = {1.0, -0.5, 0.25, -0.75};
    X.Phi   = 0.3;
    X.M     = 1.0;
    X.Kappa = 0.8;           // ЖИВОЕ (tau > 0)
    X.RegimePrev = Regime::ACC;

    StructEvent E;
    E.type = EventType::Heartbeat;
    E.dt   = 1.0;

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- TAU INVARIANT DUMP ---\n";
        std::cout << "kappa = " << out.state.Kappa << "\n";
        std::cout << "tau   = " << out.metrics.tau << "\n";
        std::cout << "status = " << (int)out.status << "\n";
        std::cout << "det_g = " << out.metrics.det_g << "\n";
    };

    // Tau must be finite
    if (!std::isfinite(out.metrics.tau))
    {
        dump();
        return 1;
    }

    // CASE 1: κ > 0 → τ must be > 0
    if (out.state.Kappa > 0.0)
    {
        if (!(out.metrics.tau > 0.0))
        {
            std::cout << "ERROR: tau must be > 0 for kappa > 0\n";
            dump();
            return 1;
        }
    }

    // CASE 2: создадим collapse и проверим tau == 0
    // Сильно разрушим организм
    StructuralState X2 = X;
    X2.Kappa = 0.00001;

    StructEvent E2;
    E2.type = EventType::Update;
    E2.dt   = 1.0;
    E2.stimulus = {100, -100, 50, -50};

    StateEnvelope out2 = FMRT_Step(X2, E2);

    if (out2.state.Kappa == 0.0)
    {
        // collapse → tau must be 0
        if (out2.metrics.tau != 0.0)
        {
            std::cout << "ERROR: tau must be 0 in collapse\n";
            std::cout << "tau = " << out2.metrics.tau << "\n";
            return 1;
        }
    }

    std::cout << "Tau invariant OK. tau=" << out.metrics.tau << "\n";
    return 0;
}
