# FMRT-Implementation-Document-V2.2

---

# 0. Overview / Purpose

The FMRT Implementation Document V2.2 defines the complete and exact technical plan for implementing FMRT Core in full compliance with:

- Flexion Market Theory (FMT) V3.1  
- FMRT Specification V2.2  
- FMRT Design Document V2.2  

This document transforms the abstract architecture and theoretical rules into concrete, deterministic, invariant-preserving implementation steps. It specifies exact data structures, algorithms, validation rules, event-handling flows, numerical requirements, and module-level responsibilities needed to build a correct FMRT Core.

FMRT Core must be implemented exactly as described here.  
No deviation, heuristic shortcut, approximation, optimization, or interpretation is allowed unless it fully preserves:

- structural invariants  
- deterministic behavior  
- numerical safety  
- architectural isolation  
- collapse geometry correctness  
- forbidden-domain constraints  

FMRT is a **pure structural evolution engine**.  
It does **not** read raw market data, time, or external context.  
It processes only two inputs:

1. Structural State X(t)  
2. Canonical Structural Event E(t)

And produces exactly one output:

• StateEnvelope(t+1)

FMRT has no side inputs, no hidden state, no randomness, and no dependence on system time.

---

## 0.1 Purpose

The goals of this document are to:

- Provide exact implementation rules for all FMRT modules.  
- Define strict data structures for structural state, events, derived metrics, invariants, and diagnostics.  
- Convert all theoretical update rules of Δ, Φ, M, κ from FMT 3.1 into concrete algorithms.  
- Specify deterministic computation of curvature, temporal density τ, morphology μ, regime, and collapse metrics.  
- Define invariant-checking procedures with precise rejection conditions.  
- Describe event-handling logic for UPDATE, GAP, HEARTBEAT, RESET.  
- Establish implementation-level rules for numerical stability, NaN/Inf protection, O(1) performance, and deterministic floating-point execution.  
- Provide the internal API for module integration and the public API for SDKs.  
- Describe error-handling behavior, including soft failures, hard failures, and collapse transitions.  
- Define the complete set of implementation-level tests required for FMRT compliance.

This document ensures a correct and reproducible implementation across all platforms, compilers, and hardware architectures.

---

## 0.2 Scope

This document covers:

- Implementation of internal FMRT modules:
  - Event Handler  
  - Core Evolution Engine  
  - Invariant Validator  
  - Diagnostics Layer  

- Exact computational rules for:
  - Δ evolution  
  - Φ evolution  
  - memory update M  
  - viability update κ  
  - curvature R computation  
  - metric determinant det g  
  - temporal density τ  
  - morphology index μ  
  - dynamic regime transitions  
  - collapse geometry  

- Deterministic error-handling and forbidden-domain protection.  
- Data structure definitions for all FMRT internal and external types.  
- API signatures and module-level interaction flows.  
- Numerical safety rules ensuring no NaN/Inf/overflow propagation.  
- Testing guidelines covering determinism, invariants, collapse, RESET, and stress testing.  

This document does **not** include:
- theoretical derivations (Specification V2.2),  
- high-level architecture diagrams (Design Document V2.2),  
- platform-specific optimizations (deployment layer).

---

## 0.3 Implementation Goals

FMRT Core must satisfy the following implementation goals:

### 1. **Determinism**
Identical inputs must yield identical bitwise outputs across:
- CPUs  
- operating systems  
- compiler versions  
- runtime schedules  

FMRT must not depend on:
- system time  
- randomness  
- global mutable state  
- non-deterministic floating-point optimizations  

### 2. **Invariant Preservation**
All invariants from FMT 3.1 must be enforced by the implementation:
- memory monotonicity  
- viability non-negativity  
- metric positivity  
- temporal density positivity  
- regime irreversibility  
- morphology bounds  
- collapse geometry behavior  
- forbidden-domain protection  

### 3. **Numerical Safety**
Implementation must guarantee:
- no NaN/Inf generated or propagated  
- no division by zero  
- no unstable exponentials or catastrophic cancellation  
- deterministic floating-point ordering  

### 4. **Modular Isolation**
Each module must:
- have a single responsibility  
- not access internal state of other modules  
- communicate via immutable, type-safe interfaces  

### 5. **O(1) Performance**
Every event must execute in constant time:
- no dynamic allocation  
- no resizing arrays  
- no history storage  
- no iterative searches  

### 6. **Atomic State Evolution**
A step is either fully applied (valid) or fully rejected (invalid).  
No partial updates are ever permitted.

### 7. **Collapse Stability**
When κ reaches zero, the organism must enter DEAD state:
- all fields freeze  
- invariants remain satisfied  
- no evolution occurs until RESET  

### 8. **RESET Determinism**
RESET must always produce a valid, minimal, fully consistent baseline organism.

---

## 0.4 Implementation Philosophy

FMRT Core implementation must follow these principles:

- **Clarity:** algorithms must be explicit and unambiguous.  
- **Predictability:** no emergent or dynamic behavior.  
- **Safety:** invariants and domain constraints must dominate all decisions.  
- **Purity:** no interaction with external systems beyond receiving events and returning results.  
- **Faithfulness:** implementation must match theory exactly.  
- **Minimality:** no additional computations, no added fields, no heuristics, no machine learning.  
- **Robustness:** events cannot corrupt state; errors cannot propagate.  

FMRT is a structural interpreter of FMT 3.1 — nothing more, nothing less.

---

## 0.5 Output of This Document

After reading this Implementation Document, an engineer must be able to:

- Implement FMRT Core from scratch.  
- Build fully deterministic, invariant-preserving evolution functions.  
- Construct correct internal and external data structures.  
- Implement exact algorithms of Δ/Φ/M/κ evolution.  
- Implement curvature, τ, morphology, and regime computations.  
- Implement strict invariant validation and rejection logic.  
- Implement deterministic diagnostics and error handling.  
- Build the FMRT public API for SDK integration.  
- Construct a complete test suite guaranteeing compliance.

This document is the **authoritative implementation guide** for FMRT Core V2.2.

---

# 1. Module Implementations

FMRT Core V2.2 consists of exactly four internal modules:  
1) Event Handler  
2) Core Evolution Engine  
3) Invariant Validator  
4) Diagnostics Layer  

There is no Tick Adapter, no Event Sequencer, no raw market processing.  
FMRT receives only canonical structural events and evolves the organism X(t) deterministically.

---

## 1.1 Event Handler — Implementation

Purpose:  
Validate and canonicalize incoming structural events before they reach the evolution engine.  
The Event Handler does NOT modify X(t), does NOT interpret markets, does NOT apply any theory.  
It is a pure input validator.

Input:  
E_raw — event from SDK or external system.

Output:  
E_canonical — validated event in canonical internal format.

Allowed event types:  
- STRUCT_UPDATE  
- STRUCT_GAP  
- STRUCT_HEARTBEAT  
- STRUCT_RESET  

Validation Rules:
- event_type must be exactly one of the allowed types.  
- UPDATE / GAP / HEARTBEAT require: dt > 0 AND dt is finite.  
- RESET must NOT contain dt.  
- All numeric fields must be valid IEEE-754 doubles without NaN/Inf/denormals.  
- stimulus (if present) must be finite.

Canonical format:
StructEvent {
    event_type;
    dt?;            // absent for RESET
    stimulus?;      // deformation input, only for UPDATE
    reason?;        // only for RESET
}

If validation fails:
- return ERROR event structure, X(t) must NOT be evolved.
- Diagnostics Layer marks Input Error.

The Event Handler contains ZERO structural logic.

---

## 1.2 Core Evolution Engine — Implementation

Purpose:  
Implements the structural evolution operator:

X(t+1) = I( X(t), E(t) )

