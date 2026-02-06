#include <iostream>
#include "fmrt_api.hpp"

using namespace fmrt;

// --------------------------------------------------------------
// Test 6: collapse_under_gap
//
// Цель:
//   Проверить, что GAP способен вызвать collapse,
//   если κ уже очень малая, и формула decay доводит её до нуля.
//
// Логика:
//   - κ = 1e-8 (живая, но почти мёртвая структура)
//   - M большая, чтобы curvature R была значимой
//   - event = GAP (stimulus=0)
//   - Пока κ > 0 → collapse НЕ должен наступить
//   - Когда κ == 0 → collapse должен наступить немедленно
// --------------------------------------------------------------

int test_collapse_under_gap()
{
    StructuralState X{};
    X.reset();

    X.Kappa = 1e-8;     // очень низкая, но живая
    X.Phi   = 0.0;
    X.M     = 100.0;    // высокая память усиливает decay через R
    X.Delta = {1.0, -1.0, 1.0, -1.0};

    StructEvent e{};
    e.type = EventType::Gap;
    e.dt   = 1.0;

    bool collapse_seen = false;

    for (int step = 0; step < 50; ++step)
    {
        StateEnvelope out = FMRT_Step(X, e);

        if (out.state.Kappa > 0.0)
        {
            // Пока организм жив — collapse НЕ должен срабатывать
            if (out.metrics.is_collapse)
            {
                std::cerr << "collapse_under_gap: collapse triggered too early (κ>0)\n";
                return 1;
            }
        }
        else
        {
            // κ == 0 → collapse должен быть активирован
            if (!out.metrics.is_collapse)
            {
                std::cerr << "collapse_under_gap: collapse flag missing at κ==0\n";
                return 1;
            }

            // Строгая геометрия collapse
            if (out.metrics.det_g != 0.0)
            {
                std::cerr << "collapse_under_gap: det_g must be 0 at collapse\n";
                return 1;
            }

            if (out.metrics.tau != 0.0)
            {
                std::cerr << "collapse_under_gap: tau must be 0 at collapse\n";
                return 1;
            }

            if (out.metrics.mu != 1.0)
            {
                std::cerr << "collapse_under_gap: mu must be 1 at collapse\n";
                return 1;
            }

            if (out.metrics.regime != Regime::COL)
            {
                std::cerr << "collapse_under_gap: regime must be COL at collapse\n";
                return 1;
            }

            collapse_seen = true;
            break;
        }

        X = out.state;
    }

    if (!collapse_seen)
    {
        std::cerr << "collapse_under_gap: collapse did not occur\n";
        return 1;
    }

    return 0; // OK
}
