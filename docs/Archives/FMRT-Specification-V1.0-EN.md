# FMRT-Specification-V1.0-EN

---

## 0. Overview

The Flexion Market Runtime Theory (FMRT) defines the executable structural model of a financial market as a living Flexion organism.  
FMRT is derived directly from Flexion Market Theory (FMT) Version 2.2 and provides the formal operational foundation required to reconstruct, evolve, and classify the internal structural state of a market in real time.

FMRT is not a trading system, forecasting model, or statistical engine.  
It does not evaluate market direction, generate entry or exit signals, or interpret price patterns.  
Instead, FMRT implements the structural laws of FMT 2.2 and produces a deterministic classification of the market’s internal state and morphology.

The runtime engine receives only observable price–timestamp pairs \((P(t), T(t))\).  
Using these inputs, FMRT performs:

1. **Input verification and normalization**,  
2. **Reconstruction of internal state** \(X(t)\) from observable projection,  
3. **Deterministic structural evolution** under the operator \(I(X)\),  
4. **Evaluation of structural time, viability, and curvature**,  
5. **Classification of the structural cycle phase**:  
   \[
   \text{State}(t) \in \{\text{ACC}, \text{DEV}, \text{REL}, \text{COLL}\},
   \]
6. **Morphological analysis** (Class I–IV),
7. **Enforcement of all structural invariants** of FMT 2.2.

FMRT guarantees:

- deterministic behavior across platforms and builds,  
- mathematically valid reconstruction of structure from price,  
- correct evolution within the structural manifold \(\mathbb{M}^+\),  
- strict preservation of memory, viability, continuity, and regime ordering,  
- compatibility with collapse geometry and κ-stable neighborhoods.

FMRT defines **what** the runtime engine must compute and guarantee, not **how** it is implemented.  
Implementation details, algorithms, and code structures are addressed in separate documents (Design, Implementation, Static Analysis, etc.).

This specification establishes the complete formal requirements for FMRT Core as the foundational structural engin

---

## 1. Scope & Purpose

This specification defines the complete formal behavior of the Flexion Market Runtime Theory (FMRT) Core — the executable structural engine derived from Flexion Market Theory (FMT) Version 2.2.  
FMRT establishes **what** the system must compute, enforce, and guarantee during real-time operation.

The scope of FMRT includes:

1. **Definition of the internal structural state**  
   \[
   X(t) = (X_\Delta, X_\Phi, X_M, X_\kappa),
   \]
   and all derived structural quantities (temporal density, curvature indicators, viability decay, collapse pressure, morphology).

2. **Deterministic reconstruction** of structural state from observable input  
   \[
   R[P(t)] \rightarrow X(t).
   \]

3. **Deterministic evolution** of structural state through time using the operator  
   \[
   X(t+1) = I(X(t)).
   \]

4. **Evaluation of structural time**, viability, collapse proximity, and κ-stable neighborhood conditions.

5. **Classification of market structure** into the universal Flexion structural cycle:
   \[
   \text{State}(t) \in \{\text{ACC}, \text{DEV}, \text{REL}, \text{COLL}\}.
   \]

6. **Morphology assessment** (Class I–IV) based on tension, curvature, and viability characteristics.

7. **Enforcement of all structural invariants** required by FMT 2.2:
   - continuity of \(X(t)\),
   - monotonic memory growth,
   - viability positivity,
   - deterministic regime ordering,
   - compatibility with collapse geometry.

8. **Input integrity and normalization** to ensure stable structural computation under imperfect or corrupted observable data.

9. **Definition of the runtime output model**, describing the exact structural information made available to external systems.

10. **Error conditions and runtime guarantees**, including deterministic failure modes and boundary behaviors.

The scope explicitly **excludes**:

- trading logic or trading signal generation,
- heuristics, indicators, or statistical methods,
- forecasting or directional inference,
- smoothing, filtering, or preprocessing of price data beyond input integrity checks,
- assumptions about market microstructure or external economic variables.

The purpose of FMRT is to provide a **pure, deterministic, mathematically valid structural engine** that can be used by higher-level systems for analysis, research, strategy development, or integration into Flexion-based architectures.

FMRT defines **WHAT the engine must do**, not **HOW it is implemented**.  
All implementation details are handled separately in the Design, Implementation, Static Analysis, and Integration documents.

---

## 2. Terminology & Definitions

This section defines all formal terms used throughout the FMRT specification.  
All terminology is consistent with Flexion Market Theory (FMT) Version 2.2 and the foundational principles of the Flexion Framework.

---

### 2.1 Structural State Vector

The internal structural state of the market is represented as:

\[
X(t) = \big( X_\Delta(t),\; X_\Phi(t),\; X_M(t),\; X_\kappa(t) \big),
\]

where:

- **\(X_\Delta\)** — Differentiation (structural deformation component),  
- **\(X_\Phi\)** — Tension (structural energy component),  
- **\(X_M\)** — Memory (irreversible structural accumulation),  
- **\(X_\kappa\)** — Viability (capacity for continued existence).

FMRT does not process the market beyond this structural representation.

---

### 2.2 Structural Manifold \(\mathbb{M}^+\)

All valid structural states must lie in:

\[
\mathbb{M}^+ = \{ X \mid X_\kappa > 0 \},
\]

the domain of structural viability.  
Exiting \(\mathbb{M}^+\) corresponds to collapse and terminates structural evolution.

---

### 2.3 Structural Operators

FMRT uses three theoretical operators:

#### **Reconstruction Operator**
\[
R[P] : P(t) \rightarrow X(t)
\]

Recovers the internal structural state from observable price.

#### **Evolution Operator**
\[
I(X) : X(t) \rightarrow X(t+1)
\]

Propagates structure through time.

#### **Projection Operator**
\[
\pi(X) : X(t) \rightarrow P(t)
\]

Maps structure into the observable domain.

Projection is lossy and non-invertible.

---

### 2.4 Structural Time

Structural time density is:

\[
\tau(t) = \frac{dX_M(t)}{dt}.
\]

It defines the intrinsic rate of irreversible structural evolution and is independent of observable chronological time.

---

### 2.5 Viability and Collapse Geometry

- **Viability**:
  \[
  X_\kappa(t) > 0
  \]

- **Collapse boundary**:
  \[
  X_\kappa = 0
  \]

- **Collapse pressure**:
  \[
  \Pi(t) = -\frac{dX_\kappa(t)}{dt}
  \]

- **Collapse gradient**:
  \[
  \nabla\Pi(t) = -\nabla X_\kappa(t)
  \]

Collapse proximity does not correspond to observable price patterns.

---

### 2.6 Structural Cycle States

FMRT classifies structure into one of four universal phases:

\[
\text{State}(t) \in \{\text{ACC},\; \text{DEV},\; \text{REL},\; \text{COLL}\}.
\]

- **ACC** — low temporal density, structural initialization, mild curvature  
- **DEV** — rapid deformation, maximal temporal density, high tension  
- **REL** — partial decompression, stabilization  
- **COLL** — viability approaching zero, curvature divergence, temporal collapse

These are **structural** states, not price-based states.

---

### 2.7 Morphological Classes

Morphology characterizes the structural condition:

- **Class I** — Elastic morphology  
- **Class II** — Plastic morphology  
- **Class III** — Degenerate morphology  
- **Class IV** — Near-collapse morphology

Morphology is independent of the structural cycle state.

---

### 2.8 Observable Input

The only permitted observable is the raw price–timestamp pair:

\[
(P(t),\; T(t)).
\]

FMRT does not interpret candles, indicators, volumes, volatility, or order flow.

---

### 2.9 Determinism

FMRT must satisfy strict determinism:

- identical input sequences → identical structural states,  
- identical price histories → identical reconstructions,  
- identical timestamps → identical temporal density.

Floating-point nondeterminism is forbidden.

---

### 2.10 Structural Invariants

Throughout FMRT operation, the following invariants must hold:

- **Memory monotonicity:**
  \[
  X_M(t+1) \ge X_M(t)
  \]
- **Viability positivity:**
  \[
  X_\kappa(t) > 0
  \]
- **Continuity:**
  \[
  X(t) \in C^0
  \]
- **Regime ordering:**
  \[
  \text{ACC} \rightarrow \text{DEV} \rightarrow \text{REL} \rightarrow (\text{COLL})
  \]
- **No structural backward evolution**, no rewrites of history.

---

## 3. Input Model

FMRT receives input exclusively from the observable projection layer in the form of price–timestamp pairs.  
FMRT does not accept, interpret, or rely on any other type of market data.  
All structural computation is derived solely from the observable input stream.

---

### 3.1 Input Data Structure

Each runtime update provides exactly one tuple:

\[
\text{Input}(t) = (P(t),\; T(t)),
\]

where:

- **\(P(t) \in \mathbb{R}\)** — raw observable price,  
- **\(T(t) \in \mathbb{N}\)** — strictly increasing timestamp.

FMRT treats these values as raw projections of structural state through the operator \(\pi(X)\).

---

### 3.2 Timestamp Requirements

The timestamp must satisfy:

1. **Strict monotonicity**:  
   \[
   T(t+1) > T(t).
   \]

2. **Non-negativity**:  
   \[
   T(t) \ge 0.
   \]

3. **Deterministic ordering** — timestamps define the only valid sequence of observation.

FMRT does **not** assume uniform spacing; timestamps may be irregular.

---

### 3.3 Price Requirements

The observable price must satisfy:

1. **Finite IEEE-754 value** (double precision).  
2. **Real-valued domain**:  
   \[
   P(t) \in \mathbb{R}.
   \]
3. **Price is treated as projection, not structure**.  
4. **No smoothing or preprocessing** is permitted prior to FMRT input.

---

### 3.4 Streaming Model

FMRT must operate in **streaming mode**, processing inputs one at a time:

