#include <iostream>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_kappa_nonnegative()
{
    // Выбираем состояние с очень маленькой κ,
    // чтобы UPDATE попытался "продавить" её в отрицательную область.
    StructuralState X;
    X.Delta = {1.0, 1.0, 1.0, 1.0};
    X.Phi   = 10.0;
    X.M     = 50.0;
    X.Kappa = 0.0001;  // почти collapse
    X.RegimePrev = Regime::REL;

    // Мощный UPDATE, который создаст огромную кривизну → decay κ
    StructEvent E;
    E.type = EventType::Update;
    E.dt   = 1.0;
    E.stimulus = {10.0, -10.0, 20.0, -20.0};

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- KAPPA INVARIANT DUMP ---\n";
        std::cout << "X.Kappa      = " << X.Kappa << "\n";
        std::cout << "out.Kappa    = " << out.state.Kappa << "\n";
        std::cout << "det_g        = " << out.metrics.det_g << "\n";
        std::cout << "tau          = " << out.metrics.tau << "\n";
        std::cout << "mu           = " << out.metrics.mu << "\n";
        std::cout << "regime       = " << (int)out.metrics.regime << "\n";
        std::cout << "status       = " << (int)out.status << "\n";
    };

    // κ must NEVER be negative
    if (out.state.Kappa < 0.0)
    {
        dump();
        return 1;
    }

    // κ may collapse to exactly 0 — это корректно.
    // Проверим специфику collapse geometry.
    if (out.state.Kappa == 0.0)
    {
        if (!(out.metrics.det_g == 0.0 &&
              out.metrics.tau   == 0.0 &&
              out.metrics.mu    == 1.0 &&
              out.metrics.regime == Regime::COL))
        {
            std::cout << "Collapse geometry invalid!\n";
            dump();
            return 1;
        }
    }

    std::cout << "Kappa invariant OK. κ_new=" << out.state.Kappa << "\n";
    return 0;
}
