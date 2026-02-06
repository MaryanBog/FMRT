#include <iostream>
#include <limits>
#include "fmrt_api.hpp"
#include "fmrt_constants.hpp"   // для EPS_KAPPA

using namespace fmrt;

int test_numeric_underflow_overflow_safety()
{
    std::cout << "Running numeric_underflow_overflow_safety...\n";

    // --------- СЦЕНАРИЙ 1: ОГРОМНЫЕ ЗНАЧЕНИЯ (overflow-риск) ---------
    {
        StructuralState X{};
        X.reset();

        X.Delta = {1e50, -1e50, 1e50, -1e50};
        X.Phi   = 1e80;
        X.M     = 1e100;
        X.Kappa = 1.0;

        StructEvent E{};
        E.type     = EventType::Update;
        E.dt       = 1.0;
        E.stimulus = {1e50, 1e50, 1e50, 1e50};

        StateEnvelope out = FMRT_Step(X, E);

        // 1) Выход обязан быть finite
        if (!out.isFinite())
        {
            std::cerr << "numeric_underflow_overflow_safety FAILED: overflow produced NaN/Inf\n";
            return 1;
        }

        if (out.status == StepStatus::OK)
        {
            // живой организм
            if (out.state.Kappa > EPS_KAPPA)
            {
                if (out.state.Kappa < 0.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: κ < 0 after overflow scenario\n";
                    return 1;
                }
                if (out.metrics.det_g <= 0.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: det_g <= 0 for live state in overflow scenario\n";
                    return 1;
                }
                if (out.metrics.tau <= 0.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: tau <= 0 for live state in overflow scenario\n";
                    return 1;
                }
                if (out.metrics.mu < 0.0 || out.metrics.mu > 1.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: mu out of [0,1] for live state in overflow scenario\n";
                    return 1;
                }
            }
            else
            {
                // коллапс допускается — проверяем геометрию коллапса
                if (out.metrics.det_g != 0.0 ||
                    out.metrics.tau   != 0.0 ||
                    out.metrics.mu    != 1.0 ||
                    out.metrics.regime != Regime::COL)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: collapse geometry invalid in overflow scenario\n";
                    return 1;
                }
            }
        }
        else
        {
            // шаг отвергнут — тоже ок, но без мусора
            if (out.error_category != ErrorCategory::NumericError &&
                out.error_category != ErrorCategory::InvariantViolation)
            {
                std::cerr << "numeric_underflow_overflow_safety FAILED: overflow scenario wrong error category\n";
                return 1;
            }
            if (!out.isFinite())
            {
                std::cerr << "numeric_underflow_overflow_safety FAILED: overflow scenario non-finite envelope on error\n";
                return 1;
            }
        }
    }

    // --------- СЦЕНАРИЙ 2: ОЧЕНЬ МАЛЫЕ, НО НЕ DENORMAL (underflow-риск) ---------
    {
        StructuralState X{};
        X.reset();

        const double small = 1e-200; // нормальное число, не denormal

        X.Delta = {small, -small, small, -small};
        X.Phi   = small;
        X.M     = small;
        X.Kappa = 1.0;

        StructEvent E{};
        E.type     = EventType::Update;
        E.dt       = 1.0;
        E.stimulus = {small, small, small, small};

        StateEnvelope out = FMRT_Step(X, E);

        if (!out.isFinite())
        {
            std::cerr << "numeric_underflow_overflow_safety FAILED: underflow produced NaN/Inf\n";
            return 1;
        }

        if (out.status == StepStatus::OK)
        {
            if (out.state.Kappa > EPS_KAPPA)
            {
                if (out.state.Kappa < 0.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: κ < 0 after underflow scenario\n";
                    return 1;
                }
                if (out.metrics.det_g <= 0.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: det_g <= 0 after underflow scenario\n";
                    return 1;
                }
                if (out.metrics.tau <= 0.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: tau <= 0 after underflow scenario\n";
                    return 1;
                }
                if (out.metrics.mu < 0.0 || out.metrics.mu > 1.0)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: mu out of [0,1] after underflow scenario\n";
                    return 1;
                }
            }
            else
            {
                // если вдруг коллапс произошёл на малых числах — всё равно проверяем геометрию
                if (out.metrics.det_g != 0.0 ||
                    out.metrics.tau   != 0.0 ||
                    out.metrics.mu    != 1.0 ||
                    out.metrics.regime != Regime::COL)
                {
                    std::cerr << "numeric_underflow_overflow_safety FAILED: collapse geometry invalid in underflow scenario\n";
                    return 1;
                }
            }
        }
        else
        {
            if (out.error_category != ErrorCategory::NumericError &&
                out.error_category != ErrorCategory::InvariantViolation)
            {
                std::cerr << "numeric_underflow_overflow_safety FAILED: underflow scenario wrong error category\n";
                return 1;
            }
            if (!out.isFinite())
            {
                std::cerr << "numeric_underflow_overflow_safety FAILED: underflow scenario non-finite envelope on error\n";
                return 1;
            }
        }
    }

    std::cout << "numeric_underflow_overflow_safety OK\n";
    return 0;
}
