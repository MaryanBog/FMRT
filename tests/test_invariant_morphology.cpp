#include <iostream>
#include <cmath>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_morphology()
{
    // Структура, создающая высокую кривизну и напряжение.
    StructuralState X;
    X.Delta = {5.0, -4.0, 3.0, -2.0};
    X.Phi   = 10.0;
    X.M     = 1.0;
    X.Kappa = 1.0;
    X.RegimePrev = Regime::ACC;

    StructEvent E;
    E.type = EventType::Update;
    E.dt   = 1.0;
    E.stimulus = {3.0, -3.0, 5.0, -5.0};

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- MORPHOLOGY INVARIANT DUMP ---\n";
        std::cout << "mu      = " << out.metrics.mu << "\n";
        std::cout << "R       = " << out.metrics.curvature_R << "\n";
        std::cout << "kappa   = " << out.state.Kappa << "\n";
        std::cout << "det_g   = " << out.metrics.det_g << "\n";
        std::cout << "tau     = " << out.metrics.tau << "\n";
        std::cout << "status  = " << (int)out.status << "\n";
    };

    // Проверка finiteness
    if (!std::isfinite(out.metrics.mu))
    {
        dump();
        return 1;
    }

    // Проверка границ диапазона
    if (out.metrics.mu < 0.0 || out.metrics.mu > 1.0)
    {
        dump();
        return 1;
    }

    std::cout << "Morphology invariant OK. mu=" << out.metrics.mu << "\n";
    return 0;
}
