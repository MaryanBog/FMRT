# FMRT-Specification-V2.0-EN

---

# 0. Purpose and Scope

The FMRT Core is the fundamental structural engine of the Flexion Market Organism.  
Its purpose is to evolve a structural state  
\[
X(t) = (\Delta(t), \Phi(t), M(t), \kappa(t))
\]
into the next state \(X(t+1)\) according to Flexion Market Theory V3.0, while ensuring strict determinism, invariant preservation, and operational safety in real-world data conditions.

This specification defines:

- **What** the FMRT Core must do,  
- **What** inputs it accepts,  
- **What** outputs it produces,  
- **What** invariants are required by theory,  
- **What minimal engineering constraints** are necessary to maintain correctness under imperfect, noisy, or corrupted market data.

The specification does **not** describe architecture, module layout, algorithms, internal formulas, or implementation details.  
Those topics are covered respectively in the Design Document and the Implementation Document.

FMRT Core is a pure structural system:  
it operates exclusively in structural space, independent of observable price or any external market representation.  
All external data must be transformed into normalized structural events before entering the core.

The scope of this document includes:

- definition of valid structural states,  
- accepted structural event types,  
- expected behavior under normal and degraded conditions,  
- determinism guarantees,  
- numerical stability requirements,  
- invariant enforcement rules,  
- allowed and forbidden behaviors.

Anything outside these boundaries—architecture, code, algorithms, optimization—belongs to other documents in the FMRT documentation set.

---

# 1. System Description

The FMRT Core is a deterministic structural evolution engine that transforms a living structural state  
\[
X(t) = (\Delta(t), \Phi(t), M(t), \kappa(t))
\]
into its successor state \(X(t+1)\) in full accordance with Flexion Market Theory V3.0.

FMRT does not operate on observable market data (price, candles, volume, order flow).  
All external information must first be translated into **normalized structural events**, ensuring that the core interacts only with abstract structural transformations, not with raw or imperfect real-world inputs.

FMRT defines and preserves the organism’s internal dynamics, including:

- structural deformation (Δ),
- structural tension (Φ),
- irreversible memory progression (M),
- viability decay (κ),
- curvature behavior,
- temporal density,
- morphological class,
- and dynamic regime.

The core’s responsibilities are strictly limited to:

1. **Processing structural events**  
   A structural event describes how the external environment *appears* to have changed, not the raw data itself.  
   FMRT accepts exactly four event types:  
   - `STRUCT_UPDATE(price_delta, dt)`  
   - `STRUCT_GAP(dt)`  
   - `STRUCT_HEARTBEAT(dt)`  
   - `STRUCT_RESET(reason)`

2. **Producing deterministic structural evolution**  
   For every accepted event and valid state, FMRT generates a next-state envelope containing:  
   - updated structural fields,  
   - morphology,  
   - regime,  
   - curvature and temporal metrics,  
   - viability dynamics,  
   - collapse indicators,  
   - and invariant validation results.

3. **Preserving theoretical invariants**  
   FMRT must not produce any state that contradicts structural theory.  
   If an event would violate invariants, the step is rejected safely.

4. **Ensuring numerical stability and safe execution**  
   FMRT must remain operational under imperfect or degraded data conditions, including missing updates, gaps, or inconsistent timing.

The system is **pure** and **side-effect free**:  
FMRT does not depend on external clocks, randomness, platform state, or hidden mutable variables.  
Given identical inputs, it must always produce identical outputs.

FMRT serves as the authoritative source of structural truth within any Flexion-based system, and all higher-level modules (SDK, trading agents, analytics) must rely solely on FMRT outputs, not on external projections.

---

# 2. Inputs

FMRT Core does not accept raw market data.  
It operates exclusively on **validated structural states** and **normalized structural events**.  
This ensures strict determinism, theoretical correctness, and protection from corrupted external inputs.

FMRT has exactly two categories of input:

