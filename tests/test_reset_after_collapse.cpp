#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_reset_after_collapse()
{
    // Состояние полностью сломано
    StructuralState X{};
    X.Delta = {3.0, -2.0, 1.5, -1.0};
    X.Phi   = 10.0;
    X.M     = 200.0;
    X.Kappa = 0.0;              // <-- ключевое: уже collapse
    X.RegimePrev = Regime::COL; // предыдущий режим тоже collapse

    // RESET event
    StructEvent ER{};
    ER.type = EventType::Reset;
    ER.dt   = 1.0;

    StateEnvelope out = FMRT_Step(X, ER);

    auto dump = [&]() {
        std::cout << "--- RESET AFTER COLLAPSE DUMP ---\n";
        std::cout << "Δ: ";
        for (double v : out.state.Delta) std::cout << v << " ";
        std::cout << "\nΦ = " << out.state.Phi
                  << "\nM = " << out.state.M
                  << "\nκ = " << out.state.Kappa
                  << "\nregime = " << (int)out.metrics.regime
                  << "\nstatus = " << (int)out.status
                  << "\n";
    };

    // 1) RESET должен быть успешным
    if (out.status != StepStatus::OK)
    {
        std::cout << "ERROR: RESET after collapse rejected\n";
        dump();
        return 1;
    }

    // 2) Δ очищена
    for (double v : out.state.Delta)
    {
        if (std::abs(v) > 1e-12)
        {
            std::cout << "ERROR: Δ not cleared on RESET after collapse\n";
            dump();
            return 1;
        }
    }

    // 3) Φ = 0
    if (out.state.Phi != 0.0)
    {
        std::cout << "ERROR: Φ not reset after collapse\n";
        dump();
        return 1;
    }

    // 4) M = 0
    if (out.state.M != 0.0)
    {
        std::cout << "ERROR: M not reset after collapse\n";
        dump();
        return 1;
    }

    // 5) κ = 1
    if (std::abs(out.state.Kappa - 1.0) > 1e-12)
    {
        std::cout << "ERROR: κ was not restored to 1 after collapse\n";
        dump();
        return 1;
    }

    // 6) Режим после RESET должен быть ACC
    if (out.metrics.regime != Regime::ACC)
    {
        std::cout << "ERROR: reset did not restore ACC regime after collapse\n";
        dump();
        return 1;
    }

    std::cout << "Reset after collapse OK.\n";
    return 0;
}