according to FMT 3.1 and FMRT Specification V2.2.

Inputs:  
- X_current — last valid state.  
- E_canonical — validated event.

Outputs:  
- X_provisional — candidate next state.  
- DerivedMetrics — curvature R, det g, τ, μ, regime, collapse flags.

Responsibilities:
1. Deterministically evolve Δ, Φ, M, κ.  
2. Compute curvature R, metric determinant det g, temporal density τ.  
3. Compute morphology index μ and class.  
4. Compute dynamic regime ACC→DEV→REL→COL.  
5. Apply event-specific logic.  
6. Produce finite state or intentionally reach κ=0 for collapse.  
7. NOT enforce invariants (Validator handles that).

Implementation Rules:

### (1) Δ update
Delta_next[i] =
    Delta[i]
  + deformation_contrib(E)
  + internal_relaxation[i] * dt

deformation_contrib = 0 for GAP/HEARTBEAT.  
Relaxation depends on morphology and regime.  
All results must be finite.

### (2) Φ update
Phi_next = Phi
           + tension_accumulation
           - relaxation_factor * dt

Phi_next must remain ≥ 0.

### (3) Memory update
M_next = M + dt * τ  
M must always increase (monotonic).

### (4) Viability update
kappa_next = max( 0 , κ - dt * decay_factor )

Where decay_factor depends on:
- curvature,
- tension Φ,
- morphology μ,
- structural load.

If kappa_next = 0 → collapse state begins.

### (5) Curvature & Metric
The engine must compute curvature_scalar R and metric determinant det g from Δ, Φ, M, κ using stable formulas.

Must satisfy:
- det g > 0 when κ > 0.  
- det g → 0 and R → ∞ as κ → 0 (collapse geometry).  

### (6) Temporal Density τ
tau = tau_min + tau_scale * exp( -lambda_kappa * κ )

τ > 0 for all κ > 0, τ → 0 as κ → 0.

### (7) Morphology μ and class
Compute μ ∈ [0,1] from curvature intensity.  
Class = Elastic / Plastic / Degenerate / NearCollapse based on μ and κ.

### (8) Regime (ACC→DEV→REL→COL)
Regime must advance irreversibly.  
Backward transitions forbidden.

### (9) Event-Type Logic
STRUCT_UPDATE: full evolution (Δ, Φ, M, κ).  
STRUCT_GAP: Δ,Φ unchanged; minimal memory/viability evolution.  
STRUCT_HEARTBEAT: same as GAP but with minimal dt.  
STRUCT_RESET: build new baseline organism:
    Δ = 0  
    Φ = default_reset_phi  
    M = 0  
    κ = default_reset_kappa  
    regime = ACC  
    morphology = Elastic  

### Engine Constraints
- Must run in O(1).  
- No allocations.  
- No external time, randomness, or I/O.  
- No shared mutable state.  
- Arithmetic must be deterministic.

---

## 1.3 Invariant Validator — Implementation

Purpose:  
Validate X_provisional and ensure invariants defined in FMT 3.1 hold.  
If ANY invariant fails → REJECT event (X(t+1) = X(t)).

Inputs:  
- X_current  
- X_provisional  
- DerivedMetrics

Output:  
- ValidatedState (if accepted)  
- REJECT (if failed)

Invariants enforced:

### (1) Memory monotonicity
M_next >= M_current

### (2) Viability non-negativity
kappa_next >= 0

### (3) Metric positivity
If κ > 0 then det g > 0.

### (4) Temporal density positivity
If κ > 0 then τ > 0.

### (5) Regime irreversibility
regime_next >= regime_current.

### (6) Morphology bounds
0 ≤ μ ≤ 1.

### (7) Collapse consistency
If κ_next = 0:
- det g = 0  
- τ = 0  
- μ = 1  
- regime = COL  
- curvature diverges appropriately  

### (8) Forbidden domain rejection
Reject if ANY field is:
- NaN  
- Inf  
- Undefined  
- det g ≤ 0 while κ > 0  
- τ ≤ 0 while κ > 0  
- κ < 0 (invalid)

If ANY invariant fails → reject state and preserve X_current.

---

## 1.4 Diagnostics Layer — Implementation

Purpose:  
Assemble the final StateEnvelope(t+1) after validation.

Inputs:  
- Either ValidatedState OR REJECT decision  
- Event metadata  
- Invariant flags

Output:  
A deterministic StateEnvelope containing all structural fields, derived metrics, collapse indicators, invariant flags, and diagnostics.

StateEnvelope fields include:
- Δ, Φ, M, κ  
- curvature R  
- det g  
- τ  
- μ  
- morphology class  
- dynamic regime  
- collapse flags  

Diagnostics include:
- step_status: OK / ERROR / DEAD  
- error_code  
- error_category  
- error_reason  
- event_type  
- invariant flags  
- rejection flags if applicable  

Rules:
- Diagnostics NEVER modify structural fields.  
- Envelope always contains complete valid data.  
- Must be deterministic under all conditions.

---

## 1.5 Module Integration

FMRT must always process events in this order:

1. Event Handler → validate event  
2. Core Evolution Engine → compute X_provisional  
3. Invariant Validator → accept or reject  
4. Diagnostics Layer → produce final StateEnvelope  

Rules:
- No module may bypass another.  
- No module may share mutable memory.  
- No module may reorder or skip processing steps.  
- ALL transitions must be deterministic.

---

## 1.6 Summary

This section defines implementation-level behavior of all modules:

- Event Handler: pure validation, no structural logic.  
- Core Engine: computes deterministic structural evolution.  
- Invariant Validator: absolute enforcement of theoretical invariants.  
- Diagnostics Layer: generates complete, immutable StateEnvelope.

These four modules form the full FMRT Core implementation in V2.2.

---

# 2. Data Structures

FMRT V2.2 defines a closed, fully deterministic set of data structures required for implementing structural state X(t), canonical structural events, derived metrics, invariants, diagnostics, and the final output envelope.  
All structures must be finite, immutable in transit, and free of hidden or platform-dependent fields.  
No raw market data, timestamps, volumes, or external context appear anywhere inside FMRT Core.

FMRT processes only:
1) StructuralState X(t)  
2) StructEvent E(t)  
And outputs a single object:
3) StateEnvelope(t+1)

All structures below are mandatory.

---

## 2.1 Structural State (X)

The structural organism state is:

StructuralState {
    Delta[MAX_DIM];     // structural differentiation vector
    Phi;                // structural tension
    M;                  // structural memory (monotonic)
    Kappa;              // viability (κ > 0 for life, κ = 0 → DEAD)
}

Constraints:
- All values must be IEEE-754 double finite numbers.
- No NaN, no Inf, no denormals.
- Kappa > 0 → living organism.
- Kappa = 0 → DEAD organism; fields must remain frozen.
- M is strictly non-decreasing.
- Delta has fixed dimension and must not be dynamically resized.

The state must be stored in a compact, contiguous memory layout to ensure O(1) access.

---

## 2.2 Canonical Structural Event (E)

FMRT accepts only four event types:

StructEvent {
    event_type;     // UPDATE | GAP | HEARTBEAT | RESET
    dt?;            // strictly positive for UPDATE, GAP, HEARTBEAT; absent for RESET
    stimulus?;      // optional deformation input for UPDATE only
    reason?;        // RESET explanation text, optional
}

Event constraints:
- dt must be strictly > 0 for non-reset events.
- RESET must not contain dt.
- stimulus must be finite if present.
- No NaN, Inf, or invalid fields.

Events must be immutable after creation.

---

## 2.3 Derived Structural Metrics

These metrics are computed by the Core Evolution Engine from X_provisional.  
They **are not part of the actual structural state**, but are needed for invariants, diagnostics, and collapse geometry.

