#include <iostream>
#include "fmrt_api.hpp"

using namespace fmrt;

int test_determinism_single_run()
{
    std::cout << "Running determinism_single_run...\n";

    // Исходное состояние
    StructuralState X{};
    X.reset();
    X.Phi = 0.5;
    X.M   = 2.0;
    X.Kappa = 1.0;
    X.Delta = {0.1, -0.2, 0.3, -0.4};

    // Событие
    StructEvent E{};
    E.type     = EventType::Update;
    E.dt       = 0.1;
    E.stimulus = {1.0, 2.0, 3.0, 4.0};

    // Первый эталонный шаг
    StateEnvelope ref = FMRT_Step(X, E);

    // Проверяем 1000 повторов
    for (int i = 0; i < 1000; i++)
    {
        StateEnvelope out = FMRT_Step(X, E);

        // 1) Проверка статуса
        if (out.status != ref.status ||
            out.error_category != ref.error_category)
        {
            std::cerr << "determinism_single_run FAILED: status mismatch on iteration "
                      << i << "\n";
            return 1;
        }

        // 2) Проверка state
        for (size_t k = 0; k < DELTA_DIM; k++)
        {
            if (out.state.Delta[k] != ref.state.Delta[k])
            {
                std::cerr << "determinism_single_run FAILED: Delta mismatch at iteration "
                          << i << "\n";
                return 1;
            }
        }

        if (out.state.Phi   != ref.state.Phi   ||
            out.state.M     != ref.state.M     ||
            out.state.Kappa != ref.state.Kappa ||
            out.state.RegimePrev != ref.state.RegimePrev)
        {
            std::cerr << "determinism_single_run FAILED: state mismatch at iteration "
                      << i << "\n";
            return 1;
        }

        // 3) Проверка metrics
        if (out.metrics.curvature_R != ref.metrics.curvature_R ||
            out.metrics.det_g       != ref.metrics.det_g       ||
            out.metrics.tau         != ref.metrics.tau         ||
            out.metrics.mu          != ref.metrics.mu          ||
            out.metrics.morph_class != ref.metrics.morph_class ||
            out.metrics.regime      != ref.metrics.regime      ||
            out.metrics.is_collapse != ref.metrics.is_collapse ||
            out.metrics.collapse_distance != ref.metrics.collapse_distance ||
            out.metrics.collapse_speed    != ref.metrics.collapse_speed    ||
            out.metrics.collapse_intensity!= ref.metrics.collapse_intensity)
        {
            std::cerr << "determinism_single_run FAILED: metrics mismatch at iteration "
                      << i << "\n";
            return 1;
        }

        // 4) Проверка инвариантов
        if (out.invariants.flags  != ref.invariants.flags ||
            out.invariants.all_ok != ref.invariants.all_ok)
        {
            std::cerr << "determinism_single_run FAILED: invariant mismatch at iteration "
                      << i << "\n";
            return 1;
        }
    }

    std::cout << "determinism_single_run OK\n";
    return 0;
}