1. **The structural state at time t**  
2. **A structural event describing the external change**

No additional inputs are permitted.

---

## 2.1 Structural State X(t)

A structural state is a 4-tuple:
\[
X(t) = (\Delta(t), \Phi(t), M(t), \kappa(t))
\]

### 2.1.1 Field Definitions (Updated with Recommended Ranges)

- **Δ (Delta)**  
  Structural differentiation vector.  
  Type: finite real vector, dimension ≥ 1.  
  Constraint: \(\|\Delta(t)\| < \infty\).  
  **Recommended operational range for testing:** norm(Δ) ∈ [0, 1.0]

- **Φ (Phi)**  
  Structural tension scalar.  
  Type: non-negative real number.  
  Constraint: \(\Phi(t) \ge 0\).  
  **Recommended operational range for testing:** 0 ≤ Φ ≤ 10.0

- **M (Memory)**  
  Irreversible structural imprint.  
  Type: non-negative real number.  
  Constraint: monotonic: \(M(t) \ge 0\).  
  **Recommended operational range for testing:** 0 ≤ M ≤ 1e6

- **κ (Kappa)**  
  Viability field.  
  Type: positive real number.  
  Constraint: \(\kappa(t) > 0\) for all living states.  
  **Recommended operational range for testing:** 0 < κ ≤ 1.0

### 2.1.2 Validity of Input State

FMRT must accept a structural state only if:

- All fields are finite real numbers.
- \(\kappa(t) > 0\).
- No invariant is violated at the moment of input.
- The state contains no NaN, no Inf, and no denormal values.
- The state is internally consistent:  
  Δ, Φ, M, κ must belong to a domain where metric positivity is theoretically possible.

If any requirement is violated, FMRT must reject the step and return an error envelope without evolving the state.

---

## 2.2 Structural Events

FMRT does not interact with price, ticks, order books, or any market-level data.  
External data sources must be processed by a **Tick Adapter**, which converts real-world noise into a clean, deterministic event stream.

FMRT accepts **four event types only**:

### 2.2.1 `STRUCT_UPDATE(price_delta, dt)`

Represents a normal observable change in projection.  
Input fields:

- `price_delta` — a small external displacement (real number)  
- `dt` — elapsed external time in seconds (positive real number)

FMRT does not use price directly; `price_delta` is an abstract deformation input used by the evolution operator.

### 2.2.2 `STRUCT_GAP(dt)`

Represents missing data or an interruption in the external feed.  
FMRT must perform only **minimal irreversible progression**:

- Slight increase in M  
- Slight decrease in κ  
- No artificial changes in Δ or Φ except those required by theoretical consistency

### 2.2.3 `STRUCT_HEARTBEAT(dt)`

Represents the absence of external motion but continuous structural time.  
FMRT must:

- Consider this a zero-deformation step  
- Apply minimal temporal evolution  
- Keep Δ and Φ stable

### 2.2.4 `STRUCT_RESET(reason)`

Forces a reinitialization of the structural organism.  
FMRT must:

- Produce a clean, valid baseline state  
- Reset time-dependent quantities  
- Clear internal buffers  
- Mark the state as reset in the output envelope

---

## 2.3 Event Validity Rules

FMRT must reject events that violate:

- dt ≤ 0  
- non-finite values in event parameters  
- NaN / Inf  
- inconsistent or impossible transitions  
- invalid ordering or duplicated timestamps (as determined by the adapter)

Upon rejection:

- FMRT returns the previous state unchanged  
- An error flag is included in the output envelope

---

## 2.4 Input Purity Requirement

FMRT must never:

- access system time,  
- generate randomness,  
- rely on external clocks,  
- depend on event source identity,  
- modify X(t) without an event.

The **only** input into FMRT Core is:

(X(t), Event(t))

Everything else belongs to external systems (adapter, platform, SDK).

### 2.5 Example Event Formats (Optional)

