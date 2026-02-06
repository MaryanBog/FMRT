#pragma once
//
// FMRT Core V2.2
// fmrt_constants.hpp
//
// Global compile-time constants required by FMRT Core.
// All constants are deterministic, immutable and defined at compile time.
//

namespace fmrt
{
    // -------------------------------------------------------------------------
    // Fundamental numeric constants
    // -------------------------------------------------------------------------

    constexpr double EPS            = 1e-14;     // Safe epsilon for divisions
    constexpr double EPS_METRIC     = 1e-12;     // Minimum allowed det(g)
    constexpr double EPS_KAPPA      = 1e-12;     // Collapse threshold

    // -------------------------------------------------------------------------
    // Temporal density parameters (τ)
    // τ = tau_min + tau_scale * exp(-lambda_k * κ)
    // -------------------------------------------------------------------------

    constexpr double TAU_MIN        = 1e-6;
    constexpr double TAU_SCALE      = 1.0;
    constexpr double LAMBDA_K       = 1.0;

    // -------------------------------------------------------------------------
    // Relaxation / evolution parameters
    // -------------------------------------------------------------------------

    constexpr double LAMBDA_RELAX   = 0.1;

    // Tension update (Φ evolution)
    constexpr double TENSION_A      = 1.0;   // amplification of deformation
    constexpr double TENSION_B      = 0.05;  // continuous relaxation

    // -------------------------------------------------------------------------
    // Viability decay coefficients:
    // κ_next = κ - dt * (a1*R + a2*Φ + a3*μ + a4)
    // -------------------------------------------------------------------------

constexpr double DECAY_A1       = 0.002;  // было 0.1
constexpr double DECAY_A2       = 0.01;   // было 0.1
constexpr double DECAY_A3       = 0.02;   // было 0.1
constexpr double DECAY_A4       = 0.001;  // было 0.01
    // -------------------------------------------------------------------------
    // Curvature coefficients
    // R = α1*||Δ||^2 + α2*Φ + α3*M/(1+κ)
    // -------------------------------------------------------------------------

constexpr double CURV_A1 = 0.01;     // было 1.0 (100x меньше)
constexpr double CURV_A2 = 0.01;     // было 0.1
constexpr double CURV_A3 = 0.005;    // было 0.05

    // -------------------------------------------------------------------------
    // Metric determinant:
    // det_g = max(epsilon, c1 * exp(-c2 * R) * κ)
    // -------------------------------------------------------------------------

    constexpr double METRIC_C1      = 1.0;
    constexpr double METRIC_C2      = 1.0;

    // -------------------------------------------------------------------------
    // Morphology normalization
    // μ = curvature_R / (curvature_R + beta)
    // -------------------------------------------------------------------------

    constexpr double MORPH_BETA     = 1.0;

    // -------------------------------------------------------------------------
    // Reset defaults (STRUCT_RESET)
    // -------------------------------------------------------------------------

    constexpr double RESET_PHI      = 0.0;
    constexpr double RESET_KAPPA    = 1.0;

} // namespace fmrt
