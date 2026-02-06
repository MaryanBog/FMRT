#include <iostream>
#include <cmath>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_collapse_geometry()
{
    StructuralState X;
    X.Delta = {10.0, -8.0, 6.0, -4.0};
    X.Phi   = 20.0;
    X.M     = 50.0;
    X.Kappa = 0.001;
    X.RegimePrev = Regime::REL;

    StructEvent E;
    E.type = EventType::Update;
    E.dt   = 1.0;
    E.stimulus = {50.0, -50.0, 30.0, -30.0};

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- COLLAPSE GEOMETRY DUMP ---\n";
        std::cout << "Kappa      = " << out.state.Kappa << "\n";
        std::cout << "det_g      = " << out.metrics.det_g << "\n";
        std::cout << "tau        = " << out.metrics.tau << "\n";
        std::cout << "mu         = " << out.metrics.mu << "\n";
        std::cout << "regime     = " << (int)out.metrics.regime << "\n";
        std::cout << "is_collapse= " << out.metrics.is_collapse << "\n";
        std::cout << "status     = " << (int)out.status << "\n";
    };

    // Проверка κ = 0
    if (std::abs(out.state.Kappa) > fmrt::EPS)
    {
        std::cout << "ERROR: kappa must be 0 in collapse\n";
        dump();
        return 1;
    }

    // Геометрия коллапса по спецификации
    if (std::abs(out.metrics.det_g) > fmrt::EPS ||
        std::abs(out.metrics.tau)   > fmrt::EPS ||
        std::abs(out.metrics.mu - 1.0) > fmrt::EPS ||
        out.metrics.regime != Regime::COL ||
        out.metrics.is_collapse != true)
    {
        std::cout << "ERROR: collapse geometry mismatch\n";
        dump();
        return 1;
    }

    std::cout << "Collapse geometry invariant OK.\n";
    return 0;
}
