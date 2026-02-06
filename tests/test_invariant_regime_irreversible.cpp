#include <iostream>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_regime_irreversible()
{
    auto dump = [&](const StateEnvelope& out, const char* label) {
        std::cout << "--- REGIME DUMP (" << label << ") ---\n";
        std::cout << "status      = " << (int)out.status << "\n";
        std::cout << "error cat   = " << (int)out.error_category << "\n";
        std::cout << "regime      = " << (int)out.metrics.regime << "\n";
        std::cout << "kappa       = " << out.state.Kappa << "\n";
        std::cout << "is_collapse = " << out.metrics.is_collapse << "\n";
    };

    // ---------- Case 1: нормальный переход вперёд (ACC -> что-то >= ACC), step принят
    {
        StructuralState X;
        X.Delta = {10, -10, 5, -5};
        X.Phi   = 20.0;
        X.M     = 1.0;
        X.Kappa = 1.0;
        X.RegimePrev = Regime::ACC;

        StructEvent E;
        E.type = EventType::Update;
        E.dt   = 1.0;
        E.stimulus = {5, -5, 3, -3};

        StateEnvelope out = FMRT_Step(X, E);

        if (out.status != StepStatus::OK) {
            std::cout << "Case 1: step must be accepted\n";
            dump(out, "case1");
            return 1;
        }

        // режим не должен быть меньше ACC
        if ((int)out.metrics.regime < (int)Regime::ACC) {
            std::cout << "Case 1: regime decreased below ACC\n";
            dump(out, "case1");
            return 1;
        }
    }

    // ---------- Case 2: prev = REL, слабый heartbeat → движок хочет откатить до ACC,
    // но инвариант ДОЛЖЕН это ЗАПРЕТИТЬ → шаг отклонён (ERROR + InvariantViolation),
    // а состояние X(t) сохраняется.
    {
        StructuralState X;
        X.Delta = {0.1, 0.1, 0.1, 0.1};
        X.Phi   = 0.1;
        X.M     = 0.0;
        X.Kappa = 1.0;
        X.RegimePrev = Regime::REL; // предыдущий режим = 2

        StructEvent E;
        E.type = EventType::Heartbeat;
        E.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, E);

        // ОЖИДАЕМ НЕ УСПЕШНЫЙ ШАГ, А ИМЕННО ОТКЛОНЕНИЕ ИНВАРИАНТОМ
        if (out.status != StepStatus::ERROR ||
            out.error_category != ErrorCategory::InvariantViolation)
        {
            std::cout << "Case 2: expected invariant rejection (ERROR + InvariantViolation)\n";
            dump(out, "case2");
            return 1;
        }

        // В этом режиме out.metrics.regime может быть каким угодно (мы возвращаем
        // пустые metrics при ошибке), главное — шаг отклонён.
    }

    // ---------- Case 3: κ = 0, RegimePrev = COL — режим не должен выходить из COL
    {
        StructuralState X;
        X.Delta = {1, 1, 1, 1};
        X.Phi   = 5.0;
        X.M     = 2.0;
        X.Kappa = 0.0;              // уже коллапс
        X.RegimePrev = Regime::COL; // история = COL

        StructEvent E;
        E.type = EventType::Heartbeat;
        E.dt   = 1.0;

        StateEnvelope out = FMRT_Step(X, E);

        // Шаг для коллапсированной структуры считается "валидным",
        // но режим обязан остаться COL.
        if (out.status != StepStatus::OK) {
            std::cout << "Case 3: collapsed step should still be OK\n";
            dump(out, "case3");
            return 1;
        }

        if (out.metrics.regime != Regime::COL) {
            std::cout << "Case 3: collapsed regime left COL\n";
            dump(out, "case3");
            return 1;
        }

        if (out.state.Kappa != 0.0) {
            std::cout << "Case 3: kappa must stay 0 in collapse\n";
            dump(out, "case3");
            return 1;
        }
    }

    std::cout << "Regime irreversibility invariant OK.\n";
    return 0;
}