- no batch input,  
- no lookahead,  
- no backward adjustment,  
- no reprocessing of past data.

Once an input is processed, it becomes part of immutable structural history.

---

### 3.5 No Derived Inputs

FMRT must not accept or interpret:

- OHLC data,  
- volumes,  
- spreads,  
- volatility estimates,  
- order flow,  
- market depth,  
- time-frame aggregates,  
- indicators,  
- statistical filters.

These values are external to the Flexion framework and have no structural meaning.

---

### 3.6 Input Domain Constraints

FMRT must validate:

- timestamp monotonicity,  
- finite numeric values,  
- ordering consistency.

Invalid inputs must be handled by the Input Integrity Layer (Section 4).

---

### 3.7 Observable Layer Independence

The observable time \(T(t)\):

- is not structural time,  
- does not reflect structural tempo,  
- does not influence memory or viability directly.

The observable price \(P(t)\):

- is not structural curvature,  
- does not encode tension or differentiation,  
- cannot reveal the structural state.

They serve only as projections for reconstruction.

---

### 3.8 Deterministic Mapping

Given an identical sequence \(\{(P(t), T(t))\}\):

- FMRT must reconstruct identical state histories,  
- produce identical structural transitions,  
- classify identical structural phases.

This determinism requirement is mandatory.

---

### 3.9 Input Assumptions

FMRT assumes:

- data may arrive at arbitrary intervals,  
- price may contain noise,  
- timestamps may be irregular,  
- projection may be distorted.

FMRT must still maintain structural consistency through reconstruction rules.

---

## 4. Input Integrity & Normalization

FMRT must ensure that the observable input stream is valid, ordered, and structurally usable before reconstruction and evolution are applied.  
Real-world price feeds may contain duplicated ticks, corrupted values, invalid timestamps, or projection artifacts.  
These defects do not exist in the theoretical layer but must be resolved at the runtime level.

The Input Integrity Layer acts as a gatekeeper.  
Only valid, normalized inputs may proceed to reconstruction.

---

### 4.1 Purpose of Input Integrity Layer

The purpose of the Input Integrity Layer is to:

1. Guarantee correctness of the observable input stream.  
2. Prevent invalid or corrupted inputs from corrupting the structural state.  
3. Maintain determinism and reproducibility under imperfect real data.  
4. Ensure FMRT always operates on a consistent ordered sequence of valid observations.

This layer does **not** modify the meaning of data — it only validates and normalizes it.

---

### 4.2 Valid Input Definition

A price–timestamp pair \((P(t), T(t))\) is valid if:

1. **Timestamp monotonicity**:  
   \[
   T(t) > T(t-1)
   \]

2. **Finite numeric price value** (IEEE-754 double):
   - not NaN,  
   - not ±Inf,  
   - not subnormal or denormalized beyond platform tolerance.

3. **Price domain validity**:
   - \(P(t) > 0\),  
   - not unrealistically large or small beyond representable numeric range.

4. **No structural contradictions**:
   A valid input must not contradict past validated timestamps.

---

### 4.3 Duplicate Tick Handling

If an incoming tick satisfies:

- \(T(t) = T(t-1)\), and  
- \(P(t) = P(t-1)\),

then the tick must be **ignored**.

Duplicate ticks do not modify structure, do not trigger reconstruction, and do not advance the internal state.

---

### 4.4 Timestamp Reversal or Equality

If:

\[
T(t) \le T(t-1),
\]

then the tick is invalid.

FMRT must:

- **discard** it,  
- **record** the integrity violation,  
- **not** modify structural state.

Reprocessing or reordering of ticks is prohibited.

---

### 4.5 Corrupted Price Values

FMRT must reject ticks with:

- NaN,  
- ±Inf,  
- negative values,  
- zero price,  
- non-finite IEEE-754 representations,  
- numeric overflows or underflows.

Such ticks must not enter reconstruction.

---

### 4.6 Unrealistic Tick Magnitude

If the observable price jump:

\[
|P(t) - P(t-1)|
\]

exceeds a clearly impossible representable bound (implementation-defined),  
FMRT must treat the tick as corrupted and discard it.

Structural continuity must never be violated due to input error.

---

### 4.7 Static or “Flatline” Tick Patterns

If a prolonged sequence of ticks occurs with identical price but increasing timestamps:

- FMRT must accept them,  
- reconstruction must interpret them as constant projection,  
- structural evolution must remain continuous.

No artificial assumptions about a “flat market” may be introduced.

---

### 4.8 High-Frequency Tick Bursts

FMRT must process all ticks regardless of speed, but:

- temporal density \(\tau\) must remain consistent,  
- memory must not inflate artificially,  
- viability must not degrade due to rate of ticks.

FMRT uses **only timestamps** and must not react to data frequency patterns.

---

### 4.9 Missing or Delayed Data

If time jumps significantly (large \(\Delta T\)):

- FMRT must accept the input,  
- reconstruction must handle the temporal gap correctly,  
- structural continuity must be preserved according to constraints.

FMRT must **not** interpolate missing data.

---

### 4.10 Tick Normalization Rules

Before being forwarded to reconstruction, every valid tick must satisfy normalization:

1. Price is finite and positive.  
2. Timestamp strictly increases.  
3. Tick is not a duplicate.  
4. Tick is not corrupted or contradictory.

FMRT must guarantee:

\[
(P_{\text{norm}}(t),\; T_{\text{norm}}(t)) \quad \text{is clean, ordered, and valid}.
\]

---

### 4.11 Deterministic Behavior Under Dirty Data

Given identical streams (including corrupted or duplicated ticks), FMRT must:

- detect defective ticks deterministically,  
- discard them deterministically,  
- proceed identically across platforms.

Nondeterministic rejection decisions are prohibited.

---

### 4.12 Structural Safety Guarantee

Under no circumstances may invalid input:

- break structural continuity,  
- corrupt viability,  
- violate invariants,  
- retroactively alter historical states.

The Input Integrity Layer is the first and mandatory line of defense.

---

### 4.13 Prohibited Behaviors

This layer must **not**:

- smooth price data,  
- average multiple ticks,  
- modify or transform price,  
- infer volatility or trends,  
- buffer or reorder ticks,  
- apply prediction or heuristics.

The layer ensures **data correctness**, not **data interpretation**.

---

## 5. Internal State Representation \(X(t)\)

FMRT maintains an internal structural state vector representing the market as a living Flexion organism.  
This state is never observed directly; it exists entirely within the structural manifold \(\mathbb{M}^+\) and is reconstructed and evolved from normalized input data.

The state vector must remain valid, continuous, invariant-preserving, and fully consistent with the laws of FMT 2.2.

---

### 5.1 Structural State Vector Definition

At each structural time step, the internal state is:

\[
X(t) = 
\big(
X_\Delta(t),\;
X_\Phi(t),\;
X_M(t),\;
X_\kappa(t)
\big).
\]

Where:

- **\(X_\Delta(t)\)** — Differentiation (structural deformation descriptor)  
- **\(X_\Phi(t)\)** — Tension (structural energy)  
- **\(X_M(t)\)** — Memory (irreversible accumulation)  
- **\(X_\kappa(t)\)** — Viability (remaining structural capacity)

This vector is the **only** representation of the market inside FMRT.

---

### 5.2 Manifold Validity

FMRT must guarantee that:

\[
X(t) \in \mathbb{M}^+ = \{ X \mid X_\kappa(t) > 0 \}.
\]

Leaving this domain represents geometric collapse and is prohibited.

---

### 5.3 Structural Continuity Requirement

FMRT must enforce:

\[
X(t) \in C^0,
\]

meaning:

- the structure must evolve smoothly,
- no discontinuities in \(X(t)\) are allowed,
- price discontinuities must not break structural continuity.

Continuity is a strict theoretical requirement.

---

### 5.4 Memory Component \(X_M(t)\)

Memory represents irreversible structural imprinting.

Invariant:

\[
X_M(t+1) \ge X_M(t).
\]

FMRT must:

- strictly enforce monotonic growth,
- prevent floating-point regression due to precision loss,
- ensure deterministic accumulation over time.

Memory defines structural time \(\tau\).

---

### 5.5 Viability Component \(X_\kappa(t)\)

Viability governs the organism’s capacity to continue evolving.

Constraints:

1. **Positivity:**  
   \[
   X_\kappa(t) > 0
   \]

2. **Monotonic decay:**  
   \[
   X_\kappa(t+1) \le X_\kappa(t)
   \]

Viability may never increase, rebound, or reset.

This component drives collapse geometry.

---

### 5.6 Tension Component \(X_\Phi(t)\)

Tension measures internal structural stress.

FMRT must ensure:

- tension evolves smoothly,  
- no negative values are allowed,  
- tension aligns with curvature and differentiation behavior,  
- tension increases during deformation-heavy regimes (DEV).

---

### 5.7 Differentiation Component \(X_\Delta(t)\)

Differentiation encodes shape deformation of the structural configuration.

Requirements:

- must evolve continuously,  
- must reflect structural compatibility with reconstruction,  
- must remain numerically stable during rapid transitions.

---

### 5.8 Derived Internal Quantities

FMRT computes the following derived values for internal use:

#### (a) Structural time density:
\[
\tau(t) = \frac{dX_M(t)}{dt}
\]

#### (b) Collapse pressure:
\[
\Pi(t) = -\frac{dX_\kappa(t)}{dt}
\]

#### (c) Collapse gradient:
\[
\nabla\Pi(t) = -\nabla X_\kappa(t)
\]

#### (d) Curvature indicators:
Derived implicitly from the structural metric defined by FMT 2.2.

Derived quantities are **not** part of external output.

---

### 5.9 Initialization Rules

On the first valid input:

\[
X(t_0) = X_0
\]

FMRT must initialize:

- \(X_\kappa(t_0) > 0\),  
- \(X_M(t_0) \ge 0\),  
- tension and differentiation in stable proportions,  
- curvature in a non-degenerate configuration.

Initialization must be deterministic.

---

### 5.10 Persistence of State

FMRT must preserve the state vector across updates.  
Loss or corruption of state is a fatal runtime error.

FMRT does **not** support:

- state rewinding,  
- state rewriting,  
- retroactive recalculation.

History is immutable.

---

### 5.11 Structural Integrity Conditions

At every update FMRT must verify:

- continuity of \(X(t)\),  
- memory monotonicity,  
- viability positivity,  
- stability of curvature,  
- regime-ordering constraints,  
- validity within \(\mathbb{M}^+\).

If any condition fails → Section 15 (Error Conditions).

---

## 6. Evolution Operator \(I(X)\)

The Evolution Operator defines how the internal structural state \(X(t)\) transitions to \(X(t+1)\) under the laws of Flexion Market Theory (FMT 2.2).  
The operator must preserve structural continuity, enforce invariants, and guarantee deterministic progression inside the manifold \(\mathbb{M}^+\).

FMRT must never violate these constraints when updating the structural state.

---

### 6.1 Operator Definition

The operator is defined as a mapping:

\[
I : \mathbb{M}^+ \rightarrow \mathbb{M}^+
\]

such that:

\[
X(t+1) = I\big(X(t)\big).
\]

This update must be deterministic and invariant-preserving.

---

### 6.2 Determinism Requirement

Given:

- identical initial state \(X(t)\),  
- identical reconstruction results,

the evolution step **must produce identical**:

- updated internal states,  
- derived quantities,  
- structural classifications.

Non-deterministic behavior is strictly prohibited.

---

### 6.3 Structural Continuity Constraint

The operator must satisfy:

\[
X(t+1) - X(t) \in C^0.
\]

This ensures:

- smooth structural evolution,  
- no discontinuities or jumps,  
- no impact from discontinuous price movement.

The structure must remain continuous even if price is not.

---

### 6.4 Memory Irreversibility

The memory component must satisfy:

\[
X_M(t+1) \ge X_M(t).
\]

FMRT must enforce:

- monotonic growth with zero tolerance for regression,  
- rounding-error-resistant accumulation,  
- structural time consistency (Section 9).

This invariant is essential to the irreversibility of structural time.

---

### 6.5 Viability Decay Constraint

Viability must satisfy:

\[
X_\kappa(t+1) \le X_\kappa(t),
\qquad X_\kappa(t+1) > 0.
\]

Meaning:

- viability consumption cannot reverse,  
- viability cannot be increased by reconstruction or evolution,  
- FMRT must prevent accidental rebound due to noise or floating-point errors.

This governs collapse dynamics.

---

### 6.6 Tension Update

Tension evolves via:

\[
X_\Phi(t+1) = I_\Phi\big(X(t)\big)
\]

and must satisfy:

- non-negativity: \(X_\Phi(t+1) \ge 0\),  
- continuity,  
- compatibility with curvature and differentiation,  
- increased activity during DEV phases.

Tension drives structural curvature.

---

### 6.7 Differentiation Update

Differentiation evolves via:

\[
X_\Delta(t+1) = I_\Delta\big(X(t)\big)
\]

and must:

- preserve continuity,  
- reflect deformation consistent with reconstruction,  
- avoid geometric singularities except near collapse.

---

### 6.8 Regime Compatibility

Evolution must respect universal structural ordering:

\[
\text{ACC} \rightarrow \text{DEV} \rightarrow \text{REL} \rightarrow \text{COLL}.
\]

FMRT must **forbid**:

- DEV → ACC,  
- REL → ACC,  
- REL → DEV,  
- COLL → any earlier state.

Permitted:

- repeated ACC cycles,  
- extended DEV phases,  
- extended REL phases.

---

### 6.9 Collapse Compatibility

If viability approaches zero:

\[
X_\kappa(t) \rightarrow 0,
\]

the operator must:

- reduce structural time density,  
- increase collapse pressure,  
- allow curvature divergence,  
- prevent transition beyond collapse boundary.

FMRT **must not simulate past collapse**; evolution stops at the boundary.

---

### 6.10 Interaction with Reconstruction

Evolution must always be compatible with reconstructed structural states:

\[
X(t+1) = I\big(R[P(t)]\big)
\]

subject to continuity and viability constraints.

Evolution cannot contradict reconstruction constraints.

---

### 6.11 Evolution Under Noisy or Irregular Inputs

Even if price movement is:

- highly noisy,  
- discontinuous,  
- irregular in time,

the structural evolution must remain:

- smooth,  
- continuous,  
- stable,  
- theoretically compliant.

Price irregularities must not propagate into structural instability.

---

### 6.12 Injectivity Requirement

The operator must satisfy:

\[
I(X_1) = I(X_2) \quad \Rightarrow \quad X_1 = X_2.
\]

Injectivity ensures:

- uniqueness of reconstructed structural trajectories,  
- determinism of state evolution,  
- absence of ambiguous structural paths.

This requirement ensures consistency with the Reconstruction Uniqueness Theorem.

---

### 6.13 Forbidden Behaviors

The evolution operator must NOT:

- inject noise into structure,  
- create discontinuities in \(X\),  
- increase viability,  
- decrease memory,  
- violate regime ordering,  
- override reconstruction constraints,  
- depend on any observable other than normalized input.

---

## 7. Reconstruction Operator \(R[P]\)

The Reconstruction Operator recovers the internal structural state \(X(t)\) from the normalized observable input stream \((P(t), T(t))\).  
Reconstruction does not invert the projection operator \(\pi\); instead, it identifies the unique structural state compatible with all Flexion constraints.

FMRT must treat reconstruction as a constraint-solving process, not a statistical or data-driven transformation.

---

### 7.1 Operator Definition

Reconstruction is a mapping:

\[
R[P(t)] : (P(t), T(t)) \rightarrow X(t).
\]

It must produce exactly one structural state at each update step.

The operator must be:

- deterministic,  
- invariant-preserving,  
- continuous,  
- consistent with \(I(X)\).  

---

### 7.2 Projection Compatibility

Reconstruction must satisfy:

\[
\pi(X(t)) = P(t).
\]

Since \(\pi\) is many-to-one and non-invertible, reconstruction must rely entirely on structural constraints, not inversion.

---

### 7.3 Structural Validity Constraints

The reconstructed state must satisfy all invariants:

1. **Manifold validity**:  
   \[
   X_\kappa(t) > 0.
   \]

2. **Continuity**:  
   \[
   X(t) \in C^0.
   \]

3. **Memory monotonicity**:  
   \[
   X_M(t) \ge X_M(t-1).
   \]

4. **Viability decay**:  
   \[
   X_\kappa(t) \le X_\kappa(t-1).
   \]

5. **Regime ordering**:  
   ACC → DEV → REL → COLL.

6. **Metric regularity** and **κ-stability**.

A reconstruction that violates any invariant is invalid and must be rejected.

---

### 7.4 Determinism Requirement

Given an identical normalized input stream:

\[
(P(t), T(t)),
\]

reconstruction must always produce the same:

- structural states \(X(t)\),  
- derived quantities (\(\tau, \Pi, R\)),  
- regime classifications.

Nondeterministic reconstruction is prohibited.

---

### 7.5 Constraint-Based Reconstruction

Reconstruction must enforce the following constraints:

#### (1) Continuity constraint  
The state cannot jump:

\[
\|X(t) - X(t-1)\| \text{ must remain small and continuous.}
\]

Price discontinuity cannot cause structural discontinuity.

---

#### (2) Evolution-compatibility constraint  

\[
X(t) \approx I\big(X(t-1)\big)
\]

within an allowable consistency tolerance, ensuring that:

- reconstruction is aligned with structural evolution,  
- evolution and reconstruction do not contradict each other.

---

#### (3) Memory constraint  

\[
X_M(t) \ge X_M(t-1)
\]

with strict enforcement, including compensation for floating-point rounding.

---

#### (4) Viability decay constraint  

\[
X_\kappa(t) \le X_\kappa(t-1),
\qquad
X_\kappa(t) > 0.
\]

---

#### (5) Metric regularity constraint  

Reconstruction must avoid configurations that violate:

- κ-stable neighborhood conditions,  
- finite curvature requirements,  
- metric positivity/regularity.

---

### 7.6 Local Linearization (Optional)

For small admissible perturbations:

\[
dP = \nabla\pi(X) \cdot dX
\]

may be used **only inside κ-stable neighborhoods**.

Outside these neighborhoods, linearization is invalid and must not be applied.

---

### 7.7 Noise Robustness

Observable noise must not distort structural reconstruction.

FMRT must guarantee:

\[
\|R[P + \eta] - R[P]\| \ll \|\eta\|
\]

for sufficiently small \(\eta\).

Noise may distort price, but must never distort structure.

---

### 7.8 Handling Price Discontinuities

If price jumps significantly:

- reconstruction must remain continuous,  
- the new \(X(t)\) must be compatible with invariants,  
- structural geometry must not jump.

Reconstruction must adjust structure *smoothly* regardless of discontinuities in price.

---

### 7.9 Initialization

At the first valid tick:

\[
X(t_0) = R[P(t_0)]
\]

FMRT must initialize:

- memory \(X_M(t_0) \ge 0\),  
- viability \(X_\kappa(t_0) > 0\),  
- tension and differentiation in stable proportions,  
- finite curvature.

Initialization must be deterministic.

---

### 7.10 Reconstruction Uniqueness Requirement

Reconstruction must always yield a **unique** state.

The following must hold:

\[
R_1[P(t)] = R_2[P(t)] \quad \Rightarrow \quad R_1 = R_2.
\]

Uniqueness is enforced by:

- injectivity of evolution \(I\),  
- monotonic memory,  
- viability positivity,  
- continuity,  
- κ-stable metric behavior.

---

### 7.11 Prohibited Behaviors

Reconstruction must NOT:

- smooth price data,  
- average values,  
- fill missing data,  
- infer trends or volatility,  
- use statistical estimation,  
- estimate structure heuristically,  
- rewrite historical states,  
- operate outside the theoretical constraints of FMT 2.2.

FMRT is a structural reconstruction engine, not a predictor or estimator.

---

## 8. Projection Operator \(\pi\)

The projection operator \(\pi\) defines how the internal structural state of the market is mapped to a one-dimensional observable price value.  
Projection is a lossy, nonlinear, and non-invertible transformation.  
FMRT must treat observable price strictly as a projection and must not infer structural properties directly from price behavior.

---

### 8.1 Operator Definition

Projection is defined as:

\[
\pi : X(t) \rightarrow P(t) \in \mathbb{R}.
\]

Thus, observed price is:

\[
P(t) = \pi(X(t)).
\]

This mapping collapses the multi-dimensional structural state into a scalar observable.

---

### 8.2 Fundamental Properties of Projection

Projection has the following properties:

- **Nonlinear**  
- **Many-to-one**  
- **Information-destroying**  
- **Non-injective**  
- **Non-invertible**  
- **Topology-distorting**  
- **Metric-compressing**  
- **Discontinuous in the observable domain**

These properties must always be assumed by FMRT.

---

### 8.3 Dimensional Collapse

Projection reduces the 4D structural state:

\[
(X_\Delta, X_\Phi, X_M, X_\kappa)
\]

to a 1D observable:

\[
P(t) \in \mathbb{R}.
\]

Consequences:

1. Many different internal states may yield the same price.  
2. Price cannot reveal structural time, memory, viability, or curvature.  
3. No structural information can be inferred from price patterns.

Reconstruction must therefore rely on constraints, not inversion.

---

### 8.4 Projection Discontinuity

Even if structure is continuous:

\[
X(t) \in C^0,
\]

projection may produce discontinuities:

\[
P(t) \notin C^0.
\]

FMRT must:

- allow discontinuous price input,  
- prevent projection jumps from creating structural jumps,  
- maintain continuity of \(X(t)\) regardless of behavior of price.

---

### 8.5 Projection Noise

The observable price may include noise:

\[
P'(t) = P(t) + \eta(t).
\]

FMRT must:

- treat \(\eta(t)\) as external and non-structural,  
- ignore noise in structural terms,  
- ensure that noise does not distort reconstruction or evolution.

Projection noise cannot modify structure.

---

### 8.6 Projection Sensitivity

In κ-stable neighborhoods:

\[
dP = \nabla\pi(X) \cdot dX,
\]

meaning price reacts smoothly to structural perturbations.

However, near collapse:

- \(\nabla\pi(X)\) may diverge,  
- projection sensitivity becomes unstable,  
- small structural changes may create large price changes.

FMRT must **never** assume linear projection behavior outside κ-stable regions.

---

### 8.7 Prohibited Interpretations of Price

FMRT must **not** use observable price for:

- trend inference,  
- volatility estimation,  
- statistical smoothing,  
- pattern recognition,  
- directional analysis,  
- prediction of structural transitions.

Price is a projection only — nothing more.

All structure must come from reconstruction, not from observable patterns.

---

### 8.8 Projection as Interface Layer

FMRT must treat projection as:

- a **read-only external view**,  
- a **distorted shadow** of the structure,  
- a **non-structural artifact**,  
- an **input for reconstruction, not interpretation**.

FMRT must not attempt to modify, reinterpret, or compensate projection data except for integrity checks (Section 4).

---

## 9. Structural Time Engine \(\tau\)

The Structural Time Engine defines the internal temporal flow of the market organism as described in FMT 2.2.  
Structural time is not observable, not derived from real-world time, and not related to sampling frequency.  
It is generated entirely by the evolution of memory and represents the intrinsic rate of structural change.

FMRT must compute structural time deterministically and maintain consistency with all invariants of the Flexion manifold.

---

### 9.1 Definition of Structural Time Density

Structural time density is defined by:

\[
\tau(t) = \frac{dX_M(t)}{dt},
\]

where \(X_M(t)\) is the memory component of the structural state.

Thus, structural time is the rate at which irreversible structural imprinting accumulates.

---

### 9.2 Fundamental Properties of Structural Time

Structural time must satisfy:

1. **Non-negativity**  
   \[
   \tau(t) \ge 0.
   \]

2. **Continuity**  
   \[
   \tau(t) \in C^0.
   \]

3. **Dependence exclusively on structure**, not on:
   - observable timeframes,
   - tick frequency,
   - price volatility,
   - external economic clocks.

4. **Irreversibility**  
   Structural time must never decrease.

5. **Regime dependence**  
   Structural time density varies with ACC/DEV/REL/COLL (Section 11).

---

### 9.3 Temporal Density Update Rule

FMRT computes structural time deterministically at each step:

\[
\tau(t) = 
\frac{X_M(t) - X_M(t-1)}
     {T(t) - T(t-1)}.
\]

Requirements:

- timestamps must be used exactly as received,  
- no interpolation or rescaling is allowed,  
- zero or negative \(\Delta T\) is prohibited (Section 4),  
- time density must reflect actual structural change.

---

### 9.4 Relationship to the Structural Cycle

Structural time is directly tied to the structural phase:

- **ACC**:  
  \[
  \tau_{\text{ACC}} \approx 0^{+}
  \]
  (slow structural buildup)

- **DEV**:  
  \[
  \tau_{\text{DEV}} \text{ maximal}
  \]
  (rapid irreversible deformation)

- **REL**:  
  \[
  \tau_{\text{REL}} \text{ decreasing}
  \]

- **COLL**:  
  \[
  \tau_{\text{COLL}} \rightarrow 0
  \]
  (structural time collapses near viability boundary)

FMRT must guarantee consistency between computed \(\tau\) and regime classification.

---

### 9.5 Temporal Consistency Constraints

FMRT must enforce the following:

1. **No negative temporal density**  
   \[
   \tau(t) \ge 0.
   \]

2. **No artificial acceleration**  
   Time density must not spike independently of memory evolution.

3. **No artificial slowdown**  
   Except when dictated by viability decay and collapse geometry.

4. **Continuity of temporal flow**  
   \[
   \tau(t) \in C^0.
   \]

---

### 9.6 Behavior Under Irregular Input Timing

Structural time must remain correct regardless of:

- sparse ticks,  
- dense tick bursts,  
- irregular reception intervals,  
- real-time vs. historical feeds.

FMRT must not interpret:

- tick frequency,  
- observable timing gaps

as structural signals.

Only the change in memory affects \(\tau\).

---

### 9.7 Behavior Near Collapse

As viability approaches zero:

\[
X_\kappa(t) \rightarrow 0,
\]

structural time must collapse:

\[
\tau(t) \rightarrow 0.
\]

This requirement enforces:

- slowdown of internal evolution,  
- stability of reconstruction,  
- proper manifestation of collapse geometry.

FMRT must never allow:

- negative \(\tau\),  
- oscillatory \(\tau\),  
- \(\tau = 0\) outside the COLL state.

---

### 9.8 Deterministic Reproducibility

Given identical input streams:

- FMRT must produce identical \(\tau(t)\) values,  
- across all platforms, environments, and builds,  
- with no nondeterministic floating-point variance.

Temporal determinism is a mandatory requirement.

---

### 9.9 Prohibited Behaviors

Structural time computation must **not**:

- apply smoothing or filtering,  
- rescale timestamps,  
- infer structure from tick frequency,  
- use moving averages or volatility measures,  
- adjust for market sessions or calendar time,  
- depend on irregularities of observable timing.

Structural time is purely intrinsic and must be unaffected by external dynamics.

---

## 10. Viability Engine \(X_\kappa\) and Collapse Geometry

The Viability Engine defines the rules governing the persistence and degradation of the market organism within the structural manifold \(\mathbb{M}^+\).  
Viability represents the remaining intrinsic capacity of the structure to continue evolving.  
Collapse Geometry defines boundary behavior as viability approaches zero.

FMRT must enforce strict viability constraints and guarantee correct collapse dynamics according to FMT 2.2.

---

### 10.1 Viability Definition

Viability is the fourth component of the structural state:

\[
X_\kappa(t) > 0.
\]

It defines whether the structure is still alive and capable of supporting structural evolution.

---

### 10.2 Viability Positivity Requirement

FMRT must guarantee:

\[
X_\kappa(t) > 0 \quad \text{for all valid states}.
\]

Under no circumstances may FMRT:

- produce,  
- accept,  
- or evolve into  

a state with non-positive viability.

Such a state lies outside the manifold \(\mathbb{M}^+\) and represents structural non-existence.

---

### 10.3 Viability Decay Rule

Viability must satisfy:

\[
X_\kappa(t+1) \le X_\kappa(t),
\]

meaning:

- viability never increases,  
- viability is consumed irreversibly as the structure evolves,  
- no reconstruction step may increase viability,  
- no evolution step may create a rebound.

This is a strict invariant of FMT.

---

### 10.4 Collapse Boundary

The collapse boundary is defined as:

\[
X_\kappa = 0.
\]

At this point:

- structural time collapses: \(\tau(t) \rightarrow 0\)  
- curvature diverges,  
- memory stops increasing,  
- evolution becomes undefined,  
- the organism ceases to exist.

FMRT must never cross this boundary.

---

### 10.5 Collapse Proximity (COLL State)

The COLL state represents **approaching collapse**, not collapse itself:

\[
X_\kappa(t) \approx 0^{+}.
\]

Characteristics:

- temporal density collapses: \(\tau(t) \to 0\),  
- curvature increases rapidly,  
- collapse pressure spikes,  
- metric becomes stiff and compressed,  
- evolution slows dramatically.

FMRT must classify the structural cycle as COLL when viability and geometry satisfy these conditions.

---

### 10.6 Collapse Pressure

Collapse pressure is:

\[
\Pi(t) = -\frac{dX_\kappa(t)}{dt}.
\]

Properties:

- \(\Pi(t) \ge 0\),  
- \(\Pi(t)\) increases near collapse,  
- divergence of \(\Pi(t)\) is expected in extreme proximity.

FMRT must compute collapse pressure deterministically for classification purposes.

---

### 10.7 Collapse Gradient

The collapse gradient describes the geometric direction of fastest viability decay:

\[
\nabla \Pi(t) = -\nabla X_\kappa(t).
\]

FMRT uses collapse gradient internally to:

- identify collapse trajectories,  
- enforce stability near collapse,  
- ensure consistent classification.

This value is **internal only** and not part of FMRT output.

---

### 10.8 Curvature Amplification

Near collapse:

\[
X_\kappa(t) \rightarrow 0
\quad \Rightarrow \quad
R(t) \rightarrow \infty,
\]

where \(R(t)\) is structural curvature.

FMRT must ensure:

- reconstruction reflects curvature growth,  
- evolution respects diverging curvature conditions,  
- structural continuity is maintained even under extreme curvature.

Curvature amplification is fundamental to collapse geometry.

---

### 10.9 Temporal Density Collapse

Structural time density must satisfy:

\[
\tau(t) = \frac{dX_M(t)}{dt} \rightarrow 0
\quad \text{as} \quad
X_\kappa(t) \rightarrow 0.
\]

The organism “freezes” as collapse approaches.  
FMRT must enforce and maintain this property.

---

### 10.10 κ-Stable Neighborhoods

FMRT must perform all updates within κ-stable neighborhoods:

- bounded curvature,  
- positive viability,  
- regular metric,  
- consistent evolution dynamics.

Exiting a κ-stable region indicates transition toward COLL.

---

### 10.11 Integrity Constraints Near Collapse

FMRT must enforce:

1. **No structural discontinuities**,  
2. **No backward movement in time**,  
3. **No viability rebound**,  
4. **No negative memory growth**,  
5. **No invalid metric textures**,  
6. **No crossing into \(X_\kappa \le 0\)**.

Collapse must be approached smoothly.

---

### 10.12 Prohibited Behaviors

FMRT must **not**:

- simulate structure beyond collapse,  
- artificially restore viability,  
- alter historical viability values,  
- infer collapse from price movement alone,  
- treat extreme price volatility as collapse indication,  
- reset or reinitialize structure unless explicitly instructed.

Collapse is a structural phenomenon, not an observable event.

---

### 10.13 Summary of Viability Engine Requirements

FMRT must guarantee:

- viability positivity,  
- monotonic viability decay,  
- correct collapse geometry behavior,  
- consistent temporal density collapse,  
- curvature amplification near collapse,  
- unbranched structural evolution within \(\mathbb{M}^+\),  
- deterministic classification of the COLL phase.

This subsystem defines the fundamental limits of structural existence within FMRT.

---

## 11. Structural Cycle (ACC → DEV → REL → COLL)

FMRT classifies each structural state \(X(t)\) into one of the four universal Flexion structural phases.  
These phases are intrinsic properties of the structural manifold and do not correspond to trends, volatility, trading opportunities, or observable price patterns.

The structural cycle governs the internal life rhythm of the market organism and must follow the canonical ordering:

\[
\text{ACC} \rightarrow \text{DEV} \rightarrow \text{REL} \rightarrow \text{COLL}.
\]

Backward transitions are prohibited.

---

### 11.1 Purpose of the Structural Cycle

The Structural Cycle provides:

- a compact descriptor of the current structural condition,  
- the only allowed high-level output state of FMRT,  
- a strict deterministic mapping from structure to phase,  
- a mechanism for preventing invalid or unstable structural transitions,  
- the canonical ordering of structural evolution described in FMT 2.2.

FMRT exposes the structural cycle to external systems as one of its primary outputs.

---

### 11.2 Phase Definitions

The classification is determined by structural geometry, viability, memory growth, and temporal density.

---

#### **ACC — Accumulation Phase**

Properties:

- low temporal density:  
  \[
  \tau(t) \approx 0^{+}
  \]
- tension low or stabilizing,  
- differentiation increasing slowly,  
- viability decays minimally,  
- curvature shallow.

Interpretation:  
The structure is forming initial deformation; evolution is gentle and stable.

---

#### **DEV — Development Phase**

Properties:

- maximal temporal density:  
  \[
  \tau(t) \text{ is high}
  \]
- rapid memory accumulation,  
- strong tension growth:  
  \[
  \frac{dX_\Phi}{dt} \gg 0
  \]
- significant differentiation dynamics,  
- viability decays at an accelerated rate,  
- curvature increases.

Interpretation:  
The organism undergoes rapid deformation; structural dynamics are most intense.

---

#### **REL — Relaxation Phase**

Properties:

- temporal density falling,  
- tension decompressing:  
  \[
  \frac{dX_\Phi}{dt} < 0
  \]
- curvature stabilizing or decreasing,  
- differentiation stabilizes or partially reverts,  
- viability decay slows.

Interpretation:  
The structure relaxes after deformation; internal forces decompress.

---

#### **COLL — Collapse Proximity Phase**

Properties:

- viability approaching zero:  
  \[
  X_\kappa(t) \approx 0^{+}
  \]
- collapse pressure rising dramatically,  
- temporal density collapsing:  
  \[
  \tau(t) \rightarrow 0
  \]
- curvature diverging,  
- metric becomes stiff and compressed.

Interpretation:  
The organism is nearing structural non-existence.

The COLL state does **not** represent price crash or market crash — it is purely structural and invisible in raw price data.

---

### 11.3 Allowed Transitions

FMRT must enforce the following rule:

\[
\text{ACC} \rightarrow \text{DEV} \rightarrow \text{REL} \rightarrow \text{COLL}
\]

Permitted:

- repeating the same state (ACC→ACC, DEV→DEV, REL→REL),  
- skipping REL only if the geometry demands direct DEV→COLL (rare).

Prohibited:

- DEV → ACC  
- REL → ACC  
- REL → DEV  
- COLL → any other state  
- any time-reversed ordering

These rules are strict invariants.

---

### 11.4 Classification Inputs

The phase must be determined **only** from internal structural quantities:

- memory \(X_M\) and its derivative \(\tau(t)\),  
- viability \(X_\kappa\) and collapse pressure \(\Pi(t)\),  
- tension \(X_\Phi\) and its dynamics,  
- differentiation \(X_\Delta\) and curvature indicators.

The observable price \(P(t)\) **must never** be used for phase classification.

---

### 11.5 Deterministic Classification Algorithm

The mapping:

\[
X(t) \rightarrow \text{State}(t)
\]

must be:

- deterministic,  
- continuous in the structural sense,  
- invariant-preserving,  
- platform-independent.

Given identical structural states, classification must always return the same phase.

---

### 11.6 Phase Boundary Behavior

FMRT must ensure:

- smooth transitions between phases,  
- no oscillations between adjacent phases,  
- no phase ambiguity or overlap,  
- monotonic structural evolution toward COLL.

Transitions must follow structural dynamics, not observable price.

---

### 11.7 Structural Cycle and FMRT Output

The structural phase is one of the **two primary outputs** of FMRT:

- **Structural Phase (ACC / DEV / REL / COLL)**  
- **Morphology Class (I / II / III / IV)**

The cycle output provides the real-time structural “life stage” of the market.

---

## 12. Morphology Classification

Morphology describes the structural condition of the market organism at any given moment.  
It characterizes the physical nature of structural deformation and its capacity to respond to internal forces.

Morphology is **independent** of the structural cycle (ACC/DEV/REL/COLL).  
Two structures may belong to the same phase yet exhibit different morphological classes.

FMRT must classify morphology deterministically using tension, curvature, viability, and deformation characteristics.

---

### 12.1 Purpose of Morphology Classification

Morphology provides:

- a second axis of structural description,  
- stability and health indicators of the organism,  
- insight into deformation quality,  
- constraints that influence reconstruction and evolution,  
- additional structural information used by downstream systems.

Morphology is one of the two primary outputs of FMRT.

---

### 12.2 Morphological Classes

FMRT must classify each structural state into one of four morphological classes:

---

#### **Class I — Elastic Morphology**

Properties:

- high structural resilience,  
- low-to-moderate curvature,  
- tension well-distributed,  
- differentiation in stable dynamic range,  
- viability decays slowly,  
- structure responds proportionally to perturbations.

Interpretation:  
The system behaves like an elastic body — deformations are smooth, recoverable, and predictable.

---

#### **Class II — Plastic Morphology**

Properties:

- curvature elevated but finite,  
- tension concentrated in limited regions,  
- differentiation shows directionality,  
- viability consumption increases,  
- partial irreversibility of deformation.

Interpretation:  
The organism behaves plastically — structural changes accumulate and are difficult to reverse.

---

#### **Class III — Degenerate Morphology**

Properties:

- curvature high and unstable,  
- tension exhibits sharp gradients,  
- differentiation distorted or asymmetric,  
- viability decays rapidly,  
- memory accumulation accelerates,  
- sensitivity to perturbations increases dramatically.

Interpretation:  
The structure is close to losing internal stability.  
Degenerate morphology indicates the system is approaching collapse conditions.

---

#### **Class IV — Near-Collapse Morphology**

Properties:

- viability extremely low:  
  \[
  X_\kappa(t) \approx 0^{+}
  \]
- curvature diverging,  
- tension extremely high or collapsing,  
- nearly zero temporal density:  
  \[
  \tau(t) \rightarrow 0
  \]