DerivedMetrics {
    curvature_R;           // scalar curvature
    det_g;                 // metric determinant (>0 for κ>0, =0 for collapse)
    tau;                   // temporal density (strictly >0 for κ>0)
    morphology_index;      // μ ∈ [0,1]
    morphology_class;      // Elastic | Plastic | Degenerate | NearCollapse
    regime;                // ACC | DEV | REL | COL
    collapse_flag;         // true if κ == 0
    collapse_distance;     // = κ
    collapse_speed;        // ||Δ_next - Δ|| / dt
    collapse_intensity;    // curvature_R scaled
}

All fields must be finite except in exact collapse geometry, where the divergence of curvature is symbolic but represented through μ = 1, det_g = 0, τ = 0, regime = COL.

---

## 2.4 Invariant Flags

Invariants reflect whether X_provisional satisfies theoretical constraints.

InvariantFlags {
    memory_monotonic;        // M_next ≥ M
    viability_nonnegative;   // κ ≥ 0
    metric_positive;         // det g > 0 for κ > 0
    time_density_positive;   // τ > 0 for κ > 0
    regime_consistent;       // no backward transitions
    morphology_valid;        // μ ∈ [0,1]
    collapse_consistent;     // geometry valid if κ=0
    forbidden_domain_ok;     // no NaN/Inf, no invalid geometry
    all_ok;                  // true if ALL invariants are satisfied
}

Invariant flags must be computed deterministically.

---

## 2.5 Diagnostics

Diagnostics provide metadata describing event processing outcome.  
They do NOT affect structural state.

Diagnostics {
    step_status;        // OK | ERROR | DEAD
    error_code;         // stable numeric identifier
    error_category;     // Input | Numerical | Invariant | Event | None
    error_reason;       // short descriptive text
    event_type;         // origin event
}

Rules:
- Diagnostics must ALWAYS be emitted.
- Never modify X_next.
- Never contain randomness.
- Must be identical across all platforms for same input.

---

## 2.6 StateEnvelope (FMRT Output)

This is the only output structure FMRT Core returns to the outside world.

StateEnvelope {
    X_next;             // accepted structural state
    DerivedMetrics;     // full set of secondary computed metrics
    InvariantFlags;     // invariant evaluation result
    Diagnostics;        // error information
    event_type;         // event that produced this envelope
}

Requirements:
- Must be complete and deterministic.
- Must include all fields, even if unchanged.
- Must NEVER contain NaN or Inf (unless collapse geometry demands symbolic representation via zeros and ones as specified).
- Must reflect either accepted evolution or a rejected step with X_next = X_current.

---

## 2.7 Internal Scratch Buffers

FMRT may use fixed-size internal buffers:

InternalBuffers {
    prev_state;         // copy of X(t) for rejection fallback
    work_state;         // provisional X(t+1)
    scratch_real[16];   // for curvature, norms, exponentials, etc.
}

Rules:
- MUST NOT dynamically allocate memory.
- MUST NOT resize arrays.
- MUST be used only inside one event step.
- MUST NOT remain exposed outside the module.

---

## 2.8 Data Structure Integrity Guarantees

All data structures must obey the following constraints:

1. **No hidden state**  
   Structures contain only explicit fields.

2. **Deterministic layout**  
   Fields must appear in consistent order across platforms.

3. **Finite numeric values**  
   Except collapse geometry limits (det_g=0, τ=0), no infinities or NaNs allowed.

4. **Fixed memory footprint**  
   No dynamic growth of arrays or containers.

5. **Atomic updates**  
   X_next replaces X_current only after invariants pass.

6. **Zero external dependencies**  
   Structures contain no timestamps, clocks, OS handles, or market data.

---

## 2.9 Summary

FMRT V2.2 uses a tightly-defined set of deterministic data structures:

- **StructuralState** holds the organism X(t).  
- **StructEvent** is the only input to FMRT.  
- **DerivedMetrics** supplement X_provisional with analytic measurements.  
- **InvariantFlags** enforce structural correctness.  
- **Diagnostics** describe evaluation outcomes.  
- **StateEnvelope** presents the final, complete, deterministic result.

These structures guarantee purity, safety, determinism, and invariant integrity across all FMRT implementations.

---

# 3. Algorithms and Computation

This section defines the exact deterministic algorithms used to compute structural evolution, derived metrics, collapse geometry, and all supporting intermediate quantities.  
Every computation must follow these rules:

- no randomness  
- no branching that depends on platform-specific behavior  
- no dynamic memory allocation  
- constant-time execution (O(1))  
- no external data (market, time, I/O)  
- strict IEEE-754 deterministic arithmetic  
- no NaN, no Inf allowed at any point  

The algorithms below are mandatory for all FMRT Core implementations.

---

## 3.1 Overview of the Evolution Step

FMRT step consists of:

1) X_provisional = Evolve(X_current, E)  
2) InvariantValidator(X_current, X_provisional)  
3) If all invariants pass → X_next = X_provisional  
4) Else → X_next = X_current (reject)  
5) Produce StateEnvelope

The evolution operator is defined by the equations in this section.

---

## 3.2 Δ Update Algorithm (Structural Differentiation)

Δ is an N-dimensional structural differentiation vector.

General update rule:

Delta_next[i]  
= Delta[i]  
+ deformation_contrib[i]  
+ relaxation_term[i] * dt

### 1. deformation_contrib[i]
For UPDATE events: supplied by the event’s structural stimulus.  
For GAP / HEARTBEAT / RESET:  
deformation_contrib[i] = 0

### 2. relaxation_term[i]
Represents autonomous structural settling:

relaxation_term[i] = -lambda_relax * Delta[i] * (1 - μ)

Where:
- lambda_relax = constant relaxation coefficient  
- μ = morphology index ∈ [0,1]  

Higher μ (more degenerate) → less relaxation.

### Final Constraint
Delta_next[i] must be finite for all i.

---

## 3.3 Φ Update Algorithm (Structural Tension)

Φ evolves according to:

Phi_next = Phi
           + A * deformation_intensity
           - B * dt

Where:

- deformation_intensity = ||Delta_next - Delta||  
- A = tension amplification coefficient  
- B = tension decay coefficient

Constraints:
- Phi_next ≥ 0  
- Phi_next must be finite

For GAP or HEARTBEAT → deformation_intensity = 0.

For RESET → Phi_next = Phi_reset_default.

---

## 3.4 Memory Update M

Memory accumulates temporal density:

M_next = M + dt * τ

Rules:
- M_next must be ≥ M  
- M_next must remain finite  
- No other term may decrease memory

RESET sets:
M_next = 0.

---

## 3.5 Viability Update κ

Viability decreases according to:

kappa_next = κ - dt * D

Where decay factor D is:

D = a1 * curvature_R
  + a2 * Phi
  + a3 * μ
  + a4

The constants a1, a2, a3, a4 are strictly positive.

Final viability value:

kappa_next = max(0, kappa_next)

If κ reaches 0 → collapse.

---

## 3.6 Curvature Computation R

Curvature captures structural deformation intensity.  
A stable approximation for scalar curvature is:

curvature_R =
      alpha1 * ||Delta||^2
    + alpha2 * Phi
    + alpha3 * M / (1 + κ)

Where α1, α2, α3 are fixed positive coefficients.

Constraints:
- curvature_R must be finite  
- If κ → 0 → curvature_R grows toward collapse indicator  
- curvature_R must NOT produce Inf

If κ == 0 → do NOT compute curvature; use collapse geometry instead.

---

## 3.7 Metric Determinant det g

The structural metric g has determinant:

det_g = max( epsilon,
             c1 * exp( -c2 * curvature_R ) * κ )