STRUCT_UPDATE example:
{
  "event_type": "STRUCT_UPDATE",
  "price_delta": 0.0005,
  "dt": 0.001
}

STRUCT_GAP example:
{
  "event_type": "STRUCT_GAP",
  "dt": 0.05
}

STRUCT_HEARTBEAT example:
{
  "event_type": "STRUCT_HEARTBEAT",
  "dt": 0.01
}

STRUCT_RESET example:
{
  "event_type": "STRUCT_RESET",
  "reason": "manual_restart"
}

---

# 3. Outputs

FMRT Core produces a **State Envelope** for each processed event. This envelope contains: (1) the next structural state X(t+1), (2) derived structural descriptors (morphology, regime, curvature, etc.), (3) temporal and viability metrics, (4) collapse-related indicators, (5) invariant validation flags, and (6) optional diagnostic information. All outputs must be deterministic, finite, and consistent with Flexion Market Theory V3.0.

## 3.1 Next Structural State

FMRT must compute:
X(t+1) = (Δ(t+1), Φ(t+1), M(t+1), κ(t+1))

Returned in the form:
X_next = { Delta_next, Phi_next, M_next, Kappa_next }

### 3.1.1 Required Properties
- M_next >= M  (strictly greater for non-trivial evolution)
- Kappa_next >= 0
- all components finite
- no NaN or Inf
- Δ, Φ remain within admissible structural domains
- if Kappa_next == 0, the organism enters the DEAD state

## 3.2 Morphology

FMRT must compute the morphology of the organism:

morphology_index ∈ [0, 1]  
morphology_class ∈ {Elastic, Plastic, Degenerate, NearCollapse}

Morphology must reflect curvature magnitude, viability level, tension distribution, and memory load.

## 3.3 Dynamic Regime

FMRT must classify the structural dynamic regime:

regime ∈ {ACC, DEV, REL, COL}

Additional outputs:
- is_dead — true iff Kappa_next == 0
- regime_transition_valid — ensures irreversible regime progression

## 3.4 Curvature and Geometry

FMRT must output geometric descriptors:

curvature_scalar — R(X)  
curvature_trend — sign/magnitude estimate of dR/dt  
metric_volume — det g(X)  
geometry_health_score — optional compressed index

All geometric outputs must satisfy continuity for κ > 0 and theoretical collapse behavior as κ → 0.

## 3.5 Temporal Outputs

FMRT must output internal temporal data:

temporal_density — τ(X) = dM/dt  
structural_time_next — next internal time value  
time_trend — {expanding, stable, collapsing}

Constraints:
- τ > 0 for κ > 0  
- τ → 0 as κ → 0  

## 3.6 Viability Dynamics

FMRT must compute viability decay behavior:

viability_rate — dκ/dt  
viability_trend — monotonic decay confirmation  
is_viability_strict_decay — true when κ decreases under non-trivial stress

Viability dynamics must be consistent with Φ, R, τ, and M.

## 3.7 Collapse Geometry Outputs

FMRT must estimate proximity to collapse:

collapse_distance — scalar distance to κ = 0  
collapse_speed — || dΔ/dt ||  
collapse_curvature — curvature of collapse trajectory  
collapse_risk_level ∈ {low, medium, high, critical}

These values must reflect theoretical collapse conditions:
κ → 0, det g → 0, R → ∞, τ → 0.

## 3.8 Invariant Flags

FMRT must validate the following invariants:

invariant_memory_monotonic  
invariant_viability_nonnegative  
invariant_metric_positive  
invariant_time_density_positive  
invariant_regime_consistent

If any invariant fails, FMRT must produce an **error step** and return the previous state unchanged (see Section 6).

### 3.9 Diagnostics (Optional) — Updated

FMRT Core may return diagnostic information for debugging and logging purposes.  
Diagnostics are purely informative and **must not influence the structural evolution**.

