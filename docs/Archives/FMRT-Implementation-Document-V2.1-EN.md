# FMRT-Implementation-Document-V2.1

---

# 0. Overview / Purpose

The FMRT Implementation Document defines the concrete technical plan for building the FMRT Core according to the FMRT Specification V2.1 and the FMRT Design Document V2.1.  
It provides detailed data structures, algorithms, processing rules, and API signatures needed to implement a deterministic, invariant-preserving, high-performance structural engine.

## 0.1 Purpose

This document serves to:

- Translate architecture (Design Document) into concrete implementation steps.
- Specify exact data structures for structural states, events, diagnostics, and envelopes.
- Define algorithms for updating Δ, Φ, M, κ and all derived structural metrics.
- Provide explicit computation rules for morphology, regime, curvature, temporal density, viability decay, and collapse indicators.
- Establish rules for numerical safety, handling NaN/Inf, and deterministic floating-point operations.
- Define the internal API between modules.
- Standardize behavior under GAP, HEARTBEAT, and RESET events.
- Describe implementation-level error handling and fault isolation.

FMRT Core must be implemented exactly as defined here, without deviation, to ensure deterministic reproducibility across all platforms.

## 0.2 Scope

This document covers:

- Module implementations: Tick Adapter, Event Sequencer, Core Engine, Invariant Checker, Diagnostics.
- Memory layout and data structure definitions.
- Event processing pipeline and function-level responsibilities.
- Deterministic algorithms for structural evolution.
- Implementation constraints for real-time, O(1)-per-event performance.
- Handling of exceptional conditions, failures, and structural collapse.
- API contracts for SDK integration.

What this document does **not** include:

- High-level architecture (covered in Design Document).  
- Theoretical reasoning or invariant derivations (covered in Specification Document).  
- Platform-specific optimizations (to be handled in deployment notes).  

## 0.3 Implementation Goals

- **Deterministic execution:** same input → same bitwise output across platforms.
- **Invariant preservation:** structural invariants must be enforced by design.
- **Numerical safety:** no NaN, Inf, or undefined behavior.
- **Memory safety:** no unbounded allocations, no hidden mutable globals.
- **Low-latency processing:** O(1) time per event, no dynamic allocations on the hot path.
- **Clear modular separation:** each module interacts only via defined interfaces.
- **Reproducibility:** implementation must allow identical simulation results across runs.

This section defines the execution philosophy and constraints that all further implementation details must follow.

---

# 1. Module Implementations

This section describes the concrete implementation plan for each FMRT Core module, including responsibilities, data structures, function definitions, logic, and constraints.

## 1.1 Tick Adapter — Implementation

Responsibilities:
- Convert raw market ticks into normalized structural events.
- Validate ticks (no NaN, no Inf, timestamps increasing, BID <= ASK).
- Apply minimal filtering only (reject impossible data, no smoothing).
- Generate four event types: STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET.
- Attach sequence ID.

Data structures:
- RawTick: bid, ask, volume, timestamp, source_id.
- StructEvent: type, price_delta, dt, sequence_id, reason (for RESET).

Core processing logic:
1. Compute dt = cur.timestamp – prev.timestamp.
2. If dt ≤ 0 → emit STRUCT_HEARTBEAT with minimal dt.
3. Compute price_delta from bid/ask.
4. If gap detected → emit STRUCT_GAP.
5. If external trigger → emit STRUCT_RESET.
6. Otherwise → emit STRUCT_UPDATE.

Tick Adapter must not smooth, average, or interpret market structure. Only transform ticks → structural events.

---

## 1.2 Event Sequencer — Implementation

Responsibilities:
- Ensure strict event ordering.
- Remove duplicates.
- Fix out-of-order arrivals.
- Assign unified sequence_id.

Data structure:
- OrderedEvent: StructEvent + final sequence_id.

Logic:
1. Maintain internal counter seq_id++.
2. If timestamp < last_timestamp → mark corrected.
3. Assign final sequence_id = seq_id.
4. Output OrderedEvent.

Sequencer performs no structural logic.

---

## 1.3 FMRT Core Engine — Implementation

Responsibilities:
- Implement evolution X(t) → X(t+1) according to FMRT Specification.
- Update Δ, Φ, M, κ.
- Compute morphology, regime, curvature, metric volume, temporal density.
- Compute viability decay and collapse indicators.
- Handle GAP, HEARTBEAT, RESET events.
- Ensure no invariant is violated before returning X(t+1).

Data structures:
- StructuralState: Delta[], Phi, M, Kappa.
- DerivedMetrics: morphology_index, morphology_class, regime, curvature_scalar, metric_volume, temporal_density, viability_rate, collapse_distance, collapse_speed, collapse_curvature, collapse_risk_level.

Core update logic (summary):
1. Update memory: M_next = M + dt * τ.
2. Viability decay: κ_next = κ − dt * f(Φ, curvature, load).
3. Update Δ using deformation from price_delta.
4. Update Φ using tension rules.
5. Compute curvature R and metric volume det(g).
6. Classify regime: ACC → DEV → REL → COL.
7. Classify morphology: Elastic, Plastic, Degenerate, NearCollapse.
8. Compute collapse indicators if κ → 0.

FMRT Core Engine must never produce an invalid state.