Where:
- epsilon = extremely small positive constant to prevent floating underflow  
- c1, c2 > 0 coefficients

For κ == 0:
det_g = 0

Constraints:
- det_g > 0 for κ > 0
- det_g = 0 for collapse
- det_g must NOT be negative or Inf or NaN

---

## 3.8 Temporal Density τ

Temporal density shrinks as organism approaches collapse:

tau = tau_min + tau_scale * exp( -lambda_k * κ )

Parameters:
- tau_min > 0  
- tau_scale > 0  
- lambda_k > 0  

Constraints:
- τ > 0 for all κ > 0  
- τ → 0 as κ → 0  
- τ must be finite  

For κ == 0:
tau = 0

---

## 3.9 Morphology Index μ

Morphology measures “structural deformation maturity.”

Formula:

μ = clamp(
        ( curvature_R / (curvature_R + beta) ),
        0,
        1
    )

Where beta > 0 ensures stability.

Morphology class:
- μ < 0.25 → Elastic  
- 0.25 ≤ μ < 0.50 → Plastic  
- 0.50 ≤ μ < 0.75 → Degenerate  
- μ ≥ 0.75 → NearCollapse  

Constraints:
- μ must remain finite  
- μ ∈ [0,1]

For κ == 0:
μ = 1.

---

## 3.10 Regime Evolution

Regime must evolve irreversibly in order:

ACC → DEV → REL → COL

Rule:

If μ < 0.25:
    regime_next = ACC
Else if μ < 0.50:
    regime_next = DEV
Else if μ < 0.75:
    regime_next = REL
Else:
    regime_next = COL

Additionally:
regime_next = max( regime_current, regime_next )

This prevents backward transitions.

If κ == 0 → regime_next = COL.

---

## 3.11 Collapse Geometry

Collapse is defined by κ == 0.

When κ hits zero:

- kappa_next = 0  
- det_g = 0  
- τ = 0  
- μ = 1  
- regime = COL  
- Delta, Φ, M freeze (no further evolution)  
- curvature_R is not computed; collapse_intensity derived from last stable R  

Collapse constraints:
- No UPDATE, GAP, HEARTBEAT may modify X_next after collapse  
- Only RESET can exit collapse state  

---

## 3.12 RESET Algorithm

RESET creates a new minimal organism:

Delta[i] = 0  
Phi = Phi_reset_default  
M = 0  
Kappa = Kappa_reset_default  
regime = ACC  
μ = 0  
det_g = positive baseline value  
τ = tau_min baseline

Constraints:
- RESET must produce a valid, invariant-satisfying state  
- RESET must run in O(1)  
- No dynamic memory  

---

## 3.13 Forbidden Computation Behaviors

The implementation must NEVER:

- rely on floating-point nondeterminism (fast-math, fused ops)  
- reorder arithmetic in a nondeterministic way  
- use iterative refinement or matrix inversions  
- accumulate history  
- allocate memory during step  
- produce NaN/Inf in intermediate calculations  
- perform loops depending on data size  
- use external data or timestamps  

Any such behavior violates the FMRT model.

---

## 3.14 Summary

Algorithms in FMRT V2.2 must be:

- deterministic  
- finite  
- invariant-compliant  
- collapse-consistent  
- O(1)  
- free of randomness  
- platform-independent  
- mathematically aligned with FMT 3.1  

These computation rules form the core executable logic of the FMRT system.

---

# 4. Numerical Stability Rules

FMRT Core V2.2 must guarantee full numerical stability and deterministic behavior across all platforms, CPUs, compilers, and operating systems. All computations must remain strictly within the allowed numeric domain. No NaN, no Inf, no denormalized values, no underflow/overflow leading to invalid structural states are permitted.

This section specifies mandatory rules for implementing safe, stable, invariant-preserving numerical computations.

---

## 4.1 IEEE-754 Strict Mode Requirement

FMRT must use deterministic IEEE-754 double precision arithmetic with the following constraints:

- No fast-math  
- No fused multiply-add unless bitwise identical across platforms  
- No architecture-dependent optimizations  
- No extended precision floating rules  
- No relaxed FP contraction  
- No nondeterministic rounding modes  

Floating-point behavior must be fully reproducible.

---

## 4.2 NaN / Inf / Undefined Value Protections

At every computation stage:

- If any Δ, Φ, M, κ become NaN → reject event  
- If any Δ, Φ, M, κ become Inf → reject event  
- If any derived metric becomes NaN/Inf → reject event  
- If underflow produces a denormal (subnormal), value must be clamped to 0  

The system must NEVER allow NaN or Inf to propagate into the structural state X(t+1).

---

## 4.3 Safe Division Rules

FMRT must never perform division by zero or near-zero.  
All divisions must satisfy:

|denominator| ≥ epsilon_div

Where epsilon_div is a small constant (e.g., 1e-14).

If a denominator violates this rule:
- substitute stable fallback expression  
- or reject the step based on forbidden-domain criteria  

Under no circumstance may division by zero produce Inf or NaN.

---

## 4.4 Overflow Protections

Any exponentials, powers, or multiplications must be checked for overflow thresholds.  
FMRT must never allow:

exp(x) → Inf  
pow(x,y) → Inf  
multiplied values → Inf  

If an overflow risk is detected:
- replace with maximum safe finite representable value  
- OR reject the step if this violates the geometric domain  

Overflow must NEVER propagate.

---

## 4.5 Underflow Protections

Underflow during metric computation (e.g., small det g) must be stabilized:

det_g = max(det_g, epsilon_metric)

Where epsilon_metric is a small positive constant ensuring metric positivity.

Underflow in global collapse region (κ=0):
- det_g = 0 explicitly  
- τ = 0 explicitly  
- μ = 1 explicitly  

No intermediate negative or tiny-denormal values are allowed.

---

## 4.6 Stable Exponential Computation

All exponentials must follow:

safe_exp(x):
    if x < exp_lower_bound:
         return exp_min_value
    if x > exp_upper_bound:
         return exp_max_value
    return exp(x)

exp_lower_bound and exp_upper_bound are predefined safe FP limits.

Exponential growth or decay near collapse must not produce NaN or Inf.

---

## 4.7 Stable Norm Computation

Vector norms must use numerically stable formulas:

||Δ|| = sqrt( Σ_i (Δ[i] * Δ[i]) )

Rules:
- must use fused operations only if deterministic  
- partial sums must not overflow  
- must clamp exceedingly small values to 0  

No iterative norm refinements allowed.

---

## 4.8 Kappa and Collapse Stability

When κ → 0, the algorithm must smoothly transition into collapse geometry:

If kappa_next <= epsilon_k:
    kappa_next = 0
    det_g = 0
    tau = 0
    mu = 1
    regime = COL

Where epsilon_k is a small threshold ensuring stable collapse detection.

After collapse:
- X must stop evolving except via RESET  
- All derived metrics must remain finite and defined  
- Curvature divergence must be symbolic, not numeric Inf  

---

## 4.9 Monotonic Memory Enforcement

M_next = M + dt * τ

Since τ ≥ 0:
- M_next must be strictly >= M  
- If floating-point rounding produces M_next < M, clamp:  
  M_next = M

Memory must NEVER decrease due to numeric error.

---

## 4.10 Deterministic Clamping Rules

Clamping must be deterministic and symmetric:

clamp(x, xmin, xmax):
    if x < xmin: return xmin
    if x > xmax: return xmax
    return x

Used for:
- Φ ≥ 0  
- μ ∈ [0,1]  
- regime stabilization  
- det_g lower bound  
- preventing small negative values from rounding errors  

---

## 4.11 Forbidden Numeric Behaviors

The implementation must NEVER:

- rely on implicit FP state  
- use random jitter to stabilize calculations  
- use iterative convergence loops  
- depend on FP exceptions  
- allow compiler to reorder FP operations nondeterministically  
- use reduced-precision intermediates  
- accumulate rounding drift through incremental loops  
- compute metrics via subtraction of nearly equal numbers (catastrophic cancellation)  
- use NaN payloads or Inf as internal signals  

These behaviors violate FMRT determinism.

---

## 4.12 Deterministic Ordering of Computations

All calculations within a step must follow a fixed strict order:

1) Compute Δ_next  
2) Compute Φ_next  
3) Compute τ  
4) Compute M_next  
5) Compute κ_next  
6) Compute curvature_R  
7) Compute det_g  
8) Compute μ  
9) Compute morphology class  
10) Compute regime  

Reordering is forbidden as it may produce platform-dependent rounding differences.

---

## 4.13 Enforcement of O(1) Computation

Every numerical operation in FMRT must satisfy:

- constant-time execution  
- fixed number of operations  
- no loops depending on data  
- no recursion  
- no adaptive steps  

This ensures:
- predictable latency  
- deterministic FP ordering  
- identical results across platforms  

---

## 4.14 Summary

FMRT V2.2 numerical rules guarantee that:

- all computations are stable  
- no NaN/Inf ever enters state  
- collapse is handled safely  
- invariants cannot be broken by FP drift  
- deterministic ordering prevents platform variance  
- memory, viability, metrics, and morphology remain mathematically well-defined  

Numerical discipline is foundational to FMRT correctness and MUST be followed exactly by all implementations.

---

# 5. Event Handling Algorithms

This section defines the complete deterministic logic for processing all structural event types in FMRT Core.  
FMRT V2.2 accepts exactly four event types:

1) STRUCT_UPDATE  
2) STRUCT_GAP  
3) STRUCT_HEARTBEAT  
4) STRUCT_RESET  

No other events exist, and no implicit events may be generated by the system.  
Every event must pass through the Event Handler → Evolution Engine → Invariant Validator → Diagnostics Layer pipeline.

The rules below define EXACTLY how each event type is processed.  
No deviation, no heuristics, no data-driven behavior, and no external interpretation is permitted.

---

## 5.1 Common Event Processing Pipeline

Every event E(t) follows the same global algorithm:

Step 1 — Event Handler validates event shape  
Step 2 — Core Engine computes X_provisional  
Step 3 — Invariant Validator ensures X_provisional is legal  
Step 4 — Diagnostics Layer emits StateEnvelope  

This section describes Step 2 for each event type.

---

## 5.2 STRUCT_UPDATE — Full Evolution Event

STRUCT_UPDATE is the primary event type responsible for meaningful structural evolution.

### 5.2.1 Preconditions
Event must contain:
- dt > 0  
- finite stimulus vector (may be low or high intensity)  
- no NaN/Inf  

### 5.2.2 Algorithm
Compute:

1) Δ_next  
Delta_next[i] = Delta[i]
               + stimulus[i]
               + (-lambda_relax * Delta[i] * (1 - μ)) * dt

2) Φ_next  
Phi_next = Phi  
           + A * ||stimulus||  
           - B * dt  
Clamp:
Phi_next = max(Phi_next, 0)

3) τ (temporal density)  
tau = tau_min + tau_scale * exp(-lambda_k * κ)

4) M_next  
M_next = M + dt * tau

5) κ_next  
kappa_next = κ - dt * (a1*curvature_R + a2*Phi + a3*μ + a4)  
Clamp:
kappa_next = max(kappa_next, 0)

6) Derived metrics (computed after Δ, Φ, M, κ)
- curvature_R  
- det_g  
- μ  
- morphology_class  
- regime  

7) Collapse handling  
If kappa_next = 0 → force collapse geometry:
- det_g = 0  
- tau = 0  
- μ = 1  
- regime = COL  

STRUCT_UPDATE event produces the fullest deformation of the organism.

---

## 5.3 STRUCT_GAP — Missing Data / No Deformation

STRUCT_GAP models periods where no structural input is available.

### 5.3.1 Preconditions
Event must contain dt > 0.  
No stimulus is provided.

### 5.3.2 Algorithm
1) Δ_next  
Delta_next[i] = Delta[i]  
(no deformation for GAP)

2) Φ_next  
Phi_next = Phi - B * dt  
Phi_next = max(Phi_next, 0)

3) τ  
tau computed normally:
tau = tau_min + tau_scale * exp(-lambda_k * κ)

4) M_next  
M_next = M + dt * tau  
Monotonic as required.

5) κ_next  
Viability decays minimally:
kappa_next = κ - dt * a4  
Clamp to zero if needed.

6) Derived metrics computed normally.

7) Collapse logic identical to UPDATE:
If kappa_next = 0 → collapse geometry.

STRUCT_GAP is a minimal-evolution event: no Δ change, small Φ decay, minimal κ decay.

---

## 5.4 STRUCT_HEARTBEAT — Minimal Time Progression

STRUCT_HEARTBEAT is identical to GAP except dt is typically small.  
It ensures the organism progresses forward in structural time even without meaningful deformation.

### 5.4.1 Preconditions
dt > 0  
No stimulus.

### 5.4.2 Algorithm
Same as GAP, with emphasis on:
- minimal tension decay  
- minimal viability decay  
- no deformation of Δ  
- memory growth proportional to τ and dt  

The organism “ticks” forward structurally even without new input.

---

## 5.5 STRUCT_RESET — Forced Reconstruction Event

RESET is the only event type capable of exiting collapse state or recovering from any corrupted or degenerate configuration.  
RESET must create a valid baseline organism and wipe the prior structure.

### 5.5.1 Preconditions
RESET must contain:
- NO dt  
- optional reason  
- no numeric fields except defaults  

### 5.5.2 Algorithm
Produce a new structural state:

Delta_next[i] = 0  
Phi_next      = Phi_reset_default  
M_next        = 0  
Kappa_next    = Kappa_reset_default  
regime        = ACC  
μ             = 0  
det_g         = det_g_reset_default (>0)  
tau           = tau_min baseline  

Derived metrics recomputed as from a fresh organism.

### 5.5.3 RESET Guarantees
- Must ALWAYS produce a valid invariant-satisfying state  
- Must ALWAYS run in O(1)  
- Must NEVER attempt to partially preserve old structural data  
- Must not depend on past events or history  

RESET effectively “reboots” the organism.

---

## 5.6 Rejection Behavior (All Events)

If ANY step of the event processing results in:
- NaN  
- Inf  
- invalid geometry  
- forbidden domain  
- invariant violation  
Then:

1) The event is **rejected**  
2) X_next = X_current (previous valid state)  
3) Diagnostics describe the exact reason  

Rejections are deterministic and do not corrupt structural memory.

---

## 5.7 Post-Collapse Behavior

If κ == 0 (collapse state):
- STRUCT_UPDATE → rejected  
- STRUCT_GAP → rejected  
- STRUCT_HEARTBEAT → rejected  
- ONLY RESET is allowed  

All other events must preserve:
- Δ (frozen)  
- Φ (frozen)  
- M (frozen)  
- κ = 0  
- μ = 1  
- det_g = 0  
- τ = 0  
- regime = COL  

This ensures collapse behaves as a stable absorbing state.

---

## 5.8 Summary

Each FMRT event type has a strict deterministic algorithm:

UPDATE → full structural evolution  
GAP → minimal evolution (no deformation)  
HEARTBEAT → ultra-minimal evolution (no deformation)  
RESET → reconstruct valid organism, only exit from collapse  

No external data, no raw markets, no heuristics, no dynamic learning, no nondeterminism.

These algorithms define EXACTLY how FMRT must evolve the organism at every step.

---

# 6. API Contracts & Function Signatures

