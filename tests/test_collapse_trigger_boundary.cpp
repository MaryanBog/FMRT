#include <iostream>
#include "fmrt_api.hpp"

using namespace fmrt;

// --------------------------------------------------------------
// Test 5: collapse_trigger_boundary (κ → 0+)
// --------------------------------------------------------------
//
// Логика:
//  - стартуем с живой структуры (κ > 0)
//  - даём сильный стресс UPDATE, чтобы гарантированно прийти к κ = 0 за несколько шагов
//  - пока κ > 0 — флаг is_collapse ДОЛЖЕН быть false
//  - на ПЕРВОМ шаге, где κ == 0, ДОЛЖНЫ выполниться:
//      * is_collapse == true
//      * det_g == 0
//      * tau == 0
//      * mu == 1
//      * regime == COL
// --------------------------------------------------------------

int test_collapse_trigger_boundary()
{
    StructuralState X{};
    X.reset();

    // Стартуем с нормальной живой структуры
    X.Kappa = 1.0;
    X.Phi   = 0.0;
    X.M     = 0.0;

    StructEvent e{};
    e.type = EventType::Update;
    e.dt   = 1.0;

    // Достаточно сильный стимул, чтобы через несколько шагов привести к коллапсу
    e.stimulus = {10.0, 10.0, 10.0, 10.0};

    // Флаг: видели ли уже коллапс
    bool seen_collapse = false;

    for (int i = 0; i < 50; ++i)
    {
        StateEnvelope out = FMRT_Step(X, e);

        // 1) Пока κ > 0 → collapse НЕ должен быть активирован
        if (out.state.Kappa > 0.0)
        {
            if (out.metrics.is_collapse)
            {
                std::cerr << "collapse_trigger_boundary: is_collapse set while κ>0\n";
                return 1;
            }
        }
        else
        {
            // Первый шаг, когда κ == 0 → здесь должен наступить корректный collapse
            if (out.state.Kappa != 0.0)
            {
                std::cerr << "collapse_trigger_boundary: κ went negative without clamp to 0\n";
                return 1;
            }

            if (!out.metrics.is_collapse)
            {
                std::cerr << "collapse_trigger_boundary: collapse flag not set at κ==0\n";
                return 1;
            }

            if (out.metrics.det_g != 0.0)
            {
                std::cerr << "collapse_trigger_boundary: det_g must be 0 at collapse\n";
                return 1;
            }

            if (out.metrics.tau != 0.0)
            {
                std::cerr << "collapse_trigger_boundary: tau must be 0 at collapse\n";
                return 1;
            }

            if (out.metrics.mu != 1.0)
            {
                std::cerr << "collapse_trigger_boundary: mu must be 1 at collapse\n";
                return 1;
            }

            if (out.metrics.regime != Regime::COL)
            {
                std::cerr << "collapse_trigger_boundary: regime must be COL at collapse\n";
                return 1;
            }

            seen_collapse = true;
            break;
        }

        X = out.state;
    }

    if (!seen_collapse)
    {
        std::cerr << "collapse_trigger_boundary: no collapse reached within 50 steps\n";
        return 1;
    }

    return 0; // PASS
}
