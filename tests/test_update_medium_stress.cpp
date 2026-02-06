#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_update_medium_stress()
{
    // Исходное состояние — спокойное
    StructuralState X{};
    X.Delta = {0.5, -0.3, 0.2, -0.1};
    X.Phi   = 1.0;
    X.M     = 2.0;
    X.Kappa = 0.8;
    X.RegimePrev = Regime::ACC;

    // Средний стресс — заметный, но не разрушающий
    StructEvent E{};
    E.type = EventType::Update;
    E.dt   = 1.0;
   E.stimulus = {0.2, -0.25, 0.15, -0.1};

    // Шаг
    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- MEDIUM STRESS UPDATE DUMP ---\n";
        std::cout << "Δ: ";
        for (double v : out.state.Delta) std::cout << v << " ";
        std::cout << "\nΦ = " << out.state.Phi << "\n";
        std::cout << "M = " << out.state.M << "\n";
        std::cout << "κ = " << out.state.Kappa << "\n";
        std::cout << "μ = " << out.metrics.mu << "\n";
        std::cout << "regime = " << (int)out.metrics.regime << "\n";
        std::cout << "status = " << (int)out.status << "\n";
    };

    // 1) Шаг должен быть принят
    if (out.status != StepStatus::OK)
    {
        std::cout << "ERROR: medium stress UPDATE rejected\n";
        dump();
        return 1;
    }

    // 2) κ не должен обрушиться
    if (out.state.Kappa <= 0.0)
    {
        std::cout << "ERROR: κ collapsed under medium stress\n";
        dump();
        return 1;
    }

    // 3) Δ должна увеличиться заметно (но не катастрофически)
    double dnorm = 0.0;
    for (size_t i = 0; i < 4; ++i)
        dnorm += std::abs(out.state.Delta[i] - X.Delta[i]);

    if (dnorm < 0.5 || dnorm > 10.0)
    {
        std::cout << "ERROR: Δ deformation out of medium range\n";
        dump();
        return 1;
    }

    // 4) Φ должна вырасти
    if (out.state.Phi <= X.Phi)
    {
        std::cout << "ERROR: Φ did not increase under medium stress\n";
        dump();
        return 1;
    }

    // 5) μ должна увеличиться, но быть далеко от collapse (<= 0.7)
    if (out.metrics.mu <= 0.0)
    {
        std::cout << "ERROR: μ did not increase\n";
        dump();
        return 1;
    }

    if (out.metrics.mu > 0.7)
    {
        std::cout << "ERROR: μ too high for medium stress\n";
        dump();
        return 1;
    }

    // 6) режим должен быть ACC или DEV, но не COL
    if (out.metrics.regime == Regime::COL)
    {
        std::cout << "ERROR: medium stress caused collapse regime\n";
        dump();
        return 1;
    }

    std::cout << "Medium stress UPDATE OK.\n";
    return 0;
}