The FMRT Core exposes a minimal, deterministic, side-effect-free API.  
All public functions must follow strict rules:

- No randomness  
- No hidden state  
- No access to time, OS, or external systems  
- No implicit mutation  
- No allocations beyond initialization  
- Absolutely deterministic return values  

FMRT Core has only **one public entry point** for structural evolution:  
FMRT.step(X(t), E(t)) → StateEnvelope(t+1)

Everything else is internal.

This section defines mandatory function signatures, input/output contracts, and internal module-level interfaces.

---

## 6.1 Public API — Main Entry Point

### Function
StateEnvelope FMRT_step(StructuralState X, StructEvent E)

### Purpose
Perform one complete FMRT evolution step:
1. Validate event  
2. Compute X_provisional  
3. Enforce invariants  
4. Produce deterministic StateEnvelope  

### Inputs
X — current structural state  
E — canonical or raw event (raw events must still be validated by Event Handler)

### Output
StateEnvelope — contains:
- X_next  
- DerivedMetrics  
- InvariantFlags  
- Diagnostics  
- event_type  

### Behavior
- If event is valid AND invariants are satisfied → evolve organism  
- Else → reject event, X_next = X  

### Forbidden
- No partial updates  
- No access to external time or OS  
- No side effects  
- No randomness  

---

## 6.2 Internal API: Event Handler

### Function
ValidationResult EventHandler_validate(StructEvent E_raw)

### Output
ValidationResult {
    valid;           // true/false
    event;           // canonical StructEvent
    error_code;      
    error_reason;
}

### Behavior
- Verify event_type, dt, stimulus, numeric safety  
- Convert into canonical StructEvent  
- DO NOT touch structural state  
- Deterministic rejection for malformed input  

---

## 6.3 Internal API: Core Evolution Engine

### Function
ProvisionalState Engine_evolve(StructuralState X, StructEvent E)

### Output
ProvisionalState {
    X_provisional;  
    DerivedMetrics;
}

### Behavior
- Computes Δ_next, Φ_next, M_next, κ_next  
- Computes τ, det_g, curvature_R, μ, morphology_class, regime  
- Applies event-type rules (UPDATE, GAP, HEARTBEAT, RESET)  
- Produces NO invariant checking  
- Must be deterministic  
- No NaN/Inf allowed; if produced, validator will reject  

---

## 6.4 Internal API: Invariant Validator

### Function
ValidationOutcome Validator_check(StructuralState X_current,
                                  ProvisionalState X_provisional)

### Outputs
ValidationOutcome {
    accepted;              // true/false
    X_validated;           // X_provisional or fallback X_current
    invariant_flags;       // invariant results
    error_code;            // if rejected
    error_reason;
}

### Behavior
- Enforce all invariants:
  memory, viability, metric positivity, τ positivity, morphology bounds, regime order, collapse geometry, forbidden domain  
- If ANY invariant fails → reject and preserve X_current  
- Must be deterministic, exact, non-heuristic  

---

## 6.5 Internal API: Diagnostics Layer

### Function
StateEnvelope Diagnostics_build(ValidationOutcome outcome,
                                DerivedMetrics metrics,
                                StructEvent event)

### Output
StateEnvelope {
    X_next;
    DerivedMetrics;
    InvariantFlags;
    Diagnostics;
    event_type;
}

### Behavior
- “Package” all results into final output  
- Must NEVER modify X_next  
- Must provide deterministic diagnostic text and codes  
- Must produce complete envelope even if event is rejected  

---

## 6.6 Data Structure Access Rules

All public and internal APIs must satisfy:

- NO mutable global state  
- NO static caches  
- NO accessing hidden buffers  
- NO shared memory between FMRT instances  
- NO pointers to internal structures returned to outside world  
- All returned objects must be **immutable copies**  

Outside systems must NEVER acquire an internal reference to X.

---

## 6.7 Error Reporting Contracts

Every error MUST produce deterministic output:

Diagnostics.error_category ∈ {
    "None",
    "InputError",
    "InvariantError",
    "NumericError",
    "ForbiddenDomain",
    "CollapseViolation"
}

Diagnostics.step_status ∈ { OK, ERROR, DEAD }

Diagnostics must contain:
- stable error_code  
- stable error_reason  
- invariant flags  
- event_type  

No nondeterministic formatting, ordering, or wording is allowed.

---

## 6.8 Reset Function Contract (Helper API)

Some implementations may expose a public reset helper:

StructuralState FMRT_reset(reason)

Rules:
- Must be identical to processing a STRUCT_RESET event  
- Must produce the same X_next, same DerivedMetrics, same envelope  
- Must be deterministic  
- Must not access external systems  

This function is OPTIONAL but, if implemented, must behave exactly like the RESET event.

---

## 6.9 Thread Safety Requirements

Public APIs MUST be thread safe under the rule:

- Multiple readers allowed  
- Exactly one writer (evolution step) at a time  
- NO overlapping evolution steps for the same organism  
- Evolution steps across different FMRT instances are independent  

Internal function signatures must not assume shared mutable state.

---

## 6.10 Summary

FMRT V2.2 API must:

- expose only **one public evolution function** (FMRT_step)  
- provide deterministic, pure, side-effect-free behavior  
- internally separate event validation, evolution, invariant checking, diagnostics  
- guarantee O(1) runtime  
- never expose internal state  
- always output a complete deterministic StateEnvelope  

This API contract defines exactly how real implementations must interact with FMRT Core.

---

# 7. Performance Requirements & Constraints

FMRT Core V2.2 must satisfy strict performance guarantees to ensure determinism, stability, and real-time capability. Performance is not an optimization goal — it is a correctness requirement. Any degradation, dynamic behavior, memory growth, or nondeterministic timing violates FMRT principles.

This section defines all mandatory performance constraints for FMRT implementations.

---

## 7.1 Constant-Time Execution (O(1)) for Every Event

Each call:
FMRT_step(X, E)

must execute in strict O(1) time.

This means:
- no loops over variable-sized data  
- no recursion  
- no dynamic branching depending on history  
- no buffers that grow with event count  
- no data structures that depend on runtime conditions  

The number of arithmetic operations must be constant across all events.

Even under extreme load, FMRT must preserve identical per-step execution time.

---

## 7.2 Zero Dynamic Memory Allocation

FMRT must not allocate memory during event processing.

Forbidden inside processing:
- malloc/new  
- vector/array resize  
- dynamic containers (std::vector push_back, etc.)  
- garbage collection triggers  
- temporary heap allocations  

Allowed only once:
- static, fixed-size allocations during initialization  
- stack-only local objects  
- preallocated scratch buffers of fixed size  

This constraint ensures consistent latency and eliminates GC pauses, fragmentation, and unpredictability.

---

## 7.3 Constant Memory Footprint

FMRT memory usage must be:
- fixed  
- constant  
- independent of number of processed events  
- independent of the organism state  

No historical data may be stored.  
No caching mechanisms may be introduced.  
No adaptive structures or context windows are allowed.

Structural state must remain exact and self-sufficient.

---

## 7.4 Deterministic Instruction Path

Every event must follow the exact same instruction path, regardless of:
- organism regime  
- morphology  
- collapse status  
- event history  
- CPU architecture  
- compiler optimizations  

Random or unpredictable instruction branching is forbidden.

The implementation must avoid:
- branch misprediction variability  
- unstable hardware-specific FP paths  
- nondeterministic SIMD fusion or contraction  

To guarantee reproducibility, all evolution, invariant checks, and diagnostics must follow stable and identical execution paths.

---

## 7.5 Bounded Floating-Point Operations

All floating-point operations must be:
- bounded  
- stable  
- non-explosive  
- non-iterative  