- extreme stiffness in structural geometry,  
- collapse pressure peaking.

Interpretation:  
The structure is at the geometric edge of existence — immediate neighborhood of COLL state.

---

### 12.3 Independence From Cycle State

Morphology and structural cycle are orthogonal:

- ACC may be Class I or Class II (rarely III),  
- DEV may be Class I, II, or III,  
- REL is typically Class I or II,  
- COLL is usually Class III or IV.

FMRT must not assume any fixed mapping between them.

---

### 12.4 Deterministic Classification

The mapping:

\[
X(t) \rightarrow \text{Morphology}(t)
\]

must be:

- deterministic,  
- continuous where defined,  
- invariant-preserving,  
- dependent only on structural quantities, not price.

Given identical internal states, the morphology must always be identical.

---

### 12.5 Classification Inputs

Morphology may depend only on internal structural variables:

- curvature indicators,  
- tension \(X_\Phi\) and its derivative,  
- differentiation \(X_\Delta\),  
- memory \(X_M\),  
- viability \(X_\kappa\),  
- collapse pressure \(\Pi\),  
- metric stability.

Projection (price) **must not** influence morphological classification.

---

### 12.6 Morphology Constraints

FMRT must guarantee:

- morphology cannot regress numerically without structural justification,  
- transitions must reflect actual internal geometry,  
- degenerate morphology (Class III) must not switch directly back to Class I,  
- Class IV may not transition to any lower class unless viability recovers (which it never does under FMT).

---

### 12.7 Prohibited Behaviors

Morphology classification must **not**:

- use heuristics,  
- use thresholds derived from price volatility,  
- use pattern recognition,  
- estimate morphology from price patterns,  
- rely on external indicators.

It must remain a pure structural classification.

---

## 13. Global Invariants & Constraints

FMRT must enforce a set of global structural invariants that ensure theoretical correctness, numerical stability, and deterministic behavior.  
These invariants apply at every update step, to every reconstructed state, and to every evolved state within the structural manifold \(\mathbb{M}^+\).

Violating any invariant constitutes a critical error requiring rejection of the update or termination of runtime evaluation.

---

### 13.1 Continuity of Structural State

The internal state must satisfy:

\[
X(t) \in C^0.
\]

Meaning:

- no discontinuities,  
- no jumps in tension, differentiation, viability, or memory,  
- price discontinuities must not produce structural discontinuities,  
- reconstruction and evolution must be smooth mappings.

---

### 13.2 Memory Monotonicity

Memory must satisfy:

\[
X_M(t+1) \ge X_M(t).
\]

This invariant must hold:

- exactly,  
- deterministically,  
- with floating-point compensation if required.

Negative changes in memory are strictly forbidden.

---

### 13.3 Viability Positivity

Viability must satisfy:

\[
X_\kappa(t) > 0.
\]

Requirements:

- non-positive viability is invalid,  
- viability may not reach zero during evolution,  
- FMRT must prevent any update that crosses or touches the collapse boundary.

---

### 13.4 Viability Monotonic Decay

Viability must also satisfy:

\[
X_\kappa(t+1) \le X_\kappa(t).
\]

Viability can never increase under any conditions.

This invariant enforces collapse geometry.

---

### 13.5 Temporal Density Consistency

Structural time density must satisfy:

\[
\tau(t) \ge 0,
\qquad
\tau(t) \in C^0.
\]

FMRT must:

- prevent negative temporal flow,  
- prevent artificial acceleration or deceleration not implied by structure,  
- ensure \(\tau(t) \rightarrow 0\) as collapse is approached.

---

### 13.6 Regime Ordering Invariant

The structural cycle must follow:

\[
\text{ACC} \rightarrow \text{DEV} \rightarrow \text{REL} \rightarrow \text{COLL}.
\]

Forbidden transitions:

- DEV → ACC  
- REL → ACC  
- REL → DEV  
- COLL → any prior state  
- any oscillation between adjacent states due to noise  
- skipping phases without structural justification

This ordering must always hold, regardless of observable input behavior.

---

### 13.7 Morphology Consistency

Morphological class must satisfy:

- transitions must be structural, not noise-driven,  
- Class IV may not revert to lower classes unless structure regenerates (but regeneration does not occur under FMT 2.2),  
- morphology must follow curvature, viability, tension, and differentiation relationships.

Morphology must remain consistent across all evolution steps.

---

### 13.8 κ-Stable Neighborhood Constraint

All updates must occur within a κ-stable region:

- bounded curvature,  
- finite metric,  
- no singularities,  
- positive viability,  
- consistent reconstruction tolerance.

If structure approaches κ-instability, FMRT must transition toward REL or COLL as appropriate.

---

### 13.9 Non-Reversibility of Structural Evolution

FMRT must guarantee:

- no rollback of internal state,  
- no rewriting of historical structural values,  
- no regeneration of viability,  
- no reduction of memory,  
- no reversal of structural time.

Structural evolution is strictly irreversible.

---

### 13.10 Deterministic Mapping Invariant

The entire FMRT pipeline must be deterministic:

\[
\{(P(t),T(t))\} \Rightarrow \{X(t)\} \Rightarrow \text{State}(t), \text{Morphology}(t)
\]

Given the same input stream, FMRT must produce:

- identical structural states,  
- identical cycle classifications,  
- identical morphology,  
- identical derived quantities.

Nondeterministic behavior is prohibited.

---

### 13.11 Projection Independence

No structural decision may depend on:

- price momentum,  
- candle patterns,  
- volatility,  
- volume,  
- timeframes,  
- statistical filters.

Structure is determined solely by internal geometry and invariants, not by observable price features.

---

### 13.12 Collapse Geometry Invariant

As viability approaches zero:

- curvature must diverge,  
- temporal density must collapse,  
- tension must reach extreme behavior,  
- structural evolution must slow asymptotically.

FMRT must enforce correct approach to collapse without crossing the boundary.

---

### 13.13 Prohibited Violations

FMRT must not permit:

- negative viability,  
- negative memory increments,  
- discontinuities in the state vector,  
- illegal regime transitions,  
- structural regeneration,  
- retroactive modification of states,  
- structural oscillation induced by price noise,  
- divergence from deterministic behavior.

Any such violation invalidates the update.

---

### 13.14 Summary of Invariants

All of the following must always hold:

- \(X(t) \in C^0\)  
- \(X_M(t+1) \ge X_M(t)\)  
- \(X_\kappa(t) > 0\)  
- \(X_\kappa(t+1) \le X_\kappa(t)\)  
- \(\tau(t) \ge 0\)  
- Proper regime ordering  
- Morphology consistency  
- κ-stability  
- Determinism  
- No projection-driven logic  
- Correct collapse geometry behavior

These invariants form the unbreakable foundation of FMRT.

---

## 14. Output Model

FMRT produces a minimal and strictly defined set of outputs that reflect the internal structural condition of the market organism.  
Outputs must be deterministic, invariant-preserving, and free from any price-derived heuristics.

FMRT exposes only structural information that is explicitly defined by FMT 2.2.

---

### 14.1 Output Categories

FMRT provides **two primary outputs**:

1. **Structural Cycle State**  
2. **Morphology Class**

Additionally, FMRT may expose **optional diagnostic outputs** for debugging and research purposes, but these must never influence runtime classification or reconstruction.

---

### 14.2 Primary Output 1 — Structural Cycle State

FMRT must output the current structural phase:

\[
\text{State}(t) \in \{\text{ACC},\; \text{DEV},\; \text{REL},\; \text{COLL}\}.
\]

This value represents the real-time structural stage of the market organism according to Section 11.

It must be:

- deterministic,  
- unique for each state vector \(X(t)\),  
- invariant-preserving,  
- independent of price patterns or noise.

---

### 14.3 Primary Output 2 — Morphology Class

FMRT must output the current morphological category:

\[
\text{Morphology}(t) \in \{\text{Class I},\; \text{Class II},\; \text{Class III},\; \text{Class IV}\}.
\]

This value describes the physical quality of structural deformation, as defined in Section 12.

---

### 14.4 Optional Diagnostic Outputs (Allowed but Not Required)

FMRT may optionally expose internal values **only for debugging or scientific visualization**:

- structural components:  
  \[
  X_\Delta(t),\; X_\Phi(t),\; X_M(t),\; X_\kappa(t)
  \]
- derived quantities:  
  \[
  \tau(t),\; \Pi(t),\; R(t)
  \]

These diagnostic values:

- must never be treated as signals,  
- must not be used to make trading decisions,  
- must not influence reconstruction or evolution,  
- must be clearly separated from official FMRT outputs.

They exist for developers and researchers only.

---

### 14.5 Forbidden Outputs

FMRT must **not** output:

- buy/sell/hold decisions,  
- indicators or oscillators,  
- price predictions,  
- volatility estimates,  
- statistical metrics,  
- probability distributions,  
- inferred features of price charts,  
- anything derived from smoothing or transforming price.

FMRT is a structural engine, not a strategy engine.

---

### 14.6 Output Determinism Requirements

All outputs must be deterministic:

Given identical input sequences, FMRT must output:

- identical structural cycle states,  
- identical morphology classes,  
- identical diagnostic values (if enabled).

Outputs must be reproducible across:

- platforms,  
- compilers,  
- hardware architectures,  
- runtimes,  
- languages (C++, Rust, Python bridge, etc.)

No nondeterministic or stochastic outputs are permitted.

---

### 14.7 Temporal Alignment

Outputs must correspond **exactly** to the structural state after processing the current tick:

\[
\text{Output}(t) = f(X(t)).
\]

FMRT must not:

- delay classification,  
- anticipate future data,  
- smooth outputs over multiple ticks,  
- revise past outputs after new data arrives.

Output is **strictly real-time** in structural time.

---

### 14.8 Output Completeness