---

## 1.4 Invariant Checker — Implementation

Responsibilities:
- Validate state from Core Engine.
- Prevent invalid states from propagating.

Required checks:
- M_next ≥ M.
- κ_next ≥ 0.
- det(g) > 0 when κ > 0.
- τ > 0 when κ > 0.
- Regime ordering maintained.
- No NaN, no Inf.

Behavior:
- If any invariant fails → reject step, keep previous X(t), set error flags.

---

## 1.5 Diagnostics Module — Implementation

Responsibilities:
- Collect and attach diagnostic information.
- Populate: step_status, error_code, error_category, error_reason, event_type.
- Build final StateEnvelope.

Data structures:
- Diagnostics: step_status, error_code, error_category, error_reason, event_type.
- StateEnvelope: X_next + DerivedMetrics + InvariantFlags + Diagnostics.

Rules:
- Diagnostics must never modify X_next.

---

# 2. Data Structures

This section defines all internal data structures required for a complete and deterministic implementation of FMRT Core. All structures must be finite, memory-safe, and free from hidden mutable state. These definitions are implementation-level, unlike the abstract definitions in the Specification.

## 2.1 Structural State

StructuralState represents X(t):

- Delta[] — structural differentiation vector; fixed dimension MAX_DIM.
- Phi — structural tension scalar.
- M — structural memory; strictly non-decreasing.
- Kappa — viability; positive for living states.

Constraints:
- All fields must contain finite IEEE-754 double values.
- No NaN, no Inf, no denormals.
- Kappa > 0 for living states; Kappa = 0 denotes DEAD state.

## 2.2 Event Structures

### 2.2.1 RawTick
Represents raw market data before conversion:
- bid
- ask
- volume
- timestamp
- source_id

### 2.2.2 StructEvent
Output of Tick Adapter:
- type ∈ {STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET}
- price_delta (valid only for STRUCT_UPDATE)
- dt — elapsed time
- sequence_id — ordering value
- reason — text for RESET

### 2.2.3 OrderedEvent
Output of Event Sequencer:
- StructEvent event
- final sequence_id (monotonic, corrected if needed)

## 2.3 Derived Structural Metrics

DerivedMetrics contains all secondary computed values:

- morphology_index ∈ [0,1]
- morphology_class ∈ {Elastic, Plastic, Degenerate, NearCollapse}
- regime ∈ {ACC, DEV, REL, COL}
- curvature_scalar
- metric_volume (det g)
- temporal_density (τ)
- viability_rate (dκ/dt)
- collapse_distance
- collapse_speed
- collapse_curvature
- collapse_risk_level ∈ {low, medium, high, critical}

All must be finite and consistent with invariants.

## 2.4 Invariant Flags

InvariantFlags includes:
- memory_monotonic
- viability_nonnegative
- metric_positive
- time_density_positive
- regime_consistent
- all_invariants_ok (aggregated boolean)

These flags guide error-handling and diagnostics.

## 2.5 Diagnostics

Diagnostics contain implementation-level error reporting:

- step_status ∈ {OK, ERROR, DEAD}
- error_code — integer identifier of error type
- error_category ∈ {Input, Numerical, Invariant, Event, Other}
- error_reason — short descriptive text
- event_type — the structural event that triggered computation

Diagnostics must never mutate structural state.

## 2.6 StateEnvelope

StateEnvelope is the final unified output returned by FMRT Core:

Includes:
1. X_next — the next structural state
2. DerivedMetrics — all computed structural metrics
3. InvariantFlags — invariant results for this step
4. Diagnostics — error information and metadata

StateEnvelope is the authoritative output consumed by SDKs and higher-level systems.

## 2.7 Internal Engine Buffers

FMRT implementation may use internal fixed-size buffers:
- PreviousStateBuffer — stores X(t)
- WorkingStateBuffer — temporary construction of X(t+1)
- NumericalScratchBuffer — intermediate values for curvature, metric computations, etc.

Rules:
- No dynamic allocation on hot path.
- No shared mutable globals.
- All buffers must be deterministic and thread-safe for read-only access.

## 2.8 Summary

The data structures defined in this section:
- Enable deterministic computation
- Encapsulate all structural information
- Prevent hidden state and undefined behavior
- Match the theoretical model while remaining implementable in real-world systems

---

# 3. Algorithms and Computation

This section defines the exact computational rules required to implement structural evolution X(t) → X(t+1) and all derived metrics. Algorithms here are implementation-level counterparts of theoretical rules defined in FMRT Specification V2.1.

All computations must satisfy:
- Determinism (bitwise identical output for identical input)
- Numerical safety (no NaN, no Inf, no division by zero)
- Invariant preservation (Section 4 of Specification)
- O(1) time complexity per event
- No dynamic memory allocation on hot path

Below are the required algorithms.

## 3.1 Memory Update (M)

Memory is strictly non-decreasing. The implementation must follow:

M_next = M + dt * τ

where τ (temporal density) is computed from structural configuration. Minimal implementation form:

τ = clamp( base_tau + alpha_phi * Phi + alpha_R * curvature_scalar , tau_min , tau_max )

Notes:
- tau_min ensures strictly positive τ for κ > 0.
- τ must smoothly tend to 0 as κ → 0.
- M_next must remain finite.

