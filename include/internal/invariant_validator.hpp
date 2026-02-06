#pragma once

#include "fmrt_state.hpp"
#include "fmrt_metrics.hpp"
#include "fmrt_invariants.hpp"
#include "fmrt_envelope.hpp"
#include "fmrt_types.hpp"
#include "fmrt_errors.hpp"

namespace fmrt
{
    class InvariantValidator
    {
    public:
        bool validate(
            const StructuralState& X_current,
            const StructuralState& X_next,
            const DerivedMetrics& metrics,
            StateEnvelope& out_env
        ) const noexcept;

    private:

        bool checkMemory(
            const StructuralState& X_cur,
            const StructuralState& X_next,
            InvariantStatus& st
        ) const noexcept;

        bool checkKappa(
            const StructuralState& X_next,
            InvariantStatus& st
        ) const noexcept;

        bool checkMetric(
            const StructuralState& X_next,
            const DerivedMetrics& metrics,
            InvariantStatus& st
        ) const noexcept;

        bool checkTau(
            const StructuralState& X_next,
            const DerivedMetrics& metrics,
            InvariantStatus& st
        ) const noexcept;

        bool checkMorphology(
            const DerivedMetrics& metrics,
            InvariantStatus& st
        ) const noexcept;

        bool checkRegime(
            Regime prev,
            Regime next,
            InvariantStatus& st
        ) const noexcept;

        bool checkCollapse(
            const StructuralState& X_next,
            const DerivedMetrics& metrics,
            InvariantStatus& st
        ) const noexcept;

        bool checkForbidden(
            const StructuralState& X_next,
            const DerivedMetrics& metrics,
            InvariantStatus& st
        ) const noexcept;
    };

} // namespace fmrt
