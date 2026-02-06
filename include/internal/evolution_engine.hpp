#pragma once

#include "fmrt_state.hpp"
#include "fmrt_event.hpp"
#include "fmrt_metrics.hpp"
#include "fmrt_envelope.hpp"
#include "fmrt_constants.hpp"

namespace fmrt
{
    class EvolutionEngine
    {
    public:
        void evolve(
            const StructuralState& X_current,
            const StructEvent&    E,
            StructuralState&      next_state,
            DerivedMetrics&       metrics
        ) const noexcept;

    private:

        // === CORE UPDATE RULES (FMT 3.1) ====================================

        void updateDelta(
            const StructuralState& X,
            const StructEvent&    E,
            double                mu,
            StructuralState&      out
        ) const noexcept;

        void updatePhi(
            const StructuralState& X,
            const StructEvent&    E,
            const StructuralState& X_next,
            StructuralState&      out
        ) const noexcept;

        void updateMemory(
            const StructuralState& X,
            double                 tau,
            const StructEvent&     E,
            StructuralState&       out
        ) const noexcept;

        void updateKappa(
            const StructuralState& X,
            double                 R,
            double                 mu,
            const StructEvent&     E,
            StructuralState&       out
        ) const noexcept;

        // === METRICS ========================================================

        double computeCurvature(const StructuralState& X) const noexcept;
        double computeDetG(double R, double kappa) const noexcept;
        double computeTau(double kappa) const noexcept;
        double computeMu(double curvature_R) const noexcept;
        MorphologyClass classifyMorphology(double mu) const noexcept;
        Regime computeRegime(Regime prev, MorphologyClass mc, double kappa) const noexcept;

        void processCollapse(StructuralState& X, DerivedMetrics& M) const noexcept;
    };
}