## 3.2 Viability Update (κ)

Viability describes the remaining structural capacity of the organism:

κ_next = κ - dt * D

where D (decay factor) depends on:
- structural load,
- curvature magnitude,
- tension (Φ),
- memory pressure,
- morphology.

Minimal implementation form:

D = beta1 * abs(curvature_scalar)
  + beta2 * Phi
  + beta3 * morphology_index
  + beta4 * load_factor

Then:

κ_next = max( 0 , κ - dt * D )

Rules:
- κ_next must never be negative.
- κ_next = 0 → DEAD state.
- κ decay must be smooth; no discontinuities for κ > 0.

## 3.3 Delta Update (Δ)

Δ encodes structural differentiation. It evolves due to external deformation (price_delta) and internal relaxation:

Δ_next[i] = Δ[i]
            + gamma1 * price_delta
            + gamma2 * internal_relaxation_term[i] * dt

Relaxation term depends on regime:
- ACC: low damping
- DEV: medium damping
- REL: strong relaxation
- COL: collapse amplification

Δ must remain finite, no NaN, no Inf.

## 3.4 Phi Update (Φ)

Φ is structural tension. It accumulates through curvature and deformation but decays when system relaxes.

Φ_next = Φ
         + lambda1 * abs(price_delta)
         + lambda2 * curvature_scalar
         - lambda3 * relaxation_factor * dt

Rules:
- Φ_next ≥ 0
- Φ trends upward in stress and downward in relaxation

## 3.5 Curvature Computation

Curvature_scalar is computed from Δ, Φ, M, κ. Implementation must approximate:

R = curvature_scalar = norm(Δ)^2 * curvature_scale_1
                      + Φ * curvature_scale_2
                      + constant_term

Must satisfy:
- R ≥ 0, except NearCollapse cases where R → ∞ as κ → 0
- R finite for κ > 0

Metric volume (det g) is computed as:

metric_volume = metric_base - metric_coeff * R

Rules:
- det g > 0 for κ > 0
- det g → 0 as κ → 0

## 3.6 Temporal Density (τ)

Temporal density τ must satisfy:

τ > 0 for κ > 0  
τ → 0 as κ → 0

Implementation form:

τ_next = tau_min + tau_scale * exp( -kappa_sensitivity * κ )

Used in memory update.

## 3.7 Regime Classification

Regime is determined by tension, curvature, viability, and memory:

If Φ increasing and κ stable → ACC  
If Δ growth dominates → DEV  
If relaxation dominates Φ, Δ → REL  
If κ is rapidly collapsing or curvature diverges → COL  

Rules:
- Regime transitions must follow irreversible path: ACC → DEV → REL → COL.
- No backward transitions.

## 3.8 Morphology Classification

Based on curvature_scalar and κ:

If R small, κ high → Elastic  
If R medium → Plastic  
If R high, κ stable → Degenerate  
If R → ∞ or κ → 0 → NearCollapse  

Morphology_index ∈ [0,1] is computed as normalized measure of curvature intensity.

## 3.9 Collapse Indicators

collapse_distance = κ  
collapse_speed = norm(Δ_next - Δ) / dt  
collapse_curvature = R  
collapse_risk_level = mapped(R, κ) into {low, medium, high, critical}

Rules:
- collapse_distance → 0 near collapse
- collapse_speed spikes in COL regime
- collapse_risk_level must monotonically increase as κ decreases or curvature rises

## 3.10 Numerical Stability Rules

All algorithms must enforce:
- No division by zero
- No sqrt of negative values
- Clamp exponential outputs to finite ranges
- Replace invalid intermediates (NaN/Inf) with rollback to previous state

## 3.11 RTC Path for GAP and HEARTBEAT Events

### GAP event:
- Δ and Φ remain stable (no external deformation)
- M increases minimally
- κ decreases minimally
- No geometry discontinuity allowed

### HEARTBEAT event:
- Identical to GAP but dt is minimal (heartbeat interval)
- τ may drift very slightly

## 3.12 RESET Event Algorithm

RESET reconstructs a valid baseline state:

Δ = zero_vector  
Φ = Phi_reset_default  
M = 0  
κ = Kappa_reset_default  

All derived metrics recomputed consistently.

Diagnostics record reset_reason.

## 3.13 Summary

This section defines exact computation rules for:
- Updating Δ, Φ, M, κ  
- Curvature and metric volume  
- Temporal density  
- Regime and morphology  
- Collapse metrics  
- Numerical safety  
- Special event handling (GAP, HEARTBEAT, RESET)

These algorithms are the backbone of FMRT Core implementation.

---

# 4. Numerical Considerations

This section defines all numerical stability, floating-point, and deterministic computation rules required for a correct, safe, and reproducible FMRT Core implementation. These rules complement the theoretical constraints from Specification V2.1 and guarantee that numerical artifacts do not break structural invariants.

## 4.1 Floating-Point Standard

FMRT Core must operate strictly under:
- IEEE-754 double precision
- Deterministic rounding mode (round-to-nearest-even)
- No extended precision registers (x87 must be disabled or forced to 64-bit mode)
- No fast-math optimizations

Compiler flags that break determinism, reorder floating-point operations, or produce non-reproducible results must be disabled.

