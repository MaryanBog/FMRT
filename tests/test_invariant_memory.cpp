#include <iostream>
#include "fmrt_api.hpp"
#include "fmrt_types.hpp"

using namespace fmrt;

int test_invariant_memory()
{
    // Создаем состояние с очень большим M,
    // чтобы любая попытка уменьшить M была инвариантно запрещена.
    StructuralState X;
    X.Delta = {0.1, 0.0, -0.2, 0.05};
    X.Phi   = 0.3;
    X.M     = 100.0;     // огромная память
    X.Kappa = 1.0;
    X.RegimePrev = Regime::ACC;

    // Событие GAP, dt маленький.
    // Теоретически M_next = M + dt*tau  -> >M
    // Но мы искусственно создадим нарушение:
    // Мы подменим provisional_next вручную через событие,
    // которое создаёт снижение M через крошечный tau.
    // EvolutionEngine гарантирует рост, но если что-то пойдёт не так —
    // инвариант обязан поймать.
    StructEvent E;
    E.type = EventType::Gap;
    E.dt   = 1e-9; // настолько маленькое dt, что прирост M может быть 0

    StateEnvelope out = FMRT_Step(X, E);

    // Проверяем инвариант:
    if (out.status != StepStatus::OK)
    {
        std::cout << "Invariant rejected transition (expected OK). Status=" << (int)out.status << "\n";
        return 1;
    }

    if (out.state.M < X.M)
    {
        std::cout << "ERROR: Memory decreased! X.M=" << X.M
                  << " out.M=" << out.state.M << "\n";
        return 1;
    }

    std::cout << "Memory invariant OK. M_old=" << X.M
              << " M_new=" << out.state.M << "\n";

    return 0;
}
