#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"
#include "fmrt_constants.hpp"

using namespace fmrt;

int test_algorithm_tau()
{
    // Базовое состояние с κ > 0
    StructuralState X{};
    X.Delta      = {0.0, 0.0, 0.0, 0.0};
    X.Phi        = 0.0;
    X.M          = 0.0;
    X.Kappa      = 0.5;          // произвольное κ > 0
    X.RegimePrev = Regime::ACC;

    // HEARTBEAT с dt = 0, чтобы κ не изменился
StructEvent E{};
E.type     = EventType::Heartbeat;
E.dt       = 1e-9; // dt must be > 0, otherwise FMRT rejects event
E.stimulus = {0.0, 0.0, 0.0, 0.0};

    StateEnvelope out = FMRT_Step(X, E);

    // Берём κ, по которому реально считался τ
    double kappa_new = out.state.Kappa;

    // Ожидаемое τ по формуле ядра:
    // tau = TAU_MIN + TAU_SCALE * exp(-LAMBDA_K * κ)
    double expected = TAU_MIN + TAU_SCALE * std::exp(-LAMBDA_K * kappa_new);
    if (expected < TAU_MIN)
        expected = TAU_MIN;

    double actual = out.metrics.tau;

    auto dump = [&]() {
        std::cout << "--- ALGO τ DUMP ---\n";
        std::cout << "kappa_new   = " << kappa_new << "\n";
        std::cout << "expected τ  = " << expected << "\n";
        std::cout << "actual   τ  = " << actual   << "\n";
        std::cout << "status      = " << (int)out.status << "\n";
    };

    if (std::abs(actual - expected) > 1e-12)
    {
        std::cout << "ERROR: τ mismatch\n";
        dump();
        return 1;
    }

    std::cout << "Algorithmic τ update OK.\n";
    return 0;
}