## 4.2 Forbidden Floating-Point Behavior

The implementation must ensure that:
- No NaN is ever created
- No Inf is ever created
- No subnormal numbers propagate into X(t)
- No division by zero occurs
- No invalid operations (0 * Inf, Inf – Inf, etc.)

If any such condition is detected while computing X(t+1), the engine must:
- Reject the step
- Preserve X(t)
- Emit numerical hazard diagnostics

## 4.3 Numerical Clamping Rules

To prevent overflow or instability, the following must be clamped to finite ranges:
- Exponential functions producing τ, curvature amplification, or morphology weights
- High curvature values approaching collapse
- Norms of Δ and Δ_next
- Any computed intermediate value exceeding safe numeric limits

Recommended maximum magnitude for any intermediate value: 1e308  
Recommended minimum positive threshold: 1e-308  

Values outside these limits must be clamped or the step must be rejected.

## 4.4 Deterministic Arithmetic Ordering

All arithmetic operations must be performed in a **fixed order** defined by implementation.  
No reordering, vectorization, or parallel accumulation that may yield different rounding results across platforms may be used.

Example policies:
- Compute sums in fixed left-to-right order
- Compute multiplications before additions when defined by algorithm
- Use explicit fused multiply-add only if guaranteed consistent across platforms

Determinism must be maintained across:
- CPU architectures
- Operating systems
- Compilers

## 4.5 Safe Update Conditions

Before assigning:
- Δ_next
- Φ_next
- M_next
- κ_next

The implementation must verify:
- All intermediate computations are finite
- No invariant is already violated
- No negative viability is produced
- No invalid metric state is produced (det g <= 0 for κ > 0)
- τ remains strictly positive for κ > 0

If any check fails → reject step.

## 4.6 Numerical Checks for Invariants

Invariant Checker relies on exact, finite values. Therefore:

- det g must be computed using numerically stable formulas
- curvature must avoid catastrophic cancellation
- Δ norms must be computed using stable summation rules
- Any division by dt must verify dt > 0

If curvature diverges faster than representable → collapse is assumed and κ_next = 0 (DEAD).

## 4.7 Handling dt in Computations

dt must satisfy:
- dt > 0 for UPDATE, GAP, HEARTBEAT
- For HEARTBEAT, dt = predefined heartbeat_interval
- For GAP, dt may be large but must remain finite

If dt <= 0 → reject event.

dt must be used as-is; no smoothing, averaging, or adapting allowed.

## 4.8 Precision of Derived Metrics

Derived metrics such as:
- morphology_index
- viability_rate
- collapse_distance
- collapse_speed
- curvature_scalar
- temporal_density

must be:
- finite
- free of NaN
- consistent with structural state

Viability_rate = (κ_next − κ) / dt must be computed safely:
- If dt is extremely small → clamp denominator to numerical_min_dt
- Avoid division by extremely small dt causing large rounding error

## 4.9 Special Case Handling

### NearCollapse
When κ approaches zero:
- curvature_scalar may grow rapidly
- det g approaches 0
- temporal_density approaches 0

These transitions must be:
- smooth (no jumps)
- monotonic
- consistent with Specification V2.1 collapse conditions

### DEAD State
When κ_next = 0:
- All fields must remain constant
- All derived metrics remain constant or become trivial
- No future computation may change X(t)

## 4.10 Summary

To guarantee safety and correctness:
- Always enforce IEEE-754 determinism
- Never allow NaN/Inf
- Clamp extreme values to safe ranges
- Validate dt and all intermediates
- Maintain invariant consistency through numeric safeguards

These rules ensure the FMRT Core never produces invalid structural states due to numerical issues.

---

# 5. Event Handling

This section specifies the exact implementation rules for handling all four event types accepted by FMRT Core: STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, and STRUCT_RESET.  
Each event type produces deterministic evolution of X(t), must preserve invariants, and must comply with numerical safety rules (Section 4).

FMRT Core processes exactly one event at a time and produces a StateEnvelope.

---

## 5.1 STRUCT_UPDATE Handling

STRUCT_UPDATE is the primary event type and represents an observable external deformation.

Inputs:
- price_delta (external deformation proxy)
- dt (elapsed external time)

Update rules:

1. Compute Δ_next using:
   - deformation contribution from price_delta
   - internal relaxation term scaled by dt

2. Compute Φ_next by:
   - tension accumulation from curvature and deformation
   - relaxation decay proportional to regime and morphology

3. Compute curvature_scalar and metric_volume from updated Δ, Φ, M, κ.

4. Update memory:
   - M_next = M + dt * τ
   - τ computed from Section 3 rules
   - ensure M_next ≥ M

5. Update viability:
   - κ_next = κ − dt * D
   - where D = decay factor derived from curvature, tension, load
   - ensure κ_next ≥ 0

6. Classify regime and morphology.

7. Compute collapse indicators.

8. Validate invariants:
   - M monotonic
   - κ ≥ 0
   - det(g) > 0 for κ > 0
   - τ > 0 for κ > 0
   - regime irreversible
   - all finite

If ANY check fails → reject step (StepStatus = ERROR).

---

## 5.2 STRUCT_GAP Handling

STRUCT_GAP represents missing external data or long delays between ticks.  
It must never create artificial deformation.