Forbidden:
- iterative solvers  
- nonlinear system solutions  
- FP accumulation in loops  
- approximate dynamic refinement  

Every math operation must be O(1) and guaranteed to complete without overflow/underflow (except collapse-defined zeroing).

---

## 7.6 No Asynchronous or Background Tasks

FMRT implementations must NOT use:
- background threads  
- task queues  
- asynchronous workers  
- schedulers  
- timeouts  
- timers  

Evolution must be synchronous and atomic.

This ensures:
- full determinism  
- reproducible ordering  
- zero temporal dependencies  

---

## 7.7 Predictable Latency Under Load

FMRT must maintain stable per-event latency even when:
- events arrive at high frequency  
- CPU is under heavy load  
- multiple FMRT instances run concurrently  
- organism approaches collapse  
- morphology becomes degenerate  

Approaching collapse MUST NOT slow the computation.

RESET MUST NOT cost more time than UPDATE.

---

## 7.8 Multi-Instance Scaling Requirements

Running N FMRT instances must scale linearly with N.  
This means:

- no shared mutable global state  
- no global locks  
- no blocking primitives  
- no static caches shared between instances  
- no contention-sensitive structures  

The cost of maintaining 10 FMRT organisms must be 10× the cost of one (within constant marginal cost).

---

## 7.9 Forbidden Performance Behavior

The following optimizations and techniques are strictly prohibited:

- using “fast math” or nondeterministic FP modes  
- using caching for Δ, Φ, M, κ evolution  
- adaptive time-stepping  
- vectorization that changes operation order  
- speculative execution influencing FP rounding  
- JIT-dependent optimizations that reorder FP ops  
- lazy evaluation that yields different rounding sequences  
- auto-tuned approximations or ML-based predictors  
- event batching or reordering  

If it breaks determinism, it is forbidden.

---

## 7.10 Timing Invariance Under OS Scheduling

FMRT must NOT rely on:
- system tick rate  
- thread scheduling  
- CPU frequency scaling  
- power-saving modes  
- kernel clock behavior  

Even if the OS delays execution, FMRT must not incorporate that information into computation.

dt from events is purely structural — it is NOT wall-clock time.

---

## 7.11 RESET Performance Requirements

RESET must:
- run in O(1)  
- allocate no memory  
- compute baseline fields instantly  
- not trigger cleanup operations  
- not depend on organism history  
- not vary in cost depending on collapse or heavy deformation  

RESET must be one of the cheapest operations in FMRT.

---

## 7.12 Profiling & Diagnostic Constraints

FMRT must produce deterministic timing behavior even with:
- logging disabled  
- logging enabled  
- profiling tools attached  

Therefore:
- NO logging inside core modules  
- NO I/O calls in evolution path  
- NO debug printing  
- NO environment-dependent diagnostics  

Diagnostics must be computed in pure CPU memory, never via I/O.

---

## 7.13 Summary

FMRT V2.2 performance rules ensure:

- strict O(1) event cost  
- no allocations  
- no history  
- no nondeterminism in branching  
- stable FP ordering  
- constant memory footprint  
- linear multi-instance scalability  
- deterministic reset and collapse handling  
- zero dependence on timing or environment  

Performance is a **correctness requirement**, not an optimization goal.

---

# 8. Error Handling & Failure Modes

FMRT Core V2.2 is designed to be structurally safe, deterministic, and invariant-preserving.  
Errors are not exceptions — they are explicit structural outcomes.  
All error handling must obey the rule:

If ANY error occurs → structural state X(t) MUST remain unchanged.

FMRT MUST NOT attempt correction, smoothing, fallback heuristics, or partial updates.  
Error handling is deterministic, atomic, and fully transparent through diagnostics.

This section defines ALL possible FMRT error types and their required behaviors.

---

## 8.1 Error Philosophy

FMRT error-handling guarantees:

1. **No corruption of X(t)**  
   If an error happens, X_next = X_current.

2. **No hidden or silent failures**  
   Every failure MUST produce an explicit Diagnostics.error_code and ERROR step_status.

3. **Deterministic rejection**  
   Same inputs → same error → same envelope.

4. **No auto-correction**  
   FMRT never guesses missing data, never adjusts bad values, never “fixes” invalid states.

5. **No partial updates**  
   The state either fully evolves or fully stays unchanged.

---

## 8.2 Error Categories

FMRT V2.2 errors fall into the following deterministic categories:

### (1) InputError
Triggered when:
- event_type invalid  
- dt ≤ 0 for UPDATE/GAP/HEARTBEAT  
- dt given for RESET  
- stimulus contains NaN/Inf  
- event contains invalid numeric formats  

Behavior:
- Reject event  
- Diagnostics.error_category = InputError  

### (2) NumericalError
Triggered when ANY provisional computation produces:
- NaN  
- Inf  
- denormalized values  
- overflow conditions  
- underflow below safe thresholds (except collapse zeroing)

Behavior:
- Reject event  
- Diagnostics.error_category = NumericalError  

### (3) InvariantError
Triggered when ANY invariant fails:
- memory decrease  
- κ < 0  
- det g ≤ 0 when κ > 0  
- τ ≤ 0 when κ > 0  
- μ out of bounds  
- regime reversal  
- collapse geometry mismatch  
- forbidden-domain entry  

Behavior:
- Reject event  
- Diagnostics.error_category = InvariantError  

### (4) ForbiddenDomain
Triggered when provisional state enters:
- κ ≤ 0 (not collapse)  
- det g ≤ 0 when κ > 0  
- τ ≤ 0 when κ > 0  
- μ outside [0,1]  
- curvature invalid  
- ANY NaN/Inf values  

Behavior:
- Reject event  
- Diagnostics.error_category = ForbiddenDomain  

### (5) CollapseViolation
Triggered when:
- organism is collapsed (κ=0)  
- and event ≠ RESET is applied  

Behavior:
- Reject event  
- X remains frozen  
- Diagnostics.error_category = CollapseViolation  
- step_status = DEAD  

---

## 8.3 Error Handling Algorithm (Universal)

For ALL events, errors are handled exactly as follows:

1) Compute X_provisional  
2) Check for numeric failures  
3) Check invariants  
4) If ANY failure:
    X_next = X_current
    step_status = ERROR (or DEAD if collapse-locked)
    Diagnostics.error_category = (InputError / NumericalError / InvariantError / ForbiddenDomain / CollapseViolation)
    Diagnostics.error_reason = precise explanation
5) Else:
    X_next = X_provisional
    step_status = OK

No branch may bypass this error pipeline.

---

## 8.4 Deterministic Error Reporting

Diagnostic errors MUST include:

Diagnostics.step_status ∈ {OK, ERROR, DEAD}  
Diagnostics.error_code    → stable numeric  
Diagnostics.error_category → deterministic enum  
Diagnostics.error_reason   → predefined static text  
Diagnostics.event_type     → original event  
Diagnostics.invariant_flags → detailed invariant evaluation  

### NO free-form strings  
### NO variable formatting  
### NO system-dependent error messages  
### NO timestamps  
### NO stack traces  
### NO nondeterministic content  

FMRT output must be byte-for-byte identical across all platforms.

---

## 8.5 X(t) Preservation Rules

If error occurs, X_next MUST equal X_current in ALL fields:

- Δ unchanged  
- Φ unchanged  
- M unchanged  
- κ unchanged  
- regime unchanged  
- morphology unchanged  
- derived metrics from previous step retained  

FMRT must NOT update ANY part of the state on error.

State preservation is absolute.

---

## 8.6 Collapse Error Behavior

When κ == 0:
- UPDATE → reject (CollapseViolation)  
- GAP → reject (CollapseViolation)  
- HEARTBEAT → reject (CollapseViolation)  
- RESET → accepted  