For each valid normalized input tick, FMRT must produce at minimum:

- 1 Structural Cycle State  
- 1 Morphology Class

There is no scenario in which FMRT may return “no output” except when rejecting the tick due to input integrity failure (Section 4).

---

### 14.9 Output Integrity

Outputs must be:

- self-consistent,  
- internally coherent with invariants,  
- compatible with collapse geometry,  
- reflective only of structure,  
- invariant under price scaling or transformation.

Outputs may not depend on:

- tick frequency,  
- observable volatility,  
- noise bursts,  
- external clock effects.

---

### 14.10 Summary

FMRT outputs exactly two canonical structural descriptors:

1. **Structural Cycle State** — the phase of structural evolution  
2. **Morphology Class** — the physical condition of structural deformation

These outputs fully describe the real-time structural condition of the market organism according to FMT 2.2.

---

## 15. Error Conditions & Failure Modes

FMRT must detect, classify, and handle all invalid conditions that would otherwise compromise structural integrity, determinism, or theoretical correctness.  
Errors must be handled deterministically and must never corrupt the internal structural state.

FMRT recognizes three classes of error conditions:

1. **Input-Level Errors** (invalid observable data)  
2. **Structural-Level Errors** (invariant violations)  
3. **Runtime-Level Errors** (numerical or implementation failures)

Each error type has mandatory handling rules.

---

# 15.1 Input-Level Errors

These errors occur before reconstruction and evolution.  
They must be handled by the Input Integrity Layer (Section 4).

---

### 15.1.1 Non-Monotonic Timestamp
Invalid if:
\[
T(t) \le T(t-1).
\]

Required behavior:
- reject tick,
- do not update structure,
- report integrity violation.

---

### 15.1.2 Duplicate Tick  
If both:
- \(T(t) = T(t-1)\),  
- \(P(t) = P(t-1)\),

then the tick must be **ignored** (not an error).

---

### 15.1.3 Invalid Price Value

Tick must be rejected if:

- price is NaN,  
- price is ±Inf,  
- price is zero or negative,  
- price is outside numeric bounds of IEEE-754.

---

### 15.1.4 Unrealistic Jump

If:
\[
|P(t) - P(t-1)| > \text{MaxAllowedDelta}
\]

then tick must be rejected as corrupted.

(*MaxAllowedDelta is implementation-defined but must prevent numeric divergence.*)

---

### 15.1.5 Missing or Corrupted Input Fields  
Any incomplete or malformed input must be rejected.

---

### 15.1.6 Prohibited Data  
If FMRT receives extra fields (volume, OHLC, indicators, etc.), they must be ignored and must not affect structure.

---

---

# 15.2 Structural-Level Errors

These errors occur after reconstruction or during evolution.  
They represent violations of theoretical invariants.

In all structural errors:

- FMRT must **reject the update**,
- preserve the previous valid state,
- report the error deterministically.

---

### 15.2.1 Viability Positivity Violation

Invalid if:

\[
X_\kappa(t) \le 0.
\]

This is a **fatal structural error**.

FMRT must not evolve into or accept such a state.

---

### 15.2.2 Memory Monotonicity Violation

Invalid if:

\[
X_M(t) < X_M(t-1).
\]

This cannot happen in valid Flexion evolution.  
FMRT must roll back to the last valid state and reject the update.

---

### 15.2.3 Discontinuity in the Structural State

Invalid if:

\[
\|X(t) - X(t-1)\| > \text{ContinuityThreshold}.
\]

Threshold is implementation-defined but must guarantee structural smoothness.

---

### 15.2.4 Illegal Regime Transition

FMRT must reject transitions such as:

- DEV → ACC  
- REL → ACC  
- REL → DEV  
- COLL → ACC/DEV/REL  
- any oscillatory transitions due to noise

Violation = invalid state classification.

---

### 15.2.5 Morphology Inconsistency  
Invalid if morphology indicates:

- Class IV → Class I transition,  
- Class III → Class I transition without structural justification,  
- any morphology that contradicts viability or curvature.

---

### 15.2.6 Collapse Boundary Breach

Invalid if reconstruction or evolution yields:

\[
X_\kappa(t) = 0.
\]

This represents structural non-existence.  
FMRT must refuse to evolve beyond this point.

---

### 15.2.7 Metric or Curvature Singularity
Invalid if:

- metric becomes non-positive,  
- curvature becomes numerically undefined,  
- the manifold loses regularity.

Structural singularities are fatal errors.

---

---

# 15.3 Runtime-Level Errors

These include numeric, implementation, or platform-level failures.

FMRT must detect and handle:

---

### 15.3.1 Floating-Point Instability  
Errors include:

- NaN propagation through internal state,  
- Inf results from intermediate computations,  
- catastrophic cancellation,  
- denormal performance traps.

Required action:

- reject update,
- preserve the last valid state.

---

### 15.3.2 Overflow / Underflow

Any internal value escaping representable range must trigger rejection.

---

### 15.3.3 Non-Deterministic Behavior

If FMRT detects nondeterministic outcomes under identical conditions, the runtime must raise a fatal consistency error.

---

### 15.3.4 Memory Corruption or Invalid State Storage

If the state vector is corrupted in memory:

- FMRT must halt evolution and return a critical error.

---

### 15.3.5 Implementation-Level Failures

Any internal exception, segmentation fault, or panic condition must be treated as a fatal failure.

---

---

# 15.4 Error Reporting Rules

FMRT must report errors in a deterministic and consistent format:

Each error report must specify:

- type of error (Input / Structural / Runtime),  
- timestamp of offending tick,  
- reason for rejection,  
- internal diagnostic state (non-optional),  
- recommended fallback behavior (ignore or stop).

---

# 15.5 Update Rejection Behavior

When an error occurs:

- **the update must not be applied**,  
- **the previous structural state must remain unchanged**,  
- **FMRT must continue operating unless the error is fatal**.

Fatal errors include:

- viability ≤ 0,  
- structural singularities,  
- nondeterminism.

---

# 15.6 Summary of Failure Modes

FMRT may only enter the following failure modes:

1. **Non-Fatal Rejection**  
   - tick rejected, state unchanged

2. **Fatal Structural Error**  
   - structural state invalid → FMRT halts

3. **Fatal Runtime Error**  
   - numeric or memory failure → FMRT halts

At no point may FMRT output:

- corrupted state,  
- inconsistent phase,  
- inconsistent morphology,  
- invalid structural metrics.

---

## 16. Runtime & Instance Model

This section defines how FMRT operates as a runtime system, how instances are created and maintained, and how the input stream is processed.  
FMRT is a deterministic streaming engine: it processes one tick at a time and evolves internal structure strictly in chronological order.

FMRT does not support asynchronous updates, multi-stream merging, or parallel structural evolution.

---

### 16.1 Single-Instrument Instance Model

FMRT operates on **one and only one** observable market stream per instance.

A single FMRT instance corresponds to:

- one instrument (symbol),  
- one price stream,  
- one structural state trajectory,  
- one structural organism.

FMRT must not:

- merge two markets into one state,  
- share state between different instruments,  
- infer cross-instrument interactions,  
- synchronize structural states across markets.

Each instrument requires its own FMRT instance.

---

### 16.2 Streaming Processing Model

FMRT processes inputs sequentially:

\[
(P(t), T(t)) \rightarrow \text{FMRT Update} \rightarrow X(t)
\]

Rules:

- no batching of inputs,  
- no reordering of inputs,  
- no skipping allowed once input is accepted,  
- no retroactive modification of past states.

Each update is atomic and must complete before the next one begins.

---

### 16.3 Tick-by-Tick Update Cycle

For each valid incoming tick:

1. **Input Integrity Check** (Section 4)  
2. **Normalization**  
3. **Reconstruction** \(R[P]\) (Section 7)  
4. **Evolution** \(I(X)\) (Section 6)  
5. **Invariant Enforcement** (Section 13)  
6. **Phase + Morphology Classification** (Sections 11–12)  
7. **Output Generation** (Section 14)

This is the canonical FMRT processing pipeline.

---

### 16.4 No Asynchronous or Parallel Updates

FMRT must not:

- update structure in parallel threads,  
- accept inputs that arrive concurrently,  
- mutate the state outside the strict tick order.

FMRT is strictly sequential.

---

### 16.5 Input Latency Tolerance

FMRT must operate correctly regardless of:

- delays between ticks,  
- irregular arrival patterns,  
- bursty or sparse data.

FMRT must not infer structural meaning from tick frequency or timing irregularities.

---

### 16.6 Backtest Mode

In backtesting:

- FMRT receives a historical stream of normalized data,  
- processing is identical to live mode,  
- determinism must hold across all runs.

Backtest ≠ simulation mode.  
Backtest = the same runtime pipeline executed on historical input.

---

### 16.7 Live Mode

In live streaming:

- FMRT must process ticks as they arrive,  
- no lookahead is possible,  
- no assumptions about future input are permitted.

FMRT must behave identically to backtesting in terms of outputs, given the same input sequence.

---

### 16.8 Replay / Recovery Mode

If FMRT is restarted:

- reconstruction must begin from \(X_0\),  
- or from a saved snapshot (if snapshots are implemented externally),  
- input stream must be replayed in order.

FMRT must never attempt heuristic reconstruction of missing history.

---

### 16.9 State Persistence Requirements

The internal state \(X(t)\):

- must persist across updates,  
- must not be overwritten unless reconstruction or evolution define a new state,  
- must never be partially updated.

State corruption is a fatal runtime error.

---

### 16.10 Deterministic Runtime Behavior

For both live and backtest modes:

Given identical inputs:

\[
(P_1, T_1), (P_2, T_2), \ldots
\]

FMRT must always produce identical:

- structural states,  
- phases,  
- morphology classes,  
- derived values.

