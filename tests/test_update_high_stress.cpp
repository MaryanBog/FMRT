#include "fmrt_api.hpp"

using namespace fmrt;

int test_update_high_stress()
{
    StructuralState X{};
    X.reset();

    // предзагружаем сильное натяжение и память
    X.Phi   = 50.0;
    X.M     = 100.0;
    X.Kappa = 0.5;

    // большая деформация → огромная кривизна R
    X.Delta = {50.0, -50.0, 50.0, -50.0};

    StructEvent E{};
    E.type = EventType::Update;
    E.dt   = 1.0;
    E.stimulus = {50.0, 50.0, 50.0, 50.0}; // экстремальный удар

    StateEnvelope out = FMRT_Step(X, E);

    // в этом сценарии ДОЛЖЕН быть коллапс
    if (out.state.Kappa != 0.0)
        return 1;

    if (out.metrics.regime != Regime::COL)
        return 1;

    if (!out.metrics.is_collapse)
        return 1;

    return 0;
}