Current fields:

- `event_type` — type of structural event processed
- `step_status` ∈ {OK, ERROR, DEAD} — overall result of the step
- `error_reason` — descriptive text if an error occurred

**New fields for enhanced diagnostics:**

- `error_code` — integer representing the specific type of error
- `error_category` ∈ {Input, Numerical, Invariant, Event, Other} — classification of the error

Example StateEnvelope diagnostics section:

{
  "step_status": "ERROR",
  "error_code": 101,
  "error_category": "Invariant",
  "error_reason": "Viability decay exceeded theoretical bounds",
  "event_type": "STRUCT_UPDATE"
}

### 3.10 Example StateEnvelope Format

This example demonstrates the full structure of the StateEnvelope returned by FMRT Core after processing a structural event.

{
  "X_next": {
    "Delta": [0.12, 0.05, 0.08],
    "Phi": 0.25,
    "M": 12.345,
    "Kappa": 0.987
  },
  "morphology_index": 0.42,
  "morphology_class": "Plastic",
  "regime": "DEV",
  "curvature_scalar": 0.13,
  "metric_volume": 1.02,
  "temporal_density": 0.015,
  "structural_time_next": 12.346,
  "viability_rate": -0.002,
  "collapse_distance": 0.15,
  "collapse_speed": 0.004,
  "collapse_curvature": 0.11,
  "collapse_risk_level": "medium",
  "invariant_flags": {
    "memory_monotonic": true,
    "viability_nonnegative": true,
    "metric_positive": true,
    "time_density_positive": true,
    "regime_consistent": true
  },
  "step_status": "OK",
  "error_reason": null,
  "error_code": null,
  "error_category": null,
  "event_type": "STRUCT_UPDATE"
}

---

# 4. Structural Invariants

FMRT Core must enforce all structural invariants derived from Flexion Market Theory V3.0. These invariants define the logical boundaries of structural life and ensure that every computed state X(t+1) remains theoretically admissible. If an evolution step would violate any invariant, the core must classify the step as an error, preserve the previous state X(t), and return an invariant violation flag in the output envelope.

The following invariants are mandatory and must hold at every step for κ(t) > 0.

## 4.1 Memory Monotonicity
Memory is strictly non-decreasing:
M(t+1) >= M(t)
This reflects irreversible structural imprint and guarantees that structural time flows forward only. Any violation of monotonicity is forbidden.

## 4.2 Viability Non-Negativity
Viability must never become negative:
Kappa(t+1) >= 0
If Kappa(t+1) == 0, the organism transitions into the DEAD state. If Kappa(t+1) < 0, the step is invalid and must be rejected.

## 4.3 Metric Positivity
When κ > 0, the structural metric must remain non-degenerate:
det g(X(t)) > 0
The core must ensure that no update results in metric collapse unless κ → 0 by theoretical evolution. Any attempt to produce a state with det g <= 0 while κ > 0 must be rejected.

## 4.4 Temporal Density Positivity
Structural time must remain flowing for all living states:
τ(X(t)) > 0  for  κ(t) > 0
Temporal density must collapse only as κ → 0. FMRT must reject any state where τ <= 0 while κ > 0.

## 4.5 Regime Irreversibility
Structural regimes must evolve monotonically through the sequence:
ACC → DEV → REL → COL
Reversal or cyclical transitions are prohibited. If computed derivatives would imply a regime reversal, the step must be rejected.

## 4.6 Continuity of Structural Evolution
For all κ > 0, FMRT must ensure:
- no discontinuities in Δ, Φ, M, κ,
- no geometric tearing,
- no infinite instantaneous jumps in structural fields.

Discontinuities may only occur at κ = 0 (structural collapse).

## 4.7 Finite Structural Quantities
All structural outputs must remain finite:
- Δ(t+1)
- Φ(t+1)
- M(t+1)
- κ(t+1)
- R(X)
- τ(X)
- det g(X)
- viability rate
- collapse metrics

