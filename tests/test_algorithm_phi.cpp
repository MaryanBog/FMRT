#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"
#include "fmrt_constants.hpp"

using namespace fmrt;

int test_algorithm_phi()
{
    // Исходное состояние
    StructuralState X{};
    X.Delta      = {0.0, 0.0, 0.0, 0.0};
    X.Phi        = 2.0;
    X.M          = 0.0;
    X.Kappa      = 1.0;
    X.RegimePrev = Regime::ACC;

    // UPDATE со стимулом, создающим деформацию
    StructEvent E{};
    E.type     = EventType::Update;
    E.dt       = 1.0;
    E.stimulus = {3.0, 4.0, 0.0, 0.0};  
    // Деформация = sqrt(3^2 + 4^2) = 5

    StateEnvelope out = FMRT_Step(X, E);

    // Ожидаемое значение Φ_next по формуле:
    // Φ_next = 2.0 + 1.0 * 5 - 0.05 * 1.0 = 6.95
    double expected = 2.0 + 5.0 - 0.05;  // = 6.95
    double actual   = out.state.Phi;

    auto dump = [&]() {
        std::cout << "--- ALGO Φ DUMP ---\n";
        std::cout << "expected Φ = " << expected << "\n";
        std::cout << "actual Φ   = " << actual   << "\n";
        std::cout << "status     = " << (int)out.status << "\n";
    };

    // Проверяем точное соответствие
    if (std::abs(actual - expected) > 1e-12)
    {
        std::cout << "ERROR: Φ mismatch\n";
        dump();
        return 1;
    }

    std::cout << "Algorithmic Φ update OK.\n";
    return 0;
}
