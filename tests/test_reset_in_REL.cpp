#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_reset_in_REL()
{
    // "Плохое" состояние, имитируем REL: большая Φ, M, низкая κ
    StructuralState X{};
    X.Delta = {2.0, -1.5, 1.2, -1.0};
    X.Phi   = 5.0;
    X.M     = 20.0;
    X.Kappa = 0.3;
    X.RegimePrev = Regime::REL;   // считаем, что были в REL

    // RESET-событие
    StructEvent ER{};
    ER.type = EventType::Reset;
    ER.dt   = 1.0;

    StateEnvelope out = FMRT_Step(X, ER);

    auto dump = [&]() {
        std::cout << "--- RESET IN REL DUMP ---\n";
        std::cout << "Δ: ";
        for (double v : out.state.Delta) std::cout << v << " ";
        std::cout << "\nΦ = " << out.state.Phi
                  << "\nM = " << out.state.M
                  << "\nκ = " << out.state.Kappa
                  << "\nregime = " << (int)out.metrics.regime
                  << "\nstatus = " << (int)out.status
                  << "\n";
    };

    // 1) RESET должен быть принят
    if (out.status != StepStatus::OK)
    {
        std::cout << "ERROR: RESET in REL-like state was rejected\n";
        dump();
        return 1;
    }

    // 2) Δ должна стать нулевой
    for (double v : out.state.Delta)
    {
        if (std::abs(v) > 1e-12)
        {
            std::cout << "ERROR: Δ not cleared on RESET\n";
            dump();
            return 1;
        }
    }

    // 3) Φ должна сброситься к 0
    if (out.state.Phi != 0.0)
    {
        std::cout << "ERROR: Φ not reset\n";
        dump();
        return 1;
    }

    // 4) M должна быть 0
    if (out.state.M != 0.0)
    {
        std::cout << "ERROR: M not reset\n";
        dump();
        return 1;
    }

    // 5) κ должна стать 1
    if (std::abs(out.state.Kappa - 1.0) > 1e-12)
    {
        std::cout << "ERROR: κ not reset to 1\n";
        dump();
        return 1;
    }

    // 6) режим после RESET должен быть ACC
    if (out.metrics.regime != Regime::ACC)
    {
        std::cout << "ERROR: RESET did not restore ACC regime\n";
        dump();
        return 1;
    }

    std::cout << "Reset in REL OK.\n";
    return 0;
}