Rules:

1. Δ_next = Δ (no deformation).
2. Φ_next = Φ (no added tension).
3. M_next = M + dt * τ_gap  
   - τ_gap is minimal temporal density allowed by theory.
4. κ_next = κ − dt * decay_gap  
   - decay_gap is minimal viability decay.
5. curvature_scalar and metric_volume recomputed consistently but without external deformation amplification.
6. regime must not jump to unexpected states; transitions must remain monotonic.
7. morphology must remain stable unless κ naturally forces transition.

STRUCT_GAP must not:
- create spikes,
- smooth structure,
- alter deformation trajectory.

Only minimal drift is allowed.

---

## 5.3 STRUCT_HEARTBEAT Handling

STRUCT_HEARTBEAT is a micro-event representing no external motion but continuous structural time.

Rules:

1. Δ_next = Δ (no deformation).
2. Φ_next = Φ (no tension change).
3. dt = heartbeat_interval (small constant).
4. M_next = M + dt * τ_heartbeat  
   - τ_heartbeat slightly > 0 to keep memory monotonic.
5. κ_next = κ − dt * decay_heartbeat  
   - usually very small decay compared to GAP.

Heartbeat events prevent time stagnation and keep X(t) evolving smoothly between real updates.

---

## 5.4 STRUCT_RESET Handling

STRUCT_RESET is a controlled structural restart.

Rules:

1. Δ_next = zero vector (reset baseline).
2. Φ_next = Phi_reset_default.
3. M_next = 0.
4. κ_next = Kappa_reset_default (must be > 0).
5. All derived metrics recomputed from baseline.
6. Regime = ACC.
7. Morphology = Elastic.
8. collapse metrics = safe defaults.
9. invariant flags = all true.

Diagnostics:
- Must record reset_reason.
- Must return StepStatus = OK.

RESET must always produce a valid structural state.

---

## 5.5 Soft Failure Handling

Soft failures occur when:
- dt ≤ 0,
- inputs contain NaN/Inf,
- impossible deformation values,
- intermediate numeric hazards that do not violate invariants but invalidate computation.

Behavior:
- Reject step.
- Preserve X(t).
- StepStatus = ERROR.
- error_category = Input or Numerical.
- Provide descriptive error_reason.

---

## 5.6 Hard Failure (DEAD State)

Occurs when viability collapses:

If κ_next = 0:

1. Freeze Δ, Φ, M, κ.
2. Freeze all derived metrics.
3. Regime = COL.
4. Morphology = NearCollapse.
5. Any subsequent event produces identical output until RESET.
6. StepStatus = DEAD.

Hard failures represent end of structural life.

---

## 5.7 Event Processing Order

For every event E(t):

1. Validate event fields.
2. Compute tentative X_next.
3. Validate numerical intermediates.
4. Apply invariant checker.
5. If OK → accept X_next.  
6. If ERROR → revert to previous X(t).  
7. Build StateEnvelope with diagnostics.

This strict order guarantees determinism and invariant preservation.

---

## 5.8 Summary

STRUCT_UPDATE:
- Full evolution step.

STRUCT_GAP:
- Minimal evolution without deformation.

STRUCT_HEARTBEAT:
- Micro-step to maintain continuity.

STRUCT_RESET:
- Full state reinitialization.

Soft Failure:
- Reject step, keep X(t).

Hard Failure:
- κ reaches 0 → DEAD state.

Event handling is the mechanism that binds theoretical FMRT dynamics to real-time implementation.

---

# 6. API Signatures

This section defines the exact function-level API used internally by FMRT Core and externally by SDKs or integration layers.  
All APIs must be deterministic, side-effect-free (except updating internal structural state), and adhere to strict validation rules defined in the Specification and Design Documents.

APIs are divided into:
1) Internal module APIs  
2) Public FMRT Core API for SDKs  

These signatures provide a blueprint for real implementation, independent of programming language.

---

## 6.1 Tick Adapter API

Function:  
processRawTick(raw_tick) → StructEvent

Inputs:
- raw_tick: (bid, ask, volume, timestamp, source_id)

Outputs:
- StructEvent: (type, price_delta?, dt, sequence_id, reason?)

Rules:
- Must validate raw_tick.
- Must never return NaN or Inf.
- Must map impossible ticks to either GAP or HEARTBEAT events.

---

## 6.2 Event Sequencer API

Function:  
orderEvent(struct_event) → OrderedEvent

Inputs:
- struct_event (unordered)

Outputs:
- OrderedEvent: event + final sequence_id

Rules:
- sequence_id must monotonically increase.
- No event interpretation (Sequencer is not a logic module).
- Must correct events arriving out-of-order.

---

## 6.3 FMRT Core Engine API

Function:  
computeNextState(X_current, ordered_event) → (X_next, DerivedMetrics)

Inputs:
- X_current: the last valid structural state
- ordered_event: event guaranteed to be ordered and validated

Outputs:
- X_next: updated structural state
- DerivedMetrics: morphology, regime, curvature, τ, viability_rate, collapse indicators

Rules:
- Must enforce all algorithms in Section 3.
- Must NOT apply invariant checks — these are handled by Invariant Checker.
- Must guarantee finite outputs for κ > 0 or trigger error conditions.

---