If any quantity becomes non-finite (NaN, Inf), FMRT must reject the step and preserve the previous state.

## 4.8 No Forbidden Domain Entry
FMRT must never produce states in:
- κ < 0  (non-structure)
- det g <= 0 with κ > 0 (geometric invalid state)
- τ <= 0 with κ > 0 (temporal invalid state)

## 4.9 Collapse Consistency
If κ(t+1) == 0:
- det g → 0
- R → ∞
- τ → 0
- regime = COL
- morphology_class = NearCollapse
These relationships must remain consistent; FMRT must not produce contradictory collapse dynamics.

## 4.10 Deterministic Evolution
Given identical X(t) and identical event E(t), FMRT must always produce identical X(t+1) and identical diagnostic outputs, with no dependence on platform, timing, or execution environment.

---

# 5. Minimal Engineering Requirements

The FMRT Core is a theoretical engine implemented in a real, imperfect data environment.  
To preserve correctness, determinism, and structural integrity, the core must satisfy the following **minimal engineering requirements**.  
These requirements are intentionally limited to what is strictly necessary for safe and valid operation.  
All additional engineering concerns belong to the Design and Implementation Documents.

The FMRT Core must not depend on raw market ticks.  
It accepts only normalized structural events (see Section 2), and must remain deterministic, invariant-preserving, and numerically stable under all valid inputs.

---

## 5.1 Determinism Requirement

FMRT must produce **bitwise identical outputs** when:

- the initial structural state X(t) is the same,
- the sequence of structural events is the same,
- the execution platform, timing, and hardware differ.

Forbidden behaviors:
- use of system time inside the core,
- any form of randomness,
- hidden implicit state outside X(t),
- behavior dependent on thread scheduling or CPU timing,
- dependence on the source or identity of events.

FMRT must be a pure deterministic function:
(X(t), Event(t)) → StateEnvelope(t+1)

---

## 5.2 Minimal Filtering Principle

FMRT must not alter, smooth, or suppress real structural evolution.  
Filtering applies **only** to impossible or invalid inputs.

### Allowed filtering:
- removal of non-finite values (NaN, Inf),
- rejecting events with dt <= 0,
- rejecting obviously corrupted deformation inputs,
- rejecting any input that would violate structural invariants.

### Forbidden filtering:
- smoothing real structural transitions,
- inventing missing structural information,
- suppressing legitimate Δ, Φ changes,
- altering event semantics.

FMRT must protect structure from corruption, not rewrite market behavior.

---

## 5.3 Safe Handling of GAP and HEARTBEAT Events

Real data streams may contain interruptions, delays, or periods of no observable movement.  
FMRT must evolve structure safely under these conditions.

### STRUCT_GAP(dt):
FMRT must:
- increase M minimally (irreversible time flow),
- decrease κ minimally (viability decay),
- avoid artificial deformation (Δ, Φ remain stable except for theoretical drift),
- maintain invariant consistency.

### STRUCT_HEARTBEAT(dt):
FMRT must:
- apply zero external deformation,
- evolve M and κ minimally according to theory,
- keep Δ and Φ unchanged unless required internally,
- maintain continuity and determinism.

GAP and HEARTBEAT events must never cause:
- instability,
- invariant violations,
- artificial transitions between regimes or morphologies.

---

## 5.4 Numerical Stability Requirement

FMRT must operate safely under IEEE-754 double precision constraints.

Required behaviors:
- all computations use IEEE-754 double precision,
- no fast-math or non-deterministic optimizations,
- no division by zero,
- no propagation of NaN or Inf into X(t+1),
- checks for invalid metric states (det g <= 0 when κ > 0),
- checks for invalid temporal density (τ <= 0 when κ > 0).

If a numerical hazard is detected:
- FMRT must reject the step,
- preserve X(t),
- return error diagnostics.

