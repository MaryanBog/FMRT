#include "fmrt_api.hpp"
using namespace fmrt;

// --------------------------------------------------------------
#include "fmrt_api.hpp"
using namespace fmrt;

int test_regime_ACC_to_DEV()
{
    StructuralState X{};
    X.reset();   // Kappa = 1, ACC

    StructEvent e{};
    e.type = EventType::Update;
    e.dt   = 1.0;

    // растём по стрессу, пока не увидим DEV
    for (int i = 0; i < 10; ++i)
    {
        double v = 1.0 + i * 2.0;   // 1, 3, 5, 7, ...
        e.stimulus = {v, v, v, v};

        StateEnvelope out = FMRT_Step(X, e);

        if (out.metrics.regime == Regime::DEV)
            return 0;   // PASS

        // если уже коллапснули раньше DEV → FAIL
        if (out.metrics.regime == Regime::COL)
            return 1;

        X = out.state;
    }

    // до DEV так и не дошли → FAIL
    return 1;
}

// --------------------------------------------------------------
int test_regime_DEV_to_REL()
{
    StructuralState X{};
    X.reset();

    // Создаем DEV
    X.Delta = {3, -3, 4, -4};
    X.Phi   = 5;
    X.Kappa = 1.0;

    StructEvent e{};
    e.type = EventType::Update;
    e.dt   = 1.0;

    // Увеличиваем стресс, чтобы morphology перешла в Degenerate
    for (int i = 0; i < 50; i++)
    {
        double v = 5.0 + i * 0.5;
        e.stimulus = {v, v, v, v};

        StateEnvelope out = FMRT_Step(X, e);

        if (out.metrics.regime == Regime::REL)
            return 0; // OK — перешли в REL под стрессом

        if (out.metrics.regime == Regime::COL)
            return 1; // FAIL — умерли раньше REL

        X = out.state;
    }

    return 1; // так и не достигли REL
}

// --------------------------------------------------------------
int test_regime_REL_to_COL()
{
    StructuralState X{};
    X.reset();
    X.RegimePrev = Regime::REL;
    X.Kappa = 1e-12;

    StructEvent e{};
    e.type = EventType::Update;
    e.dt = 1.0;
    e.stimulus = {10.0, 10.0, 10.0, 10.0};

    StateEnvelope out = FMRT_Step(X, e);

    if (out.metrics.regime != Regime::COL) return 1;
    if (out.state.Kappa != 0.0) return 1;
    if (out.metrics.det_g != 0.0) return 1;
    if (out.metrics.tau   != 0.0) return 1;
    if (out.metrics.mu    != 1.0) return 1;

    return 0;
}

// --------------------------------------------------------------
#include "fmrt_api.hpp"
using namespace fmrt;

int test_regime_cycle_ACC_DEV_REL()
{
    StructuralState X{};
    X.reset();                 // стартуем в ACC, κ=1

    StructEvent e{};
    e.type = EventType::Update;
    e.dt   = 1.0;

    bool seen_ACC = false;
    bool seen_DEV = false;
    bool seen_REL = false;

    for (int step = 0; step < 20; ++step)
    {
        double v = 1.0 + 2.0 * step;  // растущий стресс: 1,3,5,7,...
        e.stimulus = {v, v, v, v};

        StateEnvelope out = FMRT_Step(X, e);
        Regime r = out.metrics.regime;

        // фиксируем проход через режимы по порядку
        if (r == Regime::ACC)
        {
            seen_ACC = true;
        }
        else if (r == Regime::DEV)
        {
            if (!seen_ACC) return 1;  // DEV без ACC → ошибка
            seen_DEV = true;
        }
        else if (r == Regime::REL)
        {
            if (!seen_DEV) return 1;  // REL без DEV → ошибка
            seen_REL = true;
            return 0;                 // цикл ACC→DEV→REL успешно пройден
        }
        else if (r == Regime::COL)
        {
            // до REL не дошли, сразу коллапс → ошибка
            return 1;
        }

        X = out.state;
    }

    // не успели дойти до REL за 20 шагов
    return 1;
}

