#include <iostream>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_metric_positivity()
{
    // Создаем состояние, которое вызывает высокую кривизну,
    // чтобы проверить, что det_g остаётся > 0.
    StructuralState X;
    X.Delta = {2.0, -3.0, 4.0, -1.0};
    X.Phi   = 5.0;
    X.M     = 20.0;
    X.Kappa = 0.9;
    X.RegimePrev = Regime::DEV;

    StructEvent E;
    E.type = EventType::Update;
    E.dt   = 0.5;
    E.stimulus = {1.0, -1.0, 2.0, -2.0};

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- METRIC INVARIANT DUMP ---\n";
        std::cout << "det_g  = " << out.metrics.det_g << "\n";
        std::cout << "kappa  = " << out.state.Kappa << "\n";
        std::cout << "tau    = " << out.metrics.tau << "\n";
        std::cout << "mu     = " << out.metrics.mu << "\n";
        std::cout << "status = " << (int)out.status << "\n";
    };

    // Metric must ALWAYS be finite
    if (!std::isfinite(out.metrics.det_g))
    {
        dump();
        return 1;
    }

    // Case 1 — active organism (κ > 0)
    if (out.state.Kappa > 0.0)
    {
        if (!(out.metrics.det_g > 0.0))
        {
            std::cout << "ERROR: det_g must be > 0 when kappa > 0\n";
            dump();
            return 1;
        }
    }

    // Case 2 — collapse (κ == 0)
    if (out.state.Kappa == 0.0)
    {
        if (out.metrics.det_g != 0.0)
        {
            std::cout << "ERROR: det_g must be 0 when kappa == 0\n";
            dump();
            return 1;
        }
    }

    std::cout << "Metric determinant invariant OK. det_g=" << out.metrics.det_g << "\n";
    return 0;
}