---

## 5.5 Invariant Protection Requirement

If applying an event E(t) would cause any structural invariant (Section 4) to fail, FMRT must:

1. Reject the step,
2. Preserve the previous structural state X(t),
3. Set invariant violation flags,
4. Return a valid error envelope.

FMRT must **never** produce:
- κ < 0,
- M_next < M,
- det g <= 0 for κ > 0,
- τ <= 0 for κ > 0,
- regime reversal.

Even in degraded data conditions, invariants take priority over event interpretation.

---

# 6. Error Handling & Fault Tolerance

FMRT Core must remain operational and structurally consistent under all valid and degraded conditions.  
Error handling ensures that the organism’s state is never corrupted and that invariants are preserved.

---

## 6.1 Soft Failure

When a structural event would cause minor inconsistencies or transient invalidity without violating critical invariants:

- FMRT must **reject the step** and return the previous state X(t) unchanged.
- An error flag must be set in the output envelope.
- Diagnostics must describe the reason for rejection.
- No Δ, Φ, M, or κ modifications are allowed unless required to preserve minimal theoretical drift.
- The system must continue to accept subsequent events.

Soft failure applies to:
- invalid event parameters (e.g., dt <= 0, non-finite inputs),
- temporary gaps or spikes that cannot be reconciled,
- minor violations of numerical constraints.

---

## 6.2 Hard Failure (Death State)

When Kappa(t+1) reaches zero (κ = 0):

- FMRT marks the organism as `DEAD`.
- The DEAD state is terminal:
  - Δ, Φ, M, κ, curvature, temporal density, morphology, and regime must remain constant.
  - All subsequent events must **not alter** the organism’s state.
  - Diagnostics must indicate the DEAD state and reason (viability depletion).
- No further evolution is permitted until an explicit `STRUCT_RESET` event occurs.

---

## 6.3 Reset Recovery

When a `STRUCT_RESET(reason)` event is received:

- FMRT must restore the organism to a valid baseline state.
- M, κ, Δ, Φ must be re-initialized in a way that preserves all invariants.
- Temporal and geometric fields are set consistently with the start of a new life cycle.
- Diagnostics must include reset reason and timestamp.
- The reset allows the organism to resume evolution normally.

---

## 6.4 Numerical Hazard Handling

If a computation produces or would produce:

- NaN,
- Inf,
- division by zero,
- negative viability (κ < 0),
- invalid metric (det g <= 0 for κ > 0),
- invalid temporal density (τ <= 0 for κ > 0),

FMRT must:

1. Reject the current step,
2. Preserve the last valid structural state,
3. Set error flags in the output envelope,
4. Log the nature of the numerical hazard for diagnostic purposes.

No numerical hazard may propagate to subsequent computations.

---

## 6.5 Fault Isolation

FMRT must be designed so that:

- any single erroneous event cannot corrupt past or future states,
- invariants serve as protective barriers,
- the organism remains recoverable under the next valid event,
- soft or hard failures do not cause the core to crash or halt unexpectedly.

---

## 6.6 Diagnostic Reporting

FMRT must provide error and fault diagnostics as part of the State Envelope:

- `step_status` ∈ {OK, ERROR, DEAD}
- `error_reason` (optional descriptive text)
- `invariant_flags` indicating which invariants are currently valid or violated

Diagnostics must be purely informative and must not affect the deterministic evolution of X(t+1) beyond rejection of the faulty step.

---

# 7. State Validity Rules

FMRT Core defines a valid structural state as any X(t) that satisfies all theoretical, numerical, and engineering constraints. Validity rules ensure the structural organism remains inside the living domain and that all invariants are preserved.

---

## 7.1 Structural State Definition

A structural state is defined as:

X(t) = (Delta(t), Phi(t), M(t), Kappa(t))

This state is **valid** if and only if:

