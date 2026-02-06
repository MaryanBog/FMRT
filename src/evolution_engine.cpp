#include "internal/evolution_engine.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace fmrt
{

// ============================================================================
// evolve — Main FMT 3.1 / FMRT V2.2 update step
// ============================================================================
void EvolutionEngine::evolve(
    const StructuralState& X,
    const StructEvent&    E,
    StructuralState&      out,
    DerivedMetrics&       M
) const noexcept
{
    out = X;     // start from current state
    M   = {};    // clear metrics

    // RESET ================================================================
    if (E.type == EventType::Reset)
    {
        out.reset();
        M.curvature_R = 0.0;
        M.det_g       = METRIC_C1;
        M.tau         = TAU_MIN;
        M.mu          = 0.0;
        M.morph_class = MorphologyClass::Elastic;
        M.regime      = Regime::ACC;
        M.is_collapse = false;
        return;
    }

    // If collapsed — ignore everything except RESET
 // FINAL COLLAPSE OVERRIDE
if (out.Kappa <= EPS_KAPPA)
{
    processCollapse(out, M);
    out.RegimePrev = M.regime;   // <-- ДОБАВИТЬ
    return;
}

// NORMAL UPDATE
out.RegimePrev = M.regime;       // <-- ДОБАВИТЬ


    // === PRE-COMPUTE ======================================================
    const double R_prev  = computeCurvature(X);
    const double mu_prev = computeMu(R_prev);
    const double tau     = computeTau(X.Kappa);

    // === 1) Δ UPDATE ======================================================
    updateDelta(X, E, mu_prev, out);

    // === 2) Φ UPDATE ======================================================
    updatePhi(X, E, out, out);

    // === 3) M UPDATE ======================================================
    updateMemory(X, tau, E, out);

    // === 4) κ UPDATE ======================================================
    const double R_new  = computeCurvature(out);
    const double mu_new = computeMu(R_new);

    updateKappa(X, R_new, mu_new, E, out);

    // === METRICS ==========================================================
// === METRICS ==========================================================
M.curvature_R = R_new;
M.det_g       = computeDetG(R_new, out.Kappa);
M.tau         = computeTau(out.Kappa);
M.mu          = mu_new;
M.morph_class = classifyMorphology(mu_new);

// ENFORCE τ = 0 at collapse (Spec V2.2 strict rule)
if (out.Kappa <= EPS_KAPPA)
    M.tau = 0.0;

// === REGIME ============================================================
M.regime = computeRegime(
    computeRegime(Regime::ACC, classifyMorphology(mu_prev), X.Kappa),
    M.morph_class,
    out.Kappa
);

// FINAL COLLAPSE OVERRIDE
if (out.Kappa <= EPS_KAPPA)
    processCollapse(out, M);

}

// ============================================================================
// Δ update — FLEXION DIFFERENTIATION EQUATION (FDE)
// ============================================================================
void EvolutionEngine::updateDelta(
    const StructuralState& X,
    const StructEvent&    E,
    double                mu,
    StructuralState&      out
) const noexcept
{
    const double dt = E.dt;
    constexpr double MAX_DELTA = 10.0; // жёсткий предел деформации

    for (size_t i = 0; i < DELTA_DIM; ++i)
    {
        const double δ    = X.Delta[i];
        const double stim = (E.type == EventType::Update) ? E.stimulus[i] : 0.0;

        // Нормальная эволюция: стимул масштабируется по времени,
        // а не просто суммируется бесконечно.
        double next = δ + stim * dt - LAMBDA_RELAX * δ * dt;

        // Жёсткий клиппинг Δ, чтобы R не улетал в космос.
        if (next >  MAX_DELTA) next =  MAX_DELTA;
        if (next < -MAX_DELTA) next = -MAX_DELTA;

        out.Delta[i] = next;
    }
}


// ============================================================================
// Φ update — deformation-driven tension
// ============================================================================
void EvolutionEngine::updatePhi(
    const StructuralState& X,
    const StructEvent& E,
    const StructuralState& X_next,
    StructuralState& out
) const noexcept
{
    const double dt = E.dt;
    double deformation = 0.0;

    // Вычисляем модуль деформации Δ_next - Δ
    if (E.type == EventType::Update)
    {
        for (size_t i = 0; i < DELTA_DIM; ++i)
        {
            const double diff = X_next.Delta[i] - X.Delta[i];
            deformation += diff * diff;
        }
        deformation = std::sqrt(deformation);
    }

    // ЛОКАЛЬНАЯ ПЕРЕМЕННАЯ: было Φ → заменяем на Phi_local
    double Phi_local =
        X.Phi
        + TENSION_A * deformation
        - TENSION_B * dt;

    // Защита — Phi не может быть < 0
    out.Phi = (Phi_local < 0.0 ? 0.0 : Phi_local);
}


// ============================================================================
// M update — τ-weighted accumulation
// ============================================================================
void EvolutionEngine::updateMemory(
    const StructuralState& X,
    double                 tau,
    const StructEvent&     E,
    StructuralState&       out
) const noexcept
{
    if (E.type == EventType::Reset)
    {
        out.M = 0.0;
        return;
    }

    const double dt = E.dt;
    const double M_next = X.M + std::max(0.0, tau) * dt;

    out.M = (M_next < X.M ? X.M : M_next);
}

// ============================================================================
// κ update — viability decay equation
// ============================================================================
void EvolutionEngine::updateKappa(
    const StructuralState& X,
    double                 R,
    double                 mu,
    const StructEvent&     E,
    StructuralState&       out
) const noexcept
{
    if (E.type == EventType::Reset)
    {
        out.Kappa = RESET_KAPPA;
        return;
    }

    const double dt = E.dt;

double D = 0.0;

if (E.type == EventType::Update)
{
    D = DECAY_A1 * R
      + DECAY_A2 * X.Phi
      + DECAY_A3 * mu
      + DECAY_A4;
}
else
{
    D = DECAY_A4;
}

double κ = X.Kappa - dt * D;
out.Kappa = (κ < 0.0 ? 0.0 : κ);

}

// ============================================================================
// METRICS
// ============================================================================
double EvolutionEngine::computeCurvature(const StructuralState& X) const noexcept
{
    double norm2 = 0.0;
    for (double v : X.Delta) norm2 += v * v;

    const double denom = 1.0 + X.Kappa;
    const double mem   = X.M / denom;

    return CURV_A1 * norm2
         + CURV_A2 * X.Phi
         + CURV_A3 * mem;
}

double EvolutionEngine::computeDetG(double R, double kappa) const noexcept
{
    if (kappa <= 0.0) return 0.0;

    const double raw = METRIC_C1 * std::exp(-METRIC_C2 * R) * kappa;

    if (raw <= 0.0) return EPS_METRIC;
    return std::max(raw, EPS_METRIC);
}

double EvolutionEngine::computeTau(double kappa) const noexcept
{
    if (kappa <= 0.0) return 0.0;

    const double tau = TAU_MIN + TAU_SCALE * std::exp(-LAMBDA_K * kappa);
    return (tau < TAU_MIN ? TAU_MIN : tau);
}

double EvolutionEngine::computeMu(double R) const noexcept
{
    if (R <= 0.0) return 0.0;

    const double denom = R + MORPH_BETA;
    const double raw   = R / denom;

    if (denom <= EPS) return 0.0;
    return std::min(std::max(raw, 0.0), 1.0);
}

MorphologyClass EvolutionEngine::classifyMorphology(double mu) const noexcept
{
    if (mu < 0.25) return MorphologyClass::Elastic;
    if (mu < 0.50) return MorphologyClass::Plastic;
    if (mu < 0.75) return MorphologyClass::Degenerate;
    return MorphologyClass::NearCollapse;
}

Regime EvolutionEngine::computeRegime(
    Regime previous,
    MorphologyClass mc,
    double kappa
) const noexcept
{
    Regime candidate;

    if (kappa <= 0.0)
    {
        // настоящий коллапс только при κ == 0
        candidate = Regime::COL;
    }
    else
    {
        switch (mc)
        {
            case MorphologyClass::Elastic:
                candidate = Regime::ACC;
                break;
            case MorphologyClass::Plastic:
                candidate = Regime::DEV;
                break;
            case MorphologyClass::Degenerate:
            case MorphologyClass::NearCollapse:
                // при κ > 0 максимум REL, НИКОГДА НЕ COL
                candidate = Regime::REL;
                break;
        }
    }

    // запрет понижения режима
    if ((int)candidate < (int)previous)
        return previous;

    return candidate;
}



void EvolutionEngine::processCollapse(
    StructuralState& X,
    DerivedMetrics&  M
) const noexcept
{
    X.Kappa = 0.0;

    M.is_collapse = true;
    M.det_g       = 0.0;
    M.tau         = 0.0;
    M.mu          = 1.0;
    M.collapse_distance = 0.0;
    M.morph_class = MorphologyClass::NearCollapse;
    M.regime      = Regime::COL;
}

} // namespace fmrt