## 6.4 Invariant Checker API

Function:  
validateInvariants(prev_state, next_state) → InvariantFlags

Inputs:
- prev_state: last accepted X(t)
- next_state: tentative X(t+1)

Outputs:
- InvariantFlags: booleans for memory_monotonic, viability_nonnegative, metric_positive, time_density_positive, regime_consistent, all_invariants_ok

Rules:
- If ANY flag fails → state must be rejected by FMRT engine wrapper.
- Invariant checker does NOT mutate the state.

---

## 6.5 Diagnostics Module API

Function:  
buildDiagnostics(step_status, error_code, error_category, error_reason, event_type) → Diagnostics

Inputs:
- step_status ∈ {OK, ERROR, DEAD}
- error_code ∈ int
- error_category ∈ {Input, Numerical, Invariant, Event, Other}
- error_reason: brief text
- event_type: the event that triggered the step

Outputs:
- Diagnostics: the final diagnostic block for StateEnvelope

Rules:
- Must not modify X_next.
- Must be purely descriptive.

---

## 6.6 StateEnvelope Builder API

Function:  
buildEnvelope(X_next, derived_metrics, invariant_flags, diagnostics) → StateEnvelope

Inputs:
- X_next: structural state after engine update
- derived_metrics: all computed secondary metrics
- invariant_flags: results from invariant checker
- diagnostics: Diagnostics module output

Outputs:
- StateEnvelope (final FMRT output)

Rules:
- No mutation of internal engine buffers.
- Envelope must be complete and deterministic.

---

## 6.7 Public FMRT API (SDK Interface)

The SDK interacts only with the FMRT Core through the following stable interface:

Function:  
FMRT_processEvent(event) → StateEnvelope

Rules:
- event must already be validated and ordered (SDK must use Tick Adapter + Sequencer)
- FMRT_processEvent must:
  1) call computeNextState  
  2) call validateInvariants  
  3) choose ACCEPT or REJECT  
  4) generate diagnostics  
  5) build StateEnvelope  
  6) return COMPLETELY deterministic output

Additional public functions:

getCurrentState() → X(t)  
Returns last accepted structural state.

getDiagnostics() → Diagnostics  
Returns diagnostics from last processed step.

resetFMRT(reason)  
Triggers a STRUCT_RESET event internally.

---

## 6.8 Summary

The APIs defined here:
- Provide exact rules for module communication
- Establish deterministic and safe event processing
- Serve as the authoritative blueprint for FMRT Core implementation
- Ensure full compatibility with the Design and Specification documents

---

# 7. Performance and Optimization Notes

This section provides implementation-level guidelines to ensure FMRT Core achieves real-time performance while maintaining determinism, invariant preservation, and numerical safety.  
All optimizations must comply with the rules in Specification V2.1 and Design Document V2.1.

## 7.1 O(1) Event Processing

FMRT Core must guarantee constant-time execution for every event, regardless of:
- history length,
- number of processed ticks,
- structural complexity.

Implementation requirements:
- No dynamic allocations in the hot path.
- No recursion.
- No unbounded loops.
- Arrays and buffers must have fixed size.
- Derived metrics must reuse pre-allocated scratch buffers.

## 7.2 Memory Layout Optimization

StructuralState, DerivedMetrics, InvariantFlags, and Diagnostics must be stored in contiguous memory for:
- cache efficiency,
- deterministic CPU behavior,
- fast copying and comparison.

Guidelines:
- Use fixed-size arrays for Delta.
- Avoid pointer-chasing or heap-allocated linked structures.
- Prefer local stack structures during computation.

## 7.3 Avoiding Branch Explosion

Branch mispredictions can degrade performance and introduce micro-level nondeterminism.

Mitigation strategies:
- Replace deep if-else chains with table-driven logic for regime/morphology.
- Use precomputed lookup factors for relaxation, tension accumulation, or decay.
- Flatten nested conditionals when possible.

## 7.4 Mathematical Optimization

While maintaining numerical safety:
- Precompute constants such as curvature coefficients, morphology thresholds.
- Precompute exponentials or power-law factors into lookup tables if allowed.
- Use linear approximations only if guaranteed deterministic and stable.

Forbidden:
- fast-math compiler optimizations,
- architecture-dependent FMA behavior unless strictly controlled.

## 7.5 Efficient Curvature and Metric Computation

Curvature and det(g) must be computed:
- using stable formulas with minimal FLOPs,
- without redundant recomputation,
- using shared intermediate terms across Δ, Φ, M, κ updates.

Optimization pattern:
- compute norm(Δ) once,
- reuse it for curvature, viability decay, morphology index.

## 7.6 Event Batching in the Sequencer

The Sequencer may batch events when incoming rate exceeds processing capacity.

Constraints:
- Batching must not alter event order.
- dt must remain accurate.
- HEARTBEAT events may be synthesized to maintain temporal continuity.

Core Engine must still process each event individually in O(1) time.

## 7.7 Deterministic Update Ordering

To avoid platform-dependent rounding deviations:
- All update rules must follow a fixed arithmetic order.
- Do not reorder operations for “performance” unless bitwise-identical results are guaranteed.

Example rules:
- Evaluate multiplications before additions when defined.
- Sum components of Δ in fixed left-to-right sequence.
- Evaluate curvature formula in a fixed order.