1. **Viability Constraint**  
   κ(t) > 0 for all living states.  
   κ(t) = 0 represents a terminal DEAD state.

2. **Memory Monotonicity**  
   M(t) ≥ 0 and must be non-decreasing over time.  
   Any decrease in memory violates the monotonicity invariant.

3. **Finite Fields**  
   Δ(t) finite, Φ(t) finite, M(t) finite, κ(t) finite.  
   NaN, Inf, or denormalized values are forbidden.

4. **Metric Positivity**  
   For κ(t) > 0: det g(X(t)) > 0.  
   The structural metric must remain positive-definite to allow valid geometric computations.

5. **Temporal Density**  
   For κ(t) > 0: τ(X(t)) > 0.  
   Structural time flows forward, collapsing only as κ → 0.

6. **Invariant Compliance**  
   All Section 4 invariants must hold:  
   - regime ordering ACC → DEV → REL → COL  
   - continuity of evolution  
   - curvature and morphology consistency

---

## 7.2 DEAD State

A structural state is classified as **DEAD** if:

- κ(t) = 0  
- det g(X(t)) = 0  
- curvature R → ∞  
- temporal density τ → 0

Once the DEAD state is reached:

- All fields must remain constant  
- The organism cannot evolve further until a `STRUCT_RESET` occurs

---

## 7.3 Invalid States

States are **invalid** if any of the following occur:

- κ(t) < 0  
- M(t) decreases  
- det g(X) ≤ 0 while κ > 0  
- τ(X) ≤ 0 while κ > 0  
- Δ, Φ, M, κ are non-finite (NaN/Inf)  
- Regime reversals or discontinuities in structural motion

Invalid states must **never** be returned as X_next.  
FMRT must reject events producing invalid states and preserve the previous valid state.

---

## 7.4 State Transition Rules

1. Only valid events can cause transitions between valid states.  
2. Soft failure events leave the state unchanged, with diagnostic flags.  
3. Hard failure events transition the organism to the DEAD state.  
4. Reset events restore a valid baseline state.  
5. GAP or HEARTBEAT events produce minimal, invariant-consistent changes.

---

## 7.5 Summary

The validity of a structural state is defined by:

- living domain: κ > 0  
- finite, monotonic, and theoretically consistent fields  
- compliance with all invariants  
- DEAD state handled separately  
- invalid states are rejected and trigger error reporting

These rules guarantee FMRT operates strictly inside the theoretical boundaries while maintaining deterministic, safe, and predictable evolution.

---

# 8. Performance Constraints

FMRT Core must operate efficiently and predictably under all valid input conditions.  
Performance constraints ensure that the core can process structural events in real time without violating invariants, determinism, or structural integrity.

---

## 8.1 Event Processing Time

- Each structural event must be processed in **O(1) time complexity**, independent of past history.  
- Total computation time per event must be bounded to allow real-time usage for all supported symbols.  
- No step may block the processing of subsequent events beyond the configured maximum latency.

---

## 8.2 Memory Usage

- FMRT Core must manage memory deterministically and predictably.  
- No dynamic allocation is allowed in the hot path of structural computations.  
- Pre-allocated buffers and stack-only structures must be used where possible.  
- Memory growth must be strictly bounded to prevent overflow or degradation over time.

---

## 8.3 Numerical Precision

- All calculations must use IEEE-754 double-precision arithmetic.  
- Fast-math compiler optimizations are forbidden.  
- Calculations must preserve deterministic outputs across platforms and architectures.  
- Round-off errors must not violate invariants or structural constraints.

---

## 8.4 Deterministic Output

- FMRT Core must guarantee **bitwise identical outputs** for identical inputs, regardless of:

  - execution platform,  
  - number of CPU cores,  
  - operating system,  
  - compiler optimizations (within allowed standards).

- No hidden state, randomness, or external time dependencies may influence the output.

### 8.5 Event Rate Handling — Updated

