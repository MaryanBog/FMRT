#include <iostream>
#include <vector>
#include "fmrt_api.hpp"

using namespace fmrt;

struct Snap
{
    StructuralState s;
    DerivedMetrics  m;
    InvariantStatus inv;
    StepStatus      st;
    ErrorCategory   err;
};

static bool same(const Snap& a, const Snap& b)
{
    // ---- state ----
    for (size_t i = 0; i < DELTA_DIM; i++)
        if (a.s.Delta[i] != b.s.Delta[i]) return false;

    if (a.s.Phi != b.s.Phi ||
        a.s.M   != b.s.M   ||
        a.s.Kappa != b.s.Kappa ||
        a.s.RegimePrev != b.s.RegimePrev)
        return false;

    // ---- metrics ----
    if (a.m.curvature_R != b.m.curvature_R ||
        a.m.det_g       != b.m.det_g       ||
        a.m.tau         != b.m.tau         ||
        a.m.mu          != b.m.mu          ||
        a.m.morph_class != b.m.morph_class ||
        a.m.regime      != b.m.regime      ||
        a.m.is_collapse != b.m.is_collapse ||
        a.m.collapse_distance != b.m.collapse_distance ||
        a.m.collapse_speed    != b.m.collapse_speed    ||
        a.m.collapse_intensity!= b.m.collapse_intensity)
        return false;

    // ---- invariants ----
    if (a.inv.flags != b.inv.flags ||
        a.inv.all_ok != b.inv.all_ok)
        return false;

    // ---- diagnostics ----
    if (a.st != b.st || a.err != b.err)
        return false;

    return true;
}

int test_determinism_no_hidden_state()
{
    std::cout << "Running determinism_no_hidden_state...\n";

    // ------------------ Исходное состояние ------------------
    StructuralState X0{};
    X0.reset();
    X0.Phi   = 0.33;
    X0.M     = 1.5;
    X0.Kappa = 1.0;
    X0.Delta = {0.2, -0.1, 0.05, -0.02};

    StructEvent E{};
    E.type     = EventType::Update;
    E.dt       = 0.07;
    E.stimulus = {1.0, 0.5, -0.3, 2.2};

    // ------------------ Instance A ------------------
    std::vector<Snap> A;
    {
        StructuralState XA = X0;

        for (int i = 0; i < 200; i++)
        {
            StateEnvelope out = FMRT_Step(XA, E);

            Snap s{};
            s.s   = out.state;
            s.m   = out.metrics;
            s.inv = out.invariants;
            s.st  = out.status;
            s.err = out.error_category;

            A.push_back(s);
            XA = out.state;
        }
    }

    // ------------------ Instance B ------------------
    std::vector<Snap> B;
    {
        StructuralState XB = X0;

        for (int i = 0; i < 200; i++)
        {
            StateEnvelope out = FMRT_Step(XB, E);

            Snap s{};
            s.s   = out.state;
            s.m   = out.metrics;
            s.inv = out.invariants;
            s.st  = out.status;
            s.err = out.error_category;

            B.push_back(s);
            XB = out.state;
        }
    }

    // ------------------ Сравнение ------------------
    if (A.size() != B.size())
    {
        std::cerr << "determinism_no_hidden_state FAILED: size mismatch\n";
        return 1;
    }

    for (size_t i = 0; i < A.size(); i++)
    {
        if (!same(A[i], B[i]))
        {
            std::cerr << "determinism_no_hidden_state FAILED: mismatch at step " << i << "\n";
            return 1;
        }
    }

    std::cout << "determinism_no_hidden_state OK\n";
    return 0;
}
