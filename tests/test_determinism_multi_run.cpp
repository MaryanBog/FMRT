#include <iostream>
#include <vector>
#include "fmrt_api.hpp"

using namespace fmrt;

struct Snapshot
{
    StructuralState s;
    DerivedMetrics  m;
    InvariantStatus inv;
    StepStatus      st;
    ErrorCategory   err;
};

static void push_snapshot(std::vector<Snapshot>& v, const StateEnvelope& e)
{
    Snapshot snap{};
    snap.s   = e.state;
    snap.m   = e.metrics;
    snap.inv = e.invariants;
    snap.st  = e.status;
    snap.err = e.error_category;
    v.push_back(snap);
}

static bool compare_snap(const Snapshot& a, const Snapshot& b)
{
    // state
    for (size_t k = 0; k < DELTA_DIM; k++)
        if (a.s.Delta[k] != b.s.Delta[k]) return false;

    if (a.s.Phi != b.s.Phi ||
        a.s.M   != b.s.M   ||
        a.s.Kappa != b.s.Kappa ||
        a.s.RegimePrev != b.s.RegimePrev)
        return false;

    // metrics
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

    // invariants
    if (a.inv.flags  != b.inv.flags ||
        a.inv.all_ok != b.inv.all_ok)
        return false;

    // diag
    if (a.st != b.st || a.err != b.err)
        return false;

    return true;
}

int test_determinism_multi_run()
{
    std::cout << "Running determinism_multi_run...\n";

    // -------------------- Инициализация --------------------
    StructuralState X0{};
    X0.reset();
    X0.Phi = 0.2;
    X0.M   = 0.5;
    X0.Kappa = 1.0;
    X0.Delta = {0.01, -0.02, 0.03, -0.04};

    StructEvent E{};
    E.type = EventType::Update;
    E.dt   = 0.05;
    E.stimulus = {0.5, 1.0, -1.0, 0.25};

    // -------------------- Первый прогон --------------------
    std::vector<Snapshot> run1;
    StructuralState X = X0;

    for (int i = 0; i < 200; i++)
    {
        StateEnvelope out = FMRT_Step(X, E);
        push_snapshot(run1, out);
        X = out.state;
    }

    // -------------------- Второй прогон --------------------
    std::vector<Snapshot> run2;
    X = X0;

    for (int i = 0; i < 200; i++)
    {
        StateEnvelope out = FMRT_Step(X, E);
        push_snapshot(run2, out);
        X = out.state;
    }

    // -------------------- Сравнение --------------------
    if (run1.size() != run2.size())
    {
        std::cerr << "determinism_multi_run FAILED: snapshot size mismatch\n";
        return 1;
    }

    for (size_t i = 0; i < run1.size(); i++)
    {
        if (!compare_snap(run1[i], run2[i]))
        {
            std::cerr << "determinism_multi_run FAILED: mismatch at step " << i << "\n";
            return 1;
        }
    }

    std::cout << "determinism_multi_run OK\n";
    return 0;
}