## 7.8 Eliminating Dead Code and Redundant Paths

Ensure:
- No unused branches or legacy paths remain.
- No alternate update rules hidden in conditional compilation.
- No platform-specific behavior (e.g., SSE vs. AVX) unless deterministic.

## 7.9 Multithreading Optimization (Read-Only Only)

FMRT Core updates X(t) serially — *never parallelize state mutation*.

Allowed:
- Parallel read-only diagnostics,
- Parallel monitoring,
- Parallel logging or visualization.

Forbidden:
- Parallel state updates,
- Parallel curvature computation affecting X(t),
- Parallel invariant checking.

## 7.10 Handling High-Frequency Regimes

For extreme event rates (e.g., >2000 events/sec):
- Tick Adapter may drop overly granular updates by merging micro-ticks into a stable STRUCT_UPDATE with corrected dt.
- Sequencer must throttle events while keeping time monotonic.
- Core Engine must remain O(1) regardless of rate.

This does not violate FMRT theory because structural events, not raw ticks, are the source of truth.

## 7.11 Cache and Branch Efficiency

To ensure stable latencies:
- Keep hot loops small and branch-free.
- Group related fields in structures-of-arrays if beneficial.
- Avoid unpredictable branches inside numerical core.

## 7.12 Summary

FMRT Core must:
- Run in constant time per event,
- Use fixed-size memory structures,
- Avoid dynamic allocation and non-deterministic arithmetic,
- Maintain deterministic behavior under high-frequency event loads,
- Provide stable and repeatable performance across architectures.

These implementation rules ensure FMRT Core meets real-time requirements without sacrificing mathematical correctness.

---

# 8. Error Handling and Diagnostics

This section describes how errors are detected, classified, and reported during FMRT Core execution.  
The goal is to ensure strict fault isolation, invariant protection, deterministic behavior, and actionable diagnostics without ever mutating the structural evolution logic.

FMRT must never continue with an invalid or partially computed state.  
Diagnostics must provide precise, deterministic information for developers, SDKs, and test suites.

---

## 8.1 Error Categories

Every error must belong to exactly one of the following categories:

- Input — malformed or invalid event data (dt ≤ 0, NaN, Inf, impossible price_delta).
- Numerical — floating-point hazards (NaN, Inf, division by zero, overflow).
- Invariant — violation of structural invariants (memory, viability, metric positivity, regime ordering, etc.).
- Event — unsupported or inconsistent event type, sequencing error.
- Other — unexpected internal condition that does not fit above categories.

Error categories must never overlap.

---

## 8.2 Error Codes

Error codes must be stable across implementations.  
Recommended ranges:

- 100–199: Input errors  
- 200–299: Numerical errors  
- 300–399: Invariant violations  
- 400–499: Event processing errors  
- 900–999: Internal / unknown errors

Example assignments (not exhaustive):
- 101 — dt <= 0  
- 102 — NaN in event data  
- 201 — division by zero  
- 301 — memory monotonicity violation  
- 302 — negative viability attempted  
- 303 — metric degeneracy (det g <= 0 while κ > 0)  
- 304 — τ <= 0 while κ > 0  
- 401 — unsupported event type  

Actual implementation may expand this list.

---

## 8.3 Error Propagation Model

When an error is detected:

1. Computation stops immediately.  
2. X(t) remains unchanged.  
3. StepStatus is assigned:
   - ERROR for recoverable events,
   - DEAD if κ transitions to 0,
   - OK otherwise.  
4. Diagnostics are filled with:
   - error_code  
   - error_category  
   - error_reason  
   - event_type  
5. Invariant flags are updated accordingly.  
6. StateEnvelope is returned to SDK.

Errors must never:
- leak into subsequent state updates,
- modify internal buffers incorrectly,
- create undefined behavior.

---

## 8.4 Handling Soft Failures

Soft failures include:
- invalid dt,
- malformed event structure,
- numerical anomalies that occur before invariants are broken,
- out-of-range inputs.

Soft failure behavior:
- Reject event.
- Preserve state (X_next = X_current).
- Diagnostics: step_status = ERROR.
- error_code and error_category must pinpoint cause.

Soft failures do NOT move system into DEAD state.

---

## 8.5 Handling Hard Failures (DEAD State)

Hard failures occur when viability collapses:

κ_next = 0 → DEAD state

Rules:
- All fields freeze: Δ, Φ, M, κ, derived metrics.
- No further evolution allowed except RESET.
- Any event processed after death returns identical StateEnvelope.
- step_status = DEAD.
- error_category = Invariant.
- error_code must reference collapse condition (e.g., 302).

DEAD state must be deterministic and irreversible.

---

## 8.6 RESET Event Diagnostics

RESET event always sets:
- step_status = OK  
- error_code = 0  
- error_category = None  
- error_reason = reset reason  

RESET must never be marked as an error.

---

## 8.7 Invariant Violation Workflow

When invariants fail:
- ComputeNextState output is discarded.
- X(t) is preserved.
- diagnostic.error_category = Invariant
- diagnostic.step_status = ERROR
- appropriate error_code assigned (e.g., 301–304)
- invariant flags indicate the exact failure source.

No partial updates allowed.

---

