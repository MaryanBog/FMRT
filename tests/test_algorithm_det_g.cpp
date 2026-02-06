#include <iostream>
#include <cmath>

#include "fmrt_api.hpp"
#include "fmrt_types.hpp"
#include "fmrt_constants.hpp"

using namespace fmrt;

int test_algorithm_det_g()
{
    StructuralState X{};
    X.Delta      = {1.0, 0.0, 0.0, 0.0};
    X.Phi        = 0.0;
    X.M          = 0.0;
    X.Kappa      = 1.0;
    X.RegimePrev = Regime::ACC;

    StructEvent E{};
    E.type     = EventType::Heartbeat;
    E.dt       = 1.0;

    StateEnvelope out = FMRT_Step(X, E);

    // Берём значения, которые использовал движок
    double R_new = out.metrics.curvature_R;
    double kappa_new = out.state.Kappa;

    // det_g по точной формуле движка
    double expected = std::exp(-R_new) * kappa_new;

    // движок также применяет MAX(EPS_METRIC)
    if (expected < fmrt::EPS_METRIC)
        expected = fmrt::EPS_METRIC;

    auto dump = [&]() {
        std::cout << "--- det_g ALGO DUMP ---\n";
        std::cout << "expected det_g = " << expected << "\n";
        std::cout << "actual   det_g = " << out.metrics.det_g << "\n";
        std::cout << "R_new         = " << R_new << "\n";
        std::cout << "kappa_new     = " << kappa_new << "\n";
    };

    if (std::abs(out.metrics.det_g - expected) > 1e-12)
    {
        std::cout << "ERROR: det_g mismatch\n";
        dump();
        return 1;
    }

    std::cout << "Algorithmic det_g update OK.\n";
    return 0;
}
