#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_heartbeat_high_memory()
{
    StructuralState X{};
    X.Delta = {0.6, -0.4, 0.3, -0.2};
    X.Phi   = 1.2;
    X.M     = 100.0;    // высокий уровень памяти
    X.Kappa = 0.9;
    X.RegimePrev = Regime::ACC;

    StructEvent E{};
    E.type = EventType::Heartbeat;
    E.dt   = 1.0;

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- HEARTBEAT HIGH MEMORY DUMP ---\n";
        std::cout << "Δ: ";
        for (double v : out.state.Delta) std::cout << v << " ";
        std::cout << "\nΦ = " << out.state.Phi
                  << "\nM = " << out.state.M
                  << "\nκ = " << out.state.Kappa
                  << "\nμ = " << out.metrics.mu
                  << "\nregime = " << (int)out.metrics.regime
                  << "\nstatus = " << (int)out.status
                  << "\n";
    };

    // 1) шаг должен быть ОК
    if (out.status != StepStatus::OK)
    {
        std::cout << "ERROR: heartbeat with high M rejected\n";
        dump();
        return 1;
    }

    // 2) Δ должна немного уменьшиться (релаксация)
    for (size_t i = 0; i < 4; ++i)
    {
        if (std::abs(out.state.Delta[i]) > std::abs(X.Delta[i]) + 1e-12)
        {
            std::cout << "ERROR: Δ did not relax under heartbeat\n";
            dump();
            return 1;
        }
    }

    // 3) Φ должна уменьшиться (heartbeat → затухание)
    if (out.state.Phi >= X.Phi)
    {
        std::cout << "ERROR: Φ did not decay under heartbeat\n";
        dump();
        return 1;
    }

    // 4) M должна увеличиться (τ-аккумуляция)
    if (out.state.M <= X.M)
    {
        std::cout << "ERROR: M did not accumulate under heartbeat\n";
        dump();
        return 1;
    }

    // 5) κ должна остаться > 0
    if (out.state.Kappa <= 0.0)
    {
        std::cout << "ERROR: κ collapsed under heartbeat\n";
        dump();
        return 1;
    }

    // 6) РЕЖИМ МОЖЕТ СТАТЬ COL — это корректно при большом M
    // значит НИКАКОЙ проверки режима здесь быть НЕ должно.

    std::cout << "Heartbeat high memory OK.\n";
    return 0;
}