step_status MUST be:
- DEAD if organism is collapsed  
- OK only for RESET  

Diagnostics MUST reflect collapse permanence.

Structural fields (Δ, Φ, M, κ) must remain frozen until RESET.

---

## 8.7 Numeric Failure Modes

FMRT must detect and reject ANY numeric anomaly:

- exp overflow  
- underflow in τ or det g  
- FP division by zero  
- sqrt of negative  
- catastrophic cancellation producing NaN  
- invalid FP flags in Δ/Φ/M/κ calculations  

If ANY such condition is detected:
- Reject  
- error_category = NumericalError  

---

## 8.8 Forbidden Optimization Failure Modes

FMRT must NOT:

- silently contract FP operations  
- reorder operations in an unstable sequence  
- use hardware-specific undocumented fused operations  
- rely on undefined FP flags  
- compensate numeric errors with random jitter  
- round adaptively  
- use stochastic approximation  

All these cause nondeterministic results and MUST be rejected at development level.

---

## 8.9 RESET Failure Handling

If RESET fails due to incorrect baseline config:
- Fatal configuration error  
- FMRT must refuse to initialize  
- This is the ONLY unrecoverable FMRT error  
- Must be caught during system startup, NOT during event processing  

---

## 8.10 Summary of Error Handling System

FMRT error system is defined by the following invariants:

- Errors NEVER modify structural state  
- Errors ALWAYS produce deterministic envelopes  
- No silent failure is allowed  
- No heuristic correction is allowed  
- Collapse is an absorbing state  
- RESET is the only recovery path  
- Numeric anomalies cause immediate rejection  
- Forbidden domains are never entered  

FMRT error-handling architecture is minimal, strict, deterministic, and impossible to bypass.

---

# 9. Testing & Validation Requirements

FMRT Core V2.2 must undergo strict testing to guarantee deterministic behavior, invariant preservation, numerical stability, and complete compliance with FMT 3.1 and FMRT Specification V2.2.  
Testing is not optional — it is part of the correctness requirements.

This section defines mandatory test categories, expected behaviors, and validation procedures for verifying an FMRT implementation.

---

## 9.1 Testing Philosophy

FMRT tests must guarantee:

1. **Determinism**  
   Identical inputs → identical outputs (bitwise identical).

2. **Invariant correctness**  
   No implementation may violate or bypass invariants.

3. **Numerical resilience**  
   No NaN/Inf/denormals must ever appear.

4. **Forbidden-domain protection**  
   Implementation must never enter invalid geometric or viability domains.

5. **Exact FMRT semantics**  
   All results must match the expected theoretical outcomes.

6. **Zero side effects**  
   No mutation outside X(t) and no influence from hidden state.

7. **Strict O(1) performance**  
   Tests ensure no hidden loops or allocations.

FMRT must pass ALL required tests before being considered correct.

---

## 9.2 Unit Tests (Component-Level)

Each module requires independent unit testing:

### Event Handler Tests
- Accept valid events with correct canonicalization  
- Reject malformed events (bad dt, NaN stimulus, invalid type)  
- Must produce the exact error_code expected  

### Core Evolution Engine Tests
- Δ, Φ, M, κ evolution with fixed inputs yields exact known outputs  
- Deformation-only updates verified against analytical reference  
- GAP and HEARTBEAT produce minimal updates  
- RESET produces correct baseline organism  

### Invariant Validator Tests
- Reject states with κ < 0  
- Reject det g ≤ 0 when κ > 0  
- Reject τ ≤ 0 when κ > 0  
- Reject μ out of bounds  
- Reject regime reversals  
- Correctly identify collapse  

### Diagnostics Layer Tests
- step_status must match expected  
- error_category and error_code must be exact  
- Invariant flags must match true/false exactly  
- Envelope must contain ALL fields, always  

All unit tests MUST be deterministic.

---

## 9.3 Integration Tests (Full Pipeline)

These tests validate complete step execution:

### Valid UPDATE Sequence
- Provide a sequence of well-formed UPDATE events  
- Expect exact deterministic evolution  
- Validate exact Δ, Φ, M, κ values  
- Validate regime progression  
- Validate morphology transitions  

### GAP and HEARTBEAT Behavior
- Simulate periods without stimulus  
- Check minimal evolution  
- Ensure memory grows monotonically  
- Ensure viability decays at minimal rate  

### RESET Behavior
- RESET must reset the organism EXACTLY  
- RESET must recover from collapse  
- RESET must produce identical result across all platforms  

### Rejecting Invalid Events
- Feed invalid dt  
- Feed malformed stimuli  
- Expect immediate rejection  
- Ensure X(t) remains unchanged  

---

## 9.4 Determinism Tests

These tests MUST be performed on multiple architectures:

- x86 (Intel/AMD)
- ARM (mobile, servers)
- Different compilers (GCC, Clang, MSVC)
- Different OS (Linux, macOS, Windows)

Test Procedure:
- Run same input sequence  
- Output StateEnvelope series must match BIT-FOR-BIT  
- ANY deviation is a fatal determinism violation  

This is mandatory for FMRT compliance.

---

## 9.5 Numerical Stability Tests

Stress tests include:

### Boundary Condition Tests
- Extremely small Δ  
- Extremely large Δ  
- Extremely small dt  
- Large dt  
- κ near zero  
- κ exactly zero  
- M approaching large finite values  

### Forbidden Value Tests
Ensure FMRT rejects:
- NaN inputs  
- Inf inputs  
- Overflow conditions  
- Negative metric determinant  
- Negative viability  

### Collapse Boundary Tests
- κ → 0 from above  
- Validate smooth transition into collapse geometry  
- Validate τ → 0, det g → 0, μ → 1  
- Validate regime = COL  

FMRT MUST NOT produce NaN or Inf in these tests.

---

## 9.6 Performance Tests

Performance tests must verify:

1. **O(1) behavior** — runtime per event must not increase with event count.  
2. **Zero allocations** — no dynamic memory on any step.  
3. **Constant memory footprint** — resident size stable across the entire run.  
4. **RESET speed** — RESET must not be slower than UPDATE.  
5. **Multi-instance scaling** — running 10 organisms must cost ~10× running 1 organism.

All tests must be automated and reproducible.

---

## 9.7 Collapse Behavior Tests

Collapse requires special verification:

1. Apply events driving κ → 0  
2. Verify correct collapse geometry:  
   - κ=0  
   - det g=0  
   - τ=0  
   - μ=1  
   - regime=COL  
3. Apply UPDATE/GAP/HEARTBEAT after collapse → MUST be rejected  
4. Apply RESET → MUST succeed and resurrect organism  

Collapse behavior must match theoretical expectations perfectly.

---

## 9.8 Envelope Completeness Tests

Every StateEnvelope MUST contain:

- X_next  
- DerivedMetrics  
- InvariantFlags  
- Diagnostics  
- event_type  

Tests must verify:
- No missing fields  
- No null placeholders  
- No random ordering  
- No platform differences  

Envelope MUST be identical across platforms.

---

## 9.9 Regression Tests

Whenever FMRT implementation changes:

- ALL previous tests MUST still pass  
- Added tests MUST validate invariants not previously covered  
- Functional parity MUST remain intact  
- No performance regression permitted  
- No nondeterminism introduced  

Regression testing ensures long-term stability.

---

## 9.10 Summary

A valid FMRT V2.2 implementation MUST pass:

- Unit tests  
- Integration tests  
- Determinism tests  
- Stability tests  
- Performance tests  
- Collapse tests  
- RESET tests  
- Envelope completeness tests  
- Regression tests  

Failure to pass ANY test means the implementation is NOT FMRT-compliant.

Testing is not optional — it is part of FMRT’s structural foundation.
