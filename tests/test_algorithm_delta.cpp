#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_algorithm_delta()
{
    // Базовое нулевое состояние
    StructuralState X{};
    X.Delta      = {0.0, 0.0, 0.0, 0.0};
    X.Phi        = 0.0;
    X.M          = 0.0;
    X.Kappa      = 1.0;
    X.RegimePrev = Regime::ACC;

    // UPDATE со стимулом
    StructEvent E{};
    E.type     = EventType::Update;
    E.dt       = 1.0;
    E.stimulus = {1.0, -2.0, 3.5, 0.0};

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]() {
        std::cout << "--- ALGO Δ DUMP ---\n";
        std::cout << "Delta_out = { "
                  << out.state.Delta[0] << ", "
                  << out.state.Delta[1] << ", "
                  << out.state.Delta[2] << ", "
                  << out.state.Delta[3] << " }\n";
        std::cout << "status    = " << (int)out.status << "\n";
    };

    // Ожидаем: при Δ=0 и mu_prev=0 формула даёт Δ_next = stimulus
    for (size_t i = 0; i < out.state.Delta.size(); ++i)
    {
        const double expected = E.stimulus[i];
        const double actual   = out.state.Delta[i];

        if (actual != expected)
        {
            std::cout << "ERROR: Δ[" << i << "] mismatch: "
                      << "expected=" << expected << " actual=" << actual << "\n";
            dump();
            return 1;
        }
    }

    std::cout << "Algorithmic Δ update OK.\n";
    return 0;
}
