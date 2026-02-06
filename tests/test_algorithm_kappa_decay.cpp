#include "fmrt_api.hpp"
using namespace fmrt;

int test_algorithm_kappa_decay()
{
    StructuralState X{};
    X.reset();       // Kappa = 1
    X.Phi = 10;      // создаём нагрузку
    X.M   = 5;

    StructEvent e{};
    e.type = EventType::Update;
    e.dt   = 1.0;
    e.stimulus = {5,5,5,5};   // сильный стимул

    StateEnvelope out = FMRT_Step(X, e);

    // κ должна уменьшиться, но НЕ обязана упасть в 0
    if (out.state.Kappa >= X.Kappa)
        return 1; // FAIL — decay не произошло

    // κ должна оставаться > 0 при нормальной нагрузке
    if (out.state.Kappa <= 0.0)
        return 1; // FAIL — ядро убило κ слишком рано

    return 0; // PASS
}
