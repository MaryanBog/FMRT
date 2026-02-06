#include <iostream>
#include <cassert>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_reset()
{
    StructuralState X;
    X.Delta = {0.2, -0.1, 0.3, 0.05};
    X.Phi   = 0.7;
    X.M     = 5.0;
    X.Kappa = 0.42;
    X.RegimePrev = Regime::REL;

    StructEvent E;
    E.type = EventType::Reset;
    E.dt   = 0.0;

    StateEnvelope out = FMRT_Step(X, E);

    auto dump = [&]()
    {
        std::cout << "STATE AFTER RESET:\n";
        for (std::size_t i = 0; i < DELTA_DIM; ++i)
            std::cout << "Delta[" << i << "] = " << out.state.Delta[i] << "\n";

        std::cout << "Phi        = " << out.state.Phi << "\n";
        std::cout << "M          = " << out.state.M << "\n";
        std::cout << "Kappa      = " << out.state.Kappa << "\n";
        std::cout << "RegimePrev = " << (int)out.state.RegimePrev << "\n";

        std::cout << "--- METRICS ---\n";
        std::cout << "curvature_R = " << out.metrics.curvature_R << "\n";
        std::cout << "det_g       = " << out.metrics.det_g << "\n";
        std::cout << "tau         = " << out.metrics.tau << "\n";
        std::cout << "mu          = " << out.metrics.mu << "\n";
        std::cout << "regime      = " << (int)out.metrics.regime << "\n";
        std::cout << "is_collapse = " << out.metrics.is_collapse << "\n";
    };

    if (out.status != StepStatus::OK) { dump(); return 1; }
    if (out.event_type != EventType::Reset) { dump(); return 1; }

    for (std::size_t i = 0; i < DELTA_DIM; ++i)
        if (out.state.Delta[i] != 0.0) { dump(); return 1; }

    if (out.state.Phi != 0.0) { dump(); return 1; }
    if (out.state.M != 0.0) { dump(); return 1; }
    if (out.state.Kappa != 1.0) { dump(); return 1; }

    if (out.state.RegimePrev != Regime::ACC) { dump(); return 1; }

    if (out.metrics.curvature_R != 0.0) { dump(); return 1; }
    if (out.metrics.mu != 0.0) { dump(); return 1; }
    if (out.metrics.morph_class != MorphologyClass::Elastic) { dump(); return 1; }
    if (out.metrics.regime != Regime::ACC) { dump(); return 1; }

    if (!(out.metrics.det_g > 0.0)) { dump(); return 1; }
    if (out.metrics.tau <= 0.0) { dump(); return 1; }
    if (out.metrics.is_collapse != false) { dump(); return 1; }

    return 0;
}