FMRT Core must handle a continuous stream of structural events without backlog or data loss.  
Maximum supported event rate must be specified in the implementation according to system capabilities.  
TickAdapter is responsible for batching or throttling events when real-world feeds exceed the processing capacity of the core.

**Recommended target parameters for initial implementation:**

- Maximum Δt between processed events: 1 ms  
- Minimum heartbeat interval: 0.5 ms  
- Maximum supported event rate: 1000 events/sec per symbol

---

## 8.6 Fault Tolerance

- Performance must degrade gracefully under abnormal conditions such as GAP or HEARTBEAT events.  
- Even under degraded inputs, FMRT must maintain bounded computation time and avoid deadlocks or infinite loops.

---

## 8.7 Resource Isolation

- FMRT must operate without interfering with external systems or other cores.  
- CPU, memory, and I/O usage must be predictable and bounded.

---

## 8.8 Summary

FMRT Core must be:

- fast and predictable (O(1) per event),  
- memory safe and deterministic,  
- numerically precise and invariant-preserving,  
- able to handle high-frequency event streams,  
- robust under degraded conditions,  
- isolated from external resource interference.

These constraints guarantee reliable and deterministic real-time operation of the structural organism.

---

# 9. Forbidden Behaviors

FMRT Core must never perform operations that compromise determinism, invariants, structural integrity, or theoretical consistency. The following behaviors are explicitly forbidden and must be prevented in all implementations.

---

## 9.1 Dependence on External Time or Clocks

- FMRT must not use system clocks, wall-time, or any external temporal source to influence X(t+1).  
- All temporal evolution is derived solely from the internal memory field M(t) and structural events.

---

## 9.2 Use of Randomness

- FMRT must not use any form of randomness, pseudo-random number generation, or stochastic processes.  
- Deterministic outputs are mandatory for identical inputs across all executions.

---

## 9.3 Hidden or Global State

- FMRT must not rely on hidden state variables outside the structural state X(t).  
- No mutable global variables may influence the core’s computations.

---

## 9.4 Direct Dependence on Raw Market Data

- FMRT must never read, process, or respond to raw market ticks, prices, volumes, or order flow directly.  
- Only normalized structural events may enter the core.  
- No filtering, smoothing, or interpretation of raw ticks may occur inside the core.

---

## 9.5 Violation of Invariants

- FMRT must never produce X(t+1) that violates any invariant defined in Section 4, including:  
  - Memory monotonicity  
  - Viability non-negativity  
  - Metric positivity  
  - Temporal density positivity  
  - Regime irreversibility  
  - Continuity of structural evolution

---

## 9.6 Arbitrary Structural Modification

- FMRT must not artificially modify Δ, Φ, M, or κ beyond what is dictated by structural events and theoretical evolution.  
- No smoothing, averaging, or extrapolation may alter the true evolution of the organism.  
- No “corrective” modifications based on external projections are allowed.

---

## 9.7 Non-deterministic Floating Point Behavior

- FMRT must not use compiler flags or instructions that introduce non-deterministic floating-point results.  
- Fast-math optimizations and non-standard rounding modes are forbidden.

---

## 9.8 Multi-threading Violations

- FMRT must not allow concurrent modifications of X(t) from multiple threads without proper synchronization.  
- Structural state updates must remain atomic and deterministic.  
- Race conditions or parallel execution that compromises determinism are prohibited.

---

## 9.9 Structural Collapse Misrepresentation

- FMRT must not report collapse or DEAD state incorrectly.  
- The DEAD state must only be entered when κ = 0.  
- No step may artificially trigger or delay collapse.

---

## 9.10 Summary

FMRT Core must avoid all operations that:

- compromise theoretical correctness,  
- violate invariants,  
- introduce non-determinism,  
- depend on external factors,  
- or artificially modify structure.

Strict adherence to these forbidden behaviors ensures the core remains a reliable, deterministic, and theory-compliant structural engine.