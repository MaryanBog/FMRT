#include <iostream>
#include <cmath>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_gap_sequence_stability()
{
    StructuralState X{};
    X.Delta = {0.4, -0.2, 0.3, -0.1};
    X.Phi   = 1.5;
    X.M     = 5.0;
    X.Kappa = 0.9;
    X.RegimePrev = Regime::ACC;

    StructEvent E{};
    E.type = EventType::Gap;
    E.dt   = 1.0;

    StateEnvelope out1 = FMRT_Step(X, E);
    StateEnvelope out2 = FMRT_Step(out1.state, E);
    StateEnvelope out3 = FMRT_Step(out2.state, E);

    auto dump = [&](const char* label, const StateEnvelope& out) {
        std::cout << "--- " << label << " ---\n";
        std::cout << "Δ: ";
        for (double v : out.state.Delta) std::cout << v << " ";
        std::cout << "\nΦ = " << out.state.Phi
                  << "\nM = " << out.state.M
                  << "\nκ = " << out.state.Kappa
                  << "\nregime = " << (int)out.metrics.regime
                  << "\nstatus = " << (int)out.status << "\n\n";
    };

    // 1) Все шаги должны быть успешными
    if (out1.status != StepStatus::OK ||
        out2.status != StepStatus::OK ||
        out3.status != StepStatus::OK)
    {
        std::cout << "ERROR: GAP sequence produced invalid step\n";
        dump("step1", out1);
        dump("step2", out2);
        dump("step3", out3);
        return 1;
    }

    // 2) Δ должна РАССЛАБЛЯТЬСЯ (уменьшаться по модулю)
    for (size_t i = 0; i < 4; ++i)
    {
        if (std::abs(out3.state.Delta[i]) >= std::abs(X.Delta[i]))
        {
            std::cout << "ERROR: Δ did not relax during GAP sequence\n";
            dump("final", out3);
            return 1;
        }
    }

    // 3) Φ должна уменьшаться (релаксация)
    if (!(out3.state.Phi < out2.state.Phi &&
          out2.state.Phi < out1.state.Phi))
    {
        std::cout << "ERROR: Φ did not relax during GAP sequence\n";
        dump("step1", out1);
        dump("step2", out2);
        dump("step3", out3);
        return 1;
    }

    // 4) κ должна слегка уменьшиться, но не упасть слишком сильно
    if (out3.state.Kappa <= 0.0)
    {
        std::cout << "ERROR: κ collapsed during GAP sequence\n";
        dump("final", out3);
        return 1;
    }

    if (out3.state.Kappa < 0.5)
    {
        std::cout << "ERROR: κ decayed too much under GAP\n";
        dump("final", out3);
        return 1;
    }

    // 5) Режим не должен быть COL
    if (out3.metrics.regime == Regime::COL)
    {
        std::cout << "ERROR: regime collapsed during GAP\n";
        dump("final", out3);
        return 1;
    }

    std::cout << "GAP sequence stability OK.\n";
    return 0;
}
