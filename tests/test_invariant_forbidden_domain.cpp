#include <iostream>
#include <cmath>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_forbidden_domain()
{
    // Нормальное, адекватное состояние
    StructuralState X;
    X.Delta = {0.5, -0.3, 0.2, -0.1};
    X.Phi   = 0.7;
    X.M     = 2.0;
    X.Kappa = 0.9;
    X.RegimePrev = Regime::ACC;

    StructEvent E;
    E.type = EventType::Update;
    E.dt   = 1.0;
    E.stimulus = {0.1, -0.2, 0.05, -0.05};

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- FORBIDDEN DOMAIN DUMP ---\n";
        std::cout << "status      = " << (int)out.status << "\n";
        std::cout << "error cat   = " << (int)out.error_category << "\n";
        std::cout << "kappa       = " << out.state.Kappa << "\n";
        std::cout << "det_g       = " << out.metrics.det_g << "\n";
        std::cout << "tau         = " << out.metrics.tau << "\n";
        std::cout << "mu          = " << out.metrics.mu << "\n";
        std::cout << "inv_all_ok  = " << out.invariants.all_ok << "\n";
    };

    // Шаг должен быть принят
    if (out.status != StepStatus::OK || out.error_category != ErrorCategory::None)
    {
        std::cout << "ERROR: step should be OK in normal domain\n";
        dump();
        return 1;
    }

    // Все инварианты, включая ForbiddenDomain, должны быть ОК
    if (!out.invariants.all_ok)
    {
        std::cout << "ERROR: invariants not all OK in normal domain\n";
        dump();
        return 1;
    }

    // Дополнительно явно проверим условия forbidden-domain из кода:
    if (!out.state.isFinite()
        || !out.metrics.isFinite()
        || out.state.Kappa < 0.0)
    {
        std::cout << "ERROR: state/metrics not finite or kappa < 0\n";
        dump();
        return 1;
    }

    if (out.state.Kappa > 0.0)
    {
        if (out.metrics.det_g <= 0.0 || out.metrics.tau <= 0.0)
        {
            std::cout << "ERROR: det_g/tau not strictly positive in active domain\n";
            dump();
            return 1;
        }
    }

    std::cout << "Forbidden domain invariant OK.\n";
    return 0;
}
