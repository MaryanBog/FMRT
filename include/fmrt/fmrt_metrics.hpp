#pragma once
//
// FMRT Core V2.2
// fmrt_metrics.hpp
//
// Derived structural metrics computed by the Evolution Engine.
// These are NOT part of the structural state X(t), but are included
// in StateEnvelope(t+1) and used in invariant validation.
//

#include <cstddef>
#include <cstdint>

#include "fmrt_types.hpp"
#include "fmrt_config.hpp"

namespace fmrt
{
    struct DerivedMetrics
    {
        // ---------------------------------------------------------------------
        // Geometric / structural measurements
        // ---------------------------------------------------------------------
        double curvature_R = 0.0;   // scalar curvature
        double det_g = 0.0;         // metric determinant
        double tau = 0.0;           // temporal density τ
        double mu = 0.0;            // morphology index μ ∈ [0,1]

        // ---------------------------------------------------------------------
        // Classification
        // ---------------------------------------------------------------------
        MorphologyClass morph_class = MorphologyClass::Elastic;
        Regime regime = Regime::ACC;

        // ---------------------------------------------------------------------
        // Collapse indicators
        // ---------------------------------------------------------------------
        bool is_collapse = false;   // κ == 0
        double collapse_distance = 0.0;  // = κ
        double collapse_speed = 0.0;     // ||Δ_next - Δ|| / dt (computed earlier)
        double collapse_intensity = 0.0; // symbolic intensity from curvature

        // ---------------------------------------------------------------------
        // Validity / finiteness checks
        // ---------------------------------------------------------------------
        bool isFinite() const noexcept
        {
            return is_finite(curvature_R)
                && is_finite(det_g)
                && is_finite(tau)
                && is_finite(mu)
                && is_finite(collapse_distance)
                && is_finite(collapse_speed)
                && is_finite(collapse_intensity);
        }
    };

} // namespace fmrt