## 8.8 Diagnostic Completeness Requirements

Diagnostics must always include:
- step_status  
- error_code  
- error_category  
- error_reason  
- event_type  
- all invariant flags  
- sequence_id (from event)  
- timestamp (optional but recommended)

Diagnostics must be deterministic and must not affect structure or state evolution.

---

## 8.9 Logging Guidelines (Optional)

If logging is enabled:
- Log only StateEnvelope outputs.
- Never log raw floating-point intermediates that may differ across builds.
- Logging must not block execution.
- Logging must be atomic per event.

---

## 8.10 Summary

Error handling and diagnostics ensure:
- structural invariants are never violated,
- invalid states never propagate,
- developers have actionable, structured error information,
- deterministic FMRT behavior is preserved across platforms,
- DEAD state and RESET logic work consistently.

This section defines the complete framework for reliable, platform-independent error management.

---

# 9. Testing Guidelines

This section describes the testing methodology required to validate a complete FMRT Core implementation.  
The goal is to guarantee determinism, invariant correctness, numerical safety, and full compliance with FMRT Specification V2.1 and Design Document V2.1.

Testing is divided into:
- Unit tests  
- Integration tests  
- Determinism tests  
- Stress & performance tests  
- Collapse-edge tests  
- RESET & recovery tests  
- Regression suites  

FMRT must pass **all** categories before considered production-ready.

---

## 9.1 Unit Testing

Each module must be validated independently:

### Tick Adapter tests:
- Valid tick produces correct STRUCT_UPDATE.
- Bad timestamps → HEARTBEAT.
- Missing ticks → GAP.
- RESET triggers correctly.
- Ensure no NaN, Inf, or invalid dt produced.

### Event Sequencer tests:
- Strictly increasing sequence_id.
- Correction of out-of-order events.
- Duplicate event removal.
- Deterministic ordering under high-frequency input.

### Core Engine tests:
- Correct application of Δ, Φ, M, κ update rules.
- Temporal density always positive when κ > 0.
- Viability decay consistent and never becomes negative.
- Curvature finite (unless collapse intended).

### Invariant Checker tests:
- Detect negative κ.
- Detect metric degeneracy (det g <= 0).
- Detect τ <= 0.
- Detect regime violations.
- Ensure no false positives.

### Diagnostics Module tests:
- Proper classification of error types.
- Correct error_code assignment.
- Diagnostics must never mutate structural state.

---

## 9.2 Integration Testing

End-to-end simulation tests through full pipeline:
Tick Adapter → Sequencer → Core Engine → Invariant Checker → Diagnostics

Required cases:
- Continuous realistic tick streams.
- Randomized micro-tick noise.
- Mixed UPDATE/GAP/HEARTBEAT streams.
- Sudden RESET events.
- Long-term simulations (millions of events).

Assertions:
- X(t) must never go out of bounds.
- Structural invariants must always hold except at controlled collapse.
- No NaN/Inf may appear at any step.

---

## 9.3 Determinism Testing

These tests ensure FMRT behaves identically across:
- CPUs,
- operating systems,
- compiler versions,
- different execution schedules.

Procedure:
- Replay same event stream multiple times → outputs must be bitwise identical.
- Multi-threaded read access must not modify X(t).
- Deterministic logging and diagnostic generation.

If outputs differ → determinism violation → implementation invalid.

---

## 9.4 Stress Testing

Test extreme conditions:
- 2000+ events per second.
- Huge bursts of UPDATE events.
- Sequences of very small dt (near-zero heartbeat intervals).
- Large dt GAP events.

FMRT must:
- remain fast (O(1)),
- avoid memory leaks,
- avoid numerical hazards,
- avoid invariant violations.

---

## 9.5 Near-Collapse and Collapse Testing

Critical testing area.

Required scenarios:
- κ slowly decreases toward zero.
- curvature_scalar increases toward divergence.
- metric_volume approaches zero.
- τ collapses smoothly.

Assertions:
- κ_next must never be negative.
- Collapse must be monotonic.
- DEAD state must be triggered exactly when κ == 0.
- After collapse, state must freeze.

---

## 9.6 RESET Testing

RESET scenarios:
- Manual RESET.
- RESET after collapse.
- RESET after soft failure.
- RESET under heavy event load.

Assertions:
- New state must satisfy all invariants.
- Regime must reset to ACC.
- Morphology must reset to Elastic.
- Memory must reset to zero.
- Diagnostics must report reset_reason.

---

## 9.7 Error Classification Testing

Test that each error condition produces:
- correct step_status,
- correct error_code,
- correct error_category,
- correct invariant flags,
- correct preservation of X(t).

Test cases must cover:
- invalid dt,
- malformed events,
- numerical hazards,
- invariant violations,
- unsupported event types.

---

## 9.8 Regression Testing

Once FMRT passes all tests:
- Lock in test suite.
- Future changes must not alter outputs for existing test cases.
- Deterministic replay tests must match previous versions bitwise.

This guarantees stability across versions.

---

## 9.9 Summary

The FMRT testing framework must ensure:
- module correctness,
- deterministic behavior,
- invariant preservation,
- reliable collapse/RESET dynamics,
- numerical safety,
- performance stability.

A fully compliant FMRT Core MUST pass all categories before release.
