#include <iostream>
#include "fmrt_api.hpp"

using namespace fmrt;

// ----------------------------------------------------------------------
// Test 8: collapse_after_stress_sequence
//
// Проверяется цепочка:
//   UPDATE (сильный стресс) → UPDATE (ещё сильнее) → GAP → HEARTBEAT → collapse
//
// Правила проверки:
//   - Пока κ>0 → collapse быть НЕ должно
//   - Когда κ==0 → collapse ОБЯЗАН быть
//   - Геометрия collapse должна быть строго корректной
// ----------------------------------------------------------------------

int test_collapse_after_stress_sequence()
{
    StructuralState X{};
    X.reset();

    // Стартуем с умеренно низкой каппы
    X.Kappa = 0.2;
    X.Phi   = 1.0;
    X.M     = 20.0;
    X.Delta = {1.0, 1.0, 1.0, 1.0};

    // ---------------------------------------------------------------
    // STEP 1 — сильный UPDATE
    // ---------------------------------------------------------------
    {
        StructEvent e{};
        e.type = EventType::Update;
        e.dt   = 1.0;
        e.stimulus = {30.0, -25.0, 20.0, -18.0};

        StateEnvelope out = FMRT_Step(X, e);

        if (out.state.Kappa > 0.0 && out.metrics.is_collapse)
        {
            std::cerr << "collapse_after_stress_sequence: early collapse (step1)\n";
            return 1;
        }

        X = out.state;
    }

    // ---------------------------------------------------------------
    // STEP 2 — ещё один стресс UPDATE
    // ---------------------------------------------------------------
    {
        StructEvent e{};
        e.type = EventType::Update;
        e.dt   = 1.0;
        e.stimulus = {40.0, -35.0, 28.0, -22.0};

        StateEnvelope out = FMRT_Step(X, e);

        if (out.state.Kappa > 0.0 && out.metrics.is_collapse)
        {
            std::cerr << "collapse_after_stress_sequence: early collapse (step2)\n";
            return 1;
        }

        X = out.state;
    }

    // ---------------------------------------------------------------
    // STEP 3 — GAP
    // ---------------------------------------------------------------
    {
        StructEvent e{};
        e.type = EventType::Gap;
        e.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, e);

        if (out.state.Kappa > 0.0 && out.metrics.is_collapse)
        {
            std::cerr << "collapse_after_stress_sequence: early collapse (gap)\n";
            return 1;
        }

        X = out.state;
    }

    // ---------------------------------------------------------------
    // STEP 4 — HEARTBEAT → здесь должен произойти collapse
    // ---------------------------------------------------------------
    {
        StructEvent e{};
        e.type = EventType::Heartbeat;
        e.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, e);

        if (out.state.Kappa > 0.0)
        {
            std::cerr << "collapse_after_stress_sequence: κ>0 but no collapse\n";
            return 1;
        }

        // κ == 0 → collapse обязательно
        if (!out.metrics.is_collapse)
        {
            std::cerr << "collapse_after_stress_sequence: collapse flag missing\n";
            return 1;
        }

        // Строгая геометрия collapse
        if (out.metrics.det_g != 0.0)
        {
            std::cerr << "collapse_after_stress_sequence: det_g must be 0\n";
            return 1;
        }

        if (out.metrics.tau != 0.0)
        {
            std::cerr << "collapse_after_stress_sequence: tau must be 0\n";
            return 1;
        }

        if (out.metrics.mu != 1.0)
        {
            std::cerr << "collapse_after_stress_sequence: mu must be 1\n";
            return 1;
        }

        if (out.metrics.regime != Regime::COL)
        {
            std::cerr << "collapse_after_stress_sequence: regime != COL\n";
            return 1;
        }
    }

    return 0;
}