Non-determinism is forbidden.

---

### 16.11 Prohibited Runtime Behaviors

FMRT must not:

- reorder ticks,  
- interpolate missing ticks,  
- modify timestamps,  
- smooth or transform price,  
- use multithreading for structural updates,  
- share state across markets,  
- adjust structure based on external context,  
- skip ticks after acceptance.

---

### 16.12 Summary

A valid FMRT runtime must satisfy:

- **Single instrument per instance**  
- **Streaming, sequential update model**  
- **Strict chronological order**  
- **One update per tick**  
- **Deterministic reconstruction and evolution**  
- **Identical live/backtest behavior**  
- **Immutable historical state**  
- **No parallelism or asynchronous mutation**

FMRT is a real-time structural engine operating under strict theoretical and runtime constraints.

---

## 17. Numerical Stability & Determinism Requirements

FMRT must maintain strict numerical stability and full determinism across all supported platforms, compilers, and runtime environments.  
All structural computations must be performed in a manner that prevents floating-point anomalies, undefined behavior, or platform-specific divergence.

These requirements ensure correctness, reproducibility, and theoretical compliance.

---

### 17.1 Deterministic Floating-Point Behavior

FMRT must guarantee that:

- identical inputs → identical internal states → identical outputs,  
- IEEE-754 double-precision arithmetic is used consistently,  
- platform and compiler optimizations cannot change numerical results.

The following are **strictly prohibited**:

- nondeterministic math libraries,  
- fused multiply-add differences across hardware,  
- inconsistent rounding modes,  
- stochastic or approximate arithmetic.

---

### 17.2 IEEE-754 Compliance

All internal computations must use:

- **64-bit double precision** for primary structural variables,  
- consistent rounding mode (round-to-nearest even),  
- no “fast-math” optimizations that break IEEE-754 guarantees.

The following compiler flags are forbidden:

- `-ffast-math`  
- `-ffast-math-equivalents`  
- unsafe FMA fusion (unless deterministic across builds)

---

### 17.3 NaN / Inf Prevention

FMRT must ensure that **no structural state contains**:

- NaN,  
- +Inf,  
- -Inf,  
- signaling NaN,  
- uninitialized values.

If any intermediate computation produces NaN or Inf:

- the update must be rejected,  
- the previous valid state must be preserved,  
- a runtime error must be issued.

---

### 17.4 Catastrophic Cancellation Avoidance

FMRT must avoid operations prone to catastrophic cancellation, such as:

- subtracting nearly equal numbers,  
- adding extremely small values to extremely large values,  
- excessive summation error accumulation.

If such operations are unavoidable:

- compensation algorithms (e.g., Kahan summation) must be used,  
- or operations must be reformulated to avoid precision loss.

---

### 17.5 Denormal & Subnormal Handling

Subnormal floats may severely degrade performance or destabilize calculations.

FMRT must:

- detect creation of subnormal values,  
- clamp them to zero when safe,  
- ensure they cannot propagate into the structural state.

Subnormals must never be stored in:

- \(X_\Delta\),  
- \(X_\Phi\),  
- \(X_M\),  
- \(X_\kappa\),  
- derived structural metrics.

---

### 17.6 Overflow and Underflow Protection

FMRT must:

- prevent structural variables from exceeding representable range,  
- prevent exponent overflow in multiplications or exponentials,  
- prevent underflow leading to loss of structural information.

If overflow or underflow occurs:

- update must be rejected,  
- state must remain unchanged,  
- error must be reported.

---

### 17.7 Numeric Stability of Invariants

All invariants from Section 13 must remain numerically stable:

1. **Memory monotonicity**  
   \[
   X_M(t+1) \ge X_M(t)
   \]

2. **Viability positivity**  
   \[
   X_\kappa(t) > 0
   \]

3. **Viability decay**  
   \[
   X_\kappa(t+1) \le X_\kappa(t)
   \]

FMRT must apply:

- epsilon-tolerant comparisons,  
- rounding-safe updates,  
- monotonicity guards.

This ensures invariants cannot be violated by floating-point noise.

---

### 17.8 Numeric Stability of Reconstruction

Reconstruction must:

- avoid division by small denominators,  
- avoid amplification of price noise,  
- reject structurally impossible results,  
- guarantee continuity of the state vector.

If projection sensitivity diverges (near collapse):

- reconstruction must fall back to invariant-preserving mode.

---

### 17.9 Numeric Stability of Evolution

Evolution must:

- maintain continuity constraints,  
- avoid unstable curvature growth,  
- ensure consistent decay of viability,  
- prevent tension or differentiation from becoming numerically unstable.

If curvature exceeds representable limits, the update must be rejected.

---

### 17.10 Deterministic Order of Operations

FMRT must enforce a strict execution order:

- operations must not be reordered by compiler optimizations,  
- parallelism is prohibited for structural updates,  
- operations must produce identical results across builds.

The system must behave identically across:

- debug / release builds,  
- CPU architectures,  
- operating systems,  
- compilers (GCC, Clang, MSVC),  
- hardware floating-point units.

---

### 17.11 Hashability of State

To verify determinism:

- the state vector \(X(t)\) must be hashable,  
- identical inputs must produce identical hashes at each step,  
- this must hold across platforms.

This enables determinism audits.

---

### 17.12 Forbidden Numeric Behaviors

FMRT must **never** allow:

- stochastic numeric approximations,  
- randomness or nondeterministic noise injection,  
- architecture-dependent math functions,  
- GPU implementations that reduce precision,  
- approximate or accelerated math.

---

### 17.13 Summary of Requirements

FMRT must guarantee:

- strict IEEE-754 double-precision arithmetic,  
- deterministic behavior across platforms,  
- prevention of NaN/Inf/subnormal propagation,  
- stable enforcement of invariants,  
- numeric robustness near collapse,  
- rejection of updates that violate numeric safety,  
- total reproducibility under identical inputs.

These requirements ensure that FMRT is a fully deterministic structural engine compliant with FMT 2.2.

---

## 18. Formal Specification Summary

The FMRT Core is a deterministic, invariant-preserving structural engine derived from Flexion Market Theory (FMT) Version 2.2.  
It reconstructs, evolves, and classifies the internal structural state of a market organism using only normalized price–timestamp inputs.

The specification defines the complete theoretical, operational, and numerical requirements for FMRT.

---

### 18.1 Input Requirements

FMRT accepts only:

\[
(P(t), T(t))
\]

via a strict streaming model.  
Inputs must pass integrity checks for:

- timestamp monotonicity,  
- finite numeric price values,  
- duplication,  
- corruption or malformed data.

All invalid ticks must be rejected deterministically.

---

### 18.2 Internal State

The structural state is:

\[
X(t) = (X_\Delta, X_\Phi, X_M, X_\kappa) \in \mathbb{M}^+.
\]

FMRT must guarantee:

- continuity \(X(t) \in C^0\),  
- memory monotonicity \(X_M(t+1) \ge X_M(t)\),  
- viability positivity \(X_\kappa(t) > 0\),  
- viability decay \(X_\kappa(t+1) \le X_\kappa(t)\),  
- κ-stable metric behavior.

---

### 18.3 Reconstruction & Evolution

FMRT implements the structural operators:

- **Reconstruction**  
  \[
  X(t) = R[P(t)]
  \]
- **Evolution**  
  \[
  X(t+1) = I(X(t)]
  \]

Both operators must be:

- deterministic,  
- continuous,  
- invariant-preserving,  
- compatible with collapse geometry,  
- immune to price noise and discontinuities.

Reconstruction is constraint-based, not inversion-based.

---

### 18.4 Structural Time & Viability

Structural time:

\[
\tau(t) = \frac{dX_M(t)}{dt}
\]

must be non-negative and continuous.

Viability defines collapse geometry and must satisfy:

\[
X_\kappa(t) > 0,
\qquad
X_\kappa(t+1) \le X_\kappa(t).
\]

Near collapse:

- \(\tau \rightarrow 0\),  
- curvature diverges,  
- collapse pressure increases.

---

### 18.5 Structural Cycle Classification

FMRT must classify structure into:

\[
\text{ACC},\; \text{DEV},\; \text{REL},\; \text{COLL},
\]

with strict ordering:

\[
\text{ACC} \to \text{DEV} \to \text{REL} \to \text{COLL}.
\]

Illegal transitions are prohibited.

---

### 18.6 Morphology Classification

FMRT must classify morphology into:

- Class I — Elastic  
- Class II — Plastic  
- Class III — Degenerate  
- Class IV — Near-Collapse

Morphology is independent of structural cycle.

---

### 18.7 Output Model

FMRT outputs exactly:

1. **Structural Cycle State**  
2. **Morphology Class**

Optional diagnostic values may be exposed but must not affect structural logic.

No trading signals, predictions, or heuristics may be produced.

---

### 18.8 Runtime & Determinism

FMRT must operate:

- as a single-instrument instance,  
- in a strictly sequential streaming model,  
- with identical behavior in live and backtest modes,  
- under full determinism across platforms and builds.

No parallelism or asynchronous mutation is permitted.

---

### 18.9 Numerical Requirements

FMRT must ensure:

- IEEE-754 double-precision arithmetic,  
- no NaN/Inf/subnormal propagation,  
- no overflow/underflow in structural variables,  
- deterministic order of operations,  
- stable invariant enforcement.

Numeric failure = rejected update or fatal error.

---

### 18.10 Final Compliance Statement

A valid FMRT implementation must:

- accept only normalized input,  
- maintain structural continuity,  
- enforce all theoretical invariants,  
- reconstruct structure uniquely,  
- evolve structure deterministically,  
- classify structure and morphology correctly,  
- ensure collapse-consistent behavior,  
- guarantee platform-independent determinism.

Any violation of these rules constitutes non-compliance with FMRT-Specification-V1.0-EN.
