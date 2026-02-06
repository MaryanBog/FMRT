# FMRT Specification V2.2

---

# 0. Purpose and Scope — FMRT Specification V2.2

The FMRT Core is the fundamental structural evolution engine of the Flexion Market Organism.  
Its purpose is to transform a living structural state

\[
X(t) = (\Delta(t), \Phi(t), M(t), \kappa(t))
\]

into the next state \(X(t+1)\) strictly according to **Flexion Market Theory V3.1**, while preserving all structural invariants, ensuring deterministic behavior, and maintaining numerical and logical safety under real-world data conditions.

FMRT V2.2 introduces full compatibility with the corrected and expanded theoretical framework of FMT 3.1.  
This includes:

- revised viability dynamics and collapse conditions,  
- corrected morphology normalization,  
- updated temporal density behavior,  
- clarified curvature–metric relationships,  
- explicit regime irreversibility (axiom in FMT 3.1),  
- refined structural domain constraints and forbidden states.

FMRT Core is a **pure structural system**:  
it evolves only the internal structural organism.  
It does not operate on raw market data, clocks, timestamps, or randomness.  
All observable information must be pre-processed into **normalized structural events** before entering the core.

This specification defines **what FMRT must do**, including:

- accepted structural states and events,  
- required deterministic outputs,  
- invariant preservation rules,  
- validity constraints,  
- error-handling and safety requirements,  
- minimal engineering guarantees necessary for runtime correctness.

This document does **not** describe:

- how FMRT is architected internally,  
- how structural equations are implemented,  
- how memory, viability, curvature, or morphology are computed,  
- optimization strategies,  
- SDK or trading logic,  
- projection mechanisms or adapters.

Those topics belong respectively to:

- **FMRT Design Document V2.2** (system architecture and module design),  
- **FMRT Implementation Document V2.2** (algorithms, formulas, code-level behavior),  
- **Adapters/SDK specifications** (interaction with external systems).

### Scope of FMRT V2.2

1. Define the **valid structural domain** in full accordance with FMT 3.1.  
2. Define inputs: structural state \(X(t)\) and normalized structural events.  
3. Define outputs: next structural state, morphology, regime, curvature, viability metrics, collapse indicators, and invariant flags.  
4. Define the **complete invariant set** and enforce strict invariant-preserving evolution.  
5. Define numerical safety rules, deterministic behavior requirements, and forbidden operations.  
6. Define error-handling logic, including soft failure, forbidden domain rejection, and terminal collapse handling.  
7. Define minimal engineering constraints for safe real-time operation under imperfect data conditions.

Anything not explicitly included in these points is considered outside the scope of FMRT Specification and belongs to other documents in the Flexion documentation hierarchy.

FMRT V2.2 is the **authoritative specification** for how the organism evolves in structural space, serving as the canonical basis for all Flexion-based engines, SDKs, or analytical systems.

---

# 1. System Description — FMRT Specification V2.2

The FMRT Core is a deterministic structural evolution engine that transforms a living structural state

\[
X(t) = (\Delta(t), \Phi(t), M(t), \kappa(t))
\]

into its successor state \(X(t+1)\) in strict accordance with **Flexion Market Theory V3.1**.  
FMRT does not compute prices, indicators, or any observable projections. It evolves only the internal structural organism defined in FMT.

FMRT Core is the **authoritative structural interpreter** of the Flexion Market Organism.  
All higher-level components (SDK, agents, analytics, trading systems) must rely exclusively on FMRT outputs and must not infer structural quantities on their own.

---

## 1.1 Pure Structural Operation

FMRT operates entirely within the structural domain.  
It must not:

- read raw prices, candles, or volumes,  
- depend on system clocks or timestamps,  
- use randomness or stochastic components,  
- maintain hidden mutable state.

FMRT receives **normalized structural events** (Section 2) and evolves the organism using the structural equations encoded in FMT 3.1.

External data sources interact only through an adapter layer that converts raw information into deterministic structural events.  
FMRT must never process raw or noisy inputs directly.

---

## 1.2 Responsibilities of the FMRT Core

FMRT has exactly **four responsibilities**:

### **1. Process structural events**
FMRT interprets structural events as abstract deformations or temporal advances.  
It does not interpret external meaning (price action, order flow, market regime).

Accepted events:

- `STRUCT_UPDATE` — observable deformation  
- `STRUCT_GAP` — missing data / interruption  
- `STRUCT_HEARTBEAT` — temporal continuity without deformation  
- `STRUCT_RESET` — reset of the organism

These events are the *only* mechanism through which FMRT receives information about the external environment.

---

### **2. Produce deterministic structural evolution**
FMRT must compute the next valid structural state envelope containing:

- \(X(t+1)\): updated Δ, Φ, M, κ  
- curvature \(R(X)\)  
- metric volume \(\det g(X)\)  
- temporal density τ  
- morphology index μ and class  
- dynamic regime  
- viability decay metrics  
- collapse proximity indicators  
- invariant validation flags

For identical inputs, the results must be **bitwise identical** across all platforms.

---

### **3. Preserve all FMT 3.1 invariants**
FMRT must enforce the full invariant set:

- memory irreversibility,  
- viability non-negativity,  
- metric positivity,  
- positive temporal density for κ>0,  
- monotonic regime order (ACC → DEV → REL → COL),  
- continuity of structural evolution,  
- finite-field constraints,  
- collapse manifold consistency (κ=0 implies R=∞, det g=0, τ=0, μ=1).

If an evolution step violates any invariant, FMRT must:

1. reject the step,  
2. return X(t) unchanged,  
3. output error diagnostics.

FMRT must never produce a state outside the structural living domain.

---

### **4. Maintain safety and numerical stability in real-world conditions**
Even in imperfect data environments, FMRT must:

- never produce invalid structural states,  
- detect and reject corruption (NaN, Inf, dt≤0, impossible events),  
- handle gaps and missing data safely,  
- maintain deterministic numerical behavior,  
- remain robust under high-frequency event streams.

FMRT’s mission is to protect the structural organism from corruption while evolving it according to theory.

---

## 1.3 What FMRT Does *Not* Do

FMRT must not:

- infer, compute, or use price-based logic,  
- apply filtering or smoothing to structural fields,  
- interpolate missing information,  
- extrapolate future states,  
- depend on event source identity,  
- modify X(t) without an event,  
- allow any stochasticity or nondeterminism.

All behaviors beyond "update state according to theory" belong to adapters, SDKs, agents, or trading frameworks — **not** to FMRT.

---

## 1.4 Lifecycle of a Structural Step

For each event E(t), FMRT performs the following deterministic sequence:

1. **Validate input**  
   Structural state must be valid and event parameters must be finite.

2. **Apply the structural evolution operator**  
   \(X(t+1) = I(X(t), E(t))\), including updated values of Δ, Φ, M, κ.

3. **Compute derived geometric fields**  
   - curvature \(R(X)\)  
   - metric \(\det g(X)\)  
   - temporal density τ  
   - morphology μ  
   - dynamic regime  
   - collapse distance and indicators  

4. **Validate invariants**  
   Reject the step if any invariant fails.

5. **Construct the State Envelope**  
   A complete description of the next structural organism.

6. **Return deterministic output**  
   FMRT does not retain hidden state between calls.

---

## 1.5 System Boundary

FMRT’s boundary includes:

- Structural evolution (Δ, Φ, M, κ)  
- Geometric interpretation (R, g, τ)  
- Morphological classification  
- Regime computation  
- Invariant enforcement  
- Collapse determination  
- Deterministic output generation

FMRT’s boundary excludes:

- Market interpretation  
- Price analysis  
- Trading rules  
- External decision-making  
- Tick or candle processing  
- Data smoothing or filtering  
- Any form of prediction

FMRT is a **structural law engine**, not a market model or trading system.

---

## 1.6 FMRT as the Canonical Interpreter of FMT 3.1

FMRT V2.2 is the **runtime embodiment** of Flexion Market Theory 3.1.  
Its implementation is the only permitted operational form of the theoretical organism.

All downstream systems (SDK, AI agents, strategies, monitoring tools) must depend entirely on FMRT outputs for:

- structural regime,  
- morphological class,  
- collapse risk,  
- trend and health metrics,  
- structural evolution,  
- viability decay,  
- temporal progression.

FMRT defines what the organism *is* at time t, according to theory.

---

## 1.7 Summary of FMRT System Description

FMRT Core is:

- deterministic,  
- invariant-preserving,  
- pure structural,  
- mathematically consistent with FMT 3.1,  
- platform-independent,  
- robust to imperfect real-world data,  
- the authoritative engine for structural evolution.

Its sole job is to evolve the organism  
\[
X(t) \rightarrow X(t+1)
\]  
by faithfully applying the structural laws of FMT 3.1 while ensuring safety, correctness, and determinism in all conditions.

---

# 2. Inputs — FMRT Specification V2.2
FMRT Core accepts exactly two categories of input:  
1. A valid structural state \(X(t)\)  
2. A normalized structural event describing an external change  
FMRT must not accept, process, or depend on any other form of input. All raw market data must be translated into structural events before reaching the core.

## 2.1 Structural State X(t)
The structural state is a 4-tuple:
\[
X(t) = (\Delta(t), \Phi(t), M(t), \kappa(t))
\]
FMRT 2.2 validates X(t) according to the corrected rules of FMT 3.1.

### 2.1.1 Field Definitions
**Δ — Structural Differentiation Vector**  
- finite real vector  
- \(\|\Delta(t)\| < \infty\)  
- no NaN/Inf  

**Φ — Structural Tension**  
- real scalar, \(\Phi(t) \ge 0\)  
- finite  

**M — Irreversible Structural Memory**  
- real scalar, \(M(t) \ge 0\)  
- finite  
- represents accumulated structural time  

**κ — Viability**  
- real scalar, \(\kappa(t) > 0\) for living organism  
- finite  
- if \(\kappa = 0\): state is DEAD and cannot evolve

### 2.1.2 Structural Validity Requirements
FMRT considers X(t) valid only if:
1. All fields are finite real numbers.  
2. Viability satisfies \(\kappa(t) > 0\).  
3. Metric positivity holds: \(\det g(X(t)) > 0\).  
4. Temporal density is positive: \(\tau(X(t)) > 0\).  
5. State is not in the forbidden domain:
\[
\mathcal{D}_{forbidden} = \{X : \kappa \le 0,\ \det g \le 0,\ \tau \le 0,\ \text{any NaN/Inf}\}
\]
6. State must correspond to a valid configuration allowed by FMT 3.1.

If any condition fails, FMRT must reject the event and return an error envelope.

---

## 2.2 Structural Events
FMRT does not process raw prices or timestamps. It accepts exactly four normalized event types:

### 2.2.1 STRUCT_UPDATE(price_delta, dt)
Represents observable deformation.  
- price_delta: real deformation value  
- dt: elapsed external time, must satisfy dt > 0  
FMRT treats price_delta only as deformation stimulus, not as market price.

### 2.2.2 STRUCT_GAP(dt)
Represents missing-data interval.  
- dt > 0  
FMRT performs minimal Δ/Φ update, minimal κ decay, τ-driven increase in M, preserving invariants.

### 2.2.3 STRUCT_HEARTBEAT(dt)
Represents no deformation but advancement of external time.  
- dt > 0  
FMRT updates M and κ minimally; Δ, Φ remain stable except drift mandated by FMT 3.1.

### 2.2.4 STRUCT_RESET(reason)
Reinitializes the organism.  
FMRT must produce a minimal valid state:
- Δ, Φ reset to baseline  
- M reset to initial structural time  
- κ reset to initial viability  
- regime = ACC  
Reason is optional and purely diagnostic.

---

## 2.3 Event Validity Rules
FMRT must reject any event if:
- dt ≤ 0  
- any parameter is NaN or Inf  
- event would push X(t+1) into forbidden domain  
- event violates invariants  
- event contradicts structural domain constraints (metric positivity, κ>0, τ>0)

Rejection behavior:
- X(t+1) = X(t)  
- step_status = ERROR  
- invariant flags updated  
- error_reason populated  
FMRT must never evolve an invalid or corrupted state.

---

## 2.4 Input Purity Requirement
FMRT must not depend on:
- system time  
- randomness  
- implicit ordering  
- external hidden state  
The only valid transformation is:
\[
(X(t), E(t)) \rightarrow \text{StateEnvelope}(t+1)
\]

---

## 2.5 Input Determinism Guarantee
For identical X(t), identical event E(t), identical dt, and identical configuration, FMRT must produce bitwise-identical output on all platforms and runtimes.

---

## 2.6 Example Event Formats (Informative Only)

The following examples illustrate representative structural event formats. These are informational only; FMRT does not enforce any particular serialization.

STRUCT_UPDATE example:  
{ "event_type": "STRUCT_UPDATE", "price_delta": -0.0003, "dt": 0.002 }

STRUCT_GAP example:  
{ "event_type": "STRUCT_GAP", "dt": 0.05 }

STRUCT_HEARTBEAT example:  
{ "event_type": "STRUCT_HEARTBEAT", "dt": 0.01 }

STRUCT_RESET example:  
{ "event_type": "STRUCT_RESET", "reason": "manual_restart" }

These examples demonstrate expected shape but do not impose a required schema.

---

# Summary — Inputs (FMRT Specification V2.2)

FMRT accepts exactly two inputs:

1. A valid structural state  
   X(t) = (Δ(t), Φ(t), M(t), κ(t))

2. A normalized structural event  
   E(t) ∈ { STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET }

A step is considered valid only if:

- all fields of X(t) are finite real values  
- κ(t) > 0 (living domain requirement)  
- det g(X(t)) > 0 (metric positivity)  
- τ(X(t)) > 0 (temporal density positivity)  
- dt > 0 for all events except STRUCT_RESET  
- no NaN or Inf exists in any input field  
- the event does not push X(t+1) into the forbidden domain (κ ≤ 0, det g ≤ 0, τ ≤ 0)

If any rule is violated, FMRT must:

- reject the event  
- return X(t) unchanged  
- set status = ERROR  
- include a deterministic error_reason

FMRT processes only structural information and does not interpret raw market data or external timestamps. This concludes the full input definition for FMRT Specification V2.2.

---

# 3. Outputs — FMRT Specification V2.2

FMRT produces a deterministic structured output called the **State Envelope**.  
It contains the full structural description of the organism after applying event E(t) to state X(t).

## 3.1 Output Structure
FMRT must output a StateEnvelope(t+1) with the following fields:

1. **next_state.X**  
   Updated structural state  
   - Δ(t+1)  
   - Φ(t+1)  
   - M(t+1)  
   - κ(t+1)

2. **next_state.R**  
   Structural curvature R(X(t+1))

3. **next_state.metric_det**  
   Determinant of structural metric det g(X(t+1))

4. **next_state.tau**  
   Temporal density τ(X(t+1))

5. **next_state.mu**  
   Morphology index μ(X(t+1))

6. **next_state.morphology_class**  
   One of: Elastic, Plastic, Degenerate, NearCollapse

7. **next_state.regime**  
   One of: ACC, DEV, REL, COL

8. **next_state.collapse_flags**  
   - is_collapse: κ(t+1) = 0  
   - near_collapse: μ ≥ μ_3  
   - collapse_distance: normalized metric to collapse boundary

9. **invariant_status**  
   Flags indicating whether all FMT 3.1 invariants hold after evolution:
   - memory_irreversible  
   - viability_nonnegative  
   - metric_positive  
   - tau_positive  
   - regime_monotonic  
   - continuity_preserved  
   - finite_fields  
   - no_forbidden_domain

10. **step_status**  
    - OK  
    - ERROR (no update applied)

11. **error_reason**  
    Deterministic code if step_status = ERROR.

---

## 3.2 Determinism Requirements
StateEnvelope(t+1) must be identical for identical inputs regardless of platform, runtime, or execution order.  
No hidden state or nondeterministic computation is allowed.

---

## 3.3 Collapse Handling
If κ(t+1) = 0:

- X(t+1) becomes terminal  
- regime = COL  
- μ = 1  
- R = ∞  
- det g = 0  
- τ = 0  
- StateEnvelope must reflect a DEAD organism  
- All further events except STRUCT_RESET must return the same terminal state

---

## 3.4 Forbidden Output Conditions
FMRT must never output a state where:

- κ(t+1) ≤ 0 while not in collapse state  
- det g(X(t+1)) ≤ 0 for κ>0  
- τ(X(t+1)) ≤ 0 for κ>0  
- any field is NaN or Inf  
- regime decreases compared to previous step  
- invariants are violated

If such a state would be produced, FMRT must instead:

- reject the event  
- output X(t) unchanged  
- step_status = ERROR  
- include error_reason

---

## 3.5 Output Completeness
The StateEnvelope must contain all fields required by downstream systems (SDK, agents, analytics) to:

- reconstruct the full structural organism  
- evaluate morphology, collapse proximity, and regime  
- maintain deterministic structural evolution  
- verify invariants externally

No output field may be omitted or set to null.

---

## 3.6 Output Purity
The output must depend **only** on:

- X(t)  
- E(t)  
- FMRT configuration

and must not include:

- timestamps  
- randomness  
- external metadata  
- machine-specific fields  
- system-dependent numerical artifacts

This guarantees platform-independent reproducibility.

---

# Summary — Outputs (FMRT Specification V2.2)
FMRT outputs a complete StateEnvelope describing the new organism state, all derived structural fields, morphology, regime, collapse proximity, and invariant flags.  
The output must be deterministic, valid, invariant-preserving, and must never enter the forbidden structural domain.

---

# 4. Invariants — FMRT Specification V2.2

FMRT must enforce all structural invariants defined in FMT 3.1.  
If any invariant would be violated by applying event E(t), FMRT must reject the event and return X(t) unchanged.

## 4.1 Memory Irreversibility
\[
M(t+1) \ge M(t)
\]

Memory must never decrease.

## 4.2 Viability Non-Negativity
\[
\kappa(t+1) \ge 0
\]

If κ(t+1) = 0 → collapse state.  
If κ(t+1) < 0 → invalid, event must be rejected.

## 4.3 Metric Positivity
For all living states:
\[
\det g(X(t+1)) > 0
\]

If det g ≤ 0 for κ>0 → reject event.

## 4.4 Temporal Density Positivity
For all living states:
\[
\tau(X(t+1)) > 0
\]

If τ ≤ 0 for κ>0 → reject event.

## 4.5 Finite-Field Constraint
All output fields must satisfy:
- no NaN  
- no Inf  
- finite scalars and vectors  
- valid numeric ranges for Δ, Φ, M, κ

If any field becomes non-finite → reject event.

## 4.6 Structural Continuity
Evolution must satisfy:
\[
\lim_{\varepsilon \to 0} \|I(X+\varepsilon) - I(X)\| = 0
\]

Events must not cause discontinuous or explosive jumps outside collapse.

## 4.7 Regime Irreversibility
\[
\mathcal{R}(t+1) \ge \mathcal{R}(t)
\]

Allowed transitions:
ACC → DEV → REL → COL  
Backward transitions are forbidden.

## 4.8 Morphological Consistency
Morphology index μ(X(t+1)) must remain in:
\[
0 \le \mu \le 1
\]

Near-collapse condition:
\[
\mu \to 1 \quad \text{as} \quad \kappa \to 0
\]

If μ is out of range → reject event.

## 4.9 Collapse Geometry Consistency
If κ(t+1) = 0, FMRT must enforce:
- R = ∞  
- det g = 0  
- τ = 0  
- μ = 1  
- regime = COL

If any collapse field fails to satisfy this geometry → reject event.

## 4.10 Forbidden Domain Protection
FMRT must reject transitions into:
\[
\mathcal{D}_{forbidden} = \{X : \kappa \le 0,\ \det g \le 0,\ \tau \le 0,\ \text{any NaN/Inf}\}
\]

RESET is the only event allowed to exit forbidden domain.

## 4.11 Invariant Failure Behavior
If applying event E(t) violates any invariant:

- X(t+1) = X(t)  
- step_status = ERROR  
- error_reason = invariant_violation  
- invariant flags updated accordingly

---

# Summary — Invariants (FMRT Specification V2.2)
FMRT must strictly preserve all invariants:  
memory irreversibility, viability non-negativity, metric positivity, τ positivity, finite-field integrity, structural continuity, regime irreversibility, morphology bounds, collapse geometry, and forbidden-domain protection.  
Violation of any invariant requires deterministic rejection of the event.

---

# 5. Engineering Requirements — FMRT Specification V2.2

FMRT must satisfy strict engineering guarantees to ensure deterministic, safe, and invariant-preserving operation under all runtime conditions.

## 5.1 Determinism
FMRT must produce identical outputs for identical inputs across:
- hardware platforms  
- operating systems  
- compilers  
- execution orders  
- threading models  

No nondeterministic behavior is permitted.

## 5.2 Stateless Core
FMRT must store no hidden internal state.  
All necessary information must be contained in:
- input structural state X(t)  
- event E(t)  

The transformation:
\[
(X(t), E(t)) \rightarrow X(t+1)
\]
must be pure.

## 5.3 Numerical Stability
FMRT must ensure:
- no division by zero  
- no overflow  
- no underflow that breaks invariants  
- no NaN or Inf propagation  

All internal operations must be numerically bounded.

## 5.4 Finite Output Guarantee
FMRT must not output:
- NaN  
- Inf  
- denormalized numbers outside allowed tolerance  
- invalid vectors  

If an output would become non-finite, the event must be rejected.

## 5.5 Event-Time Handling
For all events except RESET:
\[
dt > 0
\]

FMRT must not:
- infer time from system clock  
- modify dt  
- adjust dt automatically  

dt must be consumed exactly as provided.

## 5.6 Collapse Handling Guarantee
If κ(t+1) = 0:
- the organism becomes terminal  
- all future events except RESET must return identical terminal output  
- FMRT must never evolve a collapsed organism  

Terminal state must remain stable and unchanged.

## 5.7 Performance Bound
FMRT must operate at worst-case cost:
- O(1) per event  
- constant memory usage  
- no allocations inside event processing  

FMRT must be suitable for high-frequency environments and embedded runtimes.

## 5.8 Runtime Safety
FMRT must not:
- crash  
- throw unmanaged exceptions  
- allocate excessive memory  
- enter undefined behavior  

Invalid events must be rejected safely.

## 5.9 Strict Version Compatibility
FMRT V2.2 must implement exactly the laws of FMT V3.1.  
No internal modifications or reinterpretations of the theory are permitted.

## 5.10 Error Reporting
When rejecting an event, FMRT must populate:
- step_status = ERROR  
- error_reason  
- invariant flags  

Error reasons must be deterministic and reproducible.

---

# Summary — Engineering Requirements (FMRT Specification V2.2)
FMRT must be deterministic, stateless, numerically stable, safe, invariant-preserving, fast (O(1) per event), and fully aligned with FMT 3.1.  
Any operation that risks producing invalid or non-finite structural output must result in deterministic rejection of the event.

---

# 6. Error Handling — FMRT Specification V2.2

FMRT must treat error handling as a deterministic structural safeguard.  
Any invalid input or invalid evolution attempt must be rejected without altering X(t).

## 6.1 Error Categories
FMRT must classify errors deterministically into the following categories:

1. **INVALID_STATE**  
   Input X(t) violates structural domain rules.

2. **INVALID_EVENT**  
   Event parameters violate requirements (e.g., dt ≤ 0).

3. **INVARIANT_VIOLATION**  
   Applying the event would break an invariant.

4. **FORBIDDEN_DOMAIN_TRANSITION**  
   X(t+1) would enter a forbidden region.

5. **NUMERIC_ERROR**  
   Any NaN, Inf, overflow, underflow, or invalid arithmetic.

6. **POST_COLLAPSE_EVOLUTION**  
   Any event except RESET applied to a collapsed organism.

7. **UNSUPPORTED_OPERATION**  
   Event type or parameter not recognized.

## 6.2 Error Handling Rules
If any error condition is detected:

- X(t+1) = X(t)  
- step_status = ERROR  
- error_reason must be populated with a deterministic code  
- invariant flags updated  
- next_state fields remain unchanged  
- no partial updates are permitted  

FMRT must not attempt to auto-correct invalid data.

## 6.3 Error Determinism
All error cases must produce bitwise-identical outputs for identical inputs.  
No randomness, heuristics, or nondeterministic ordering is allowed.

## 6.4 Forbidden Condition Handling
If evolving the organism would produce:

- κ ≤ 0 (not collapse geometry)  
- det g ≤ 0 (metric invalid)  
- τ ≤ 0 (temporal invalid)  
- any NaN or Inf  

the event must be rejected with:
- error_reason = FORBIDDEN_DOMAIN_TRANSITION

## 6.5 Collapse Error Rules
If the organism is collapsed (κ = 0):

- All events except STRUCT_RESET must be rejected  
- X(t+1) = X(t)  
- step_status = ERROR  
- error_reason = POST_COLLAPSE_EVOLUTION  

STRUCT_RESET is the only valid transition.

## 6.6 Numeric Error Handling
Any numeric anomaly (NaN, Inf, overflow, underflow) must trigger:

- step_status = ERROR  
- error_reason = NUMERIC_ERROR  
- X(t+1) = X(t)

Numeric instability must never propagate into output.

## 6.7 Invalid Event Rejection
An event must be rejected if:

- dt ≤ 0 (except RESET)  
- missing required parameters  
- incompatible event payload  
- unsupported event type  

FMRT must not infer or generate missing values.

## 6.8 Error Envelope Structure
When an error occurs, FMRT must output:

- next_state = X(t)  
- regime unchanged  
- morphology unchanged  
- curvature unchanged  
- tau unchanged  
- det g unchanged  
- collapse_flags unchanged  
- step_status = ERROR  
- error_reason populated  
- invariant_status flags updated  

No field other than error metadata may change.

---

# Summary — Error Handling (FMRT Specification V2.2)
FMRT must reject invalid inputs and invalid structural transitions deterministically, safely, and without modifying the organism.  
Error states must be explicit, reproducible, and must never allow forbidden or undefined behavior.

---

# 7. State Validity — FMRT Specification V2.2

FMRT must validate both the **input state X(t)** and the **computed next state X(t+1)**.  
If either fails validation, the event must be rejected.

## 7.1 Input State Validity Requirements
X(t) is valid only if:

1. All fields Δ, Φ, M, κ are finite real numbers.  
2. κ(t) > 0 (living domain).  
3. det g(X(t)) > 0 (metric positivity).  
4. τ(X(t)) > 0 (temporal density positivity).  
5. No NaN or Inf exists in any field.  
6. Structural configuration corresponds to a valid FMT 3.1 organism.

If X(t) is invalid, the step must be rejected.

## 7.2 Next-State Validity Requirements
After evolution, X(t+1) must satisfy:

1. All fields are finite and non-NaN.  
2. κ(t+1) ≥ 0.  
3. If κ(t+1) > 0, then det g(X(t+1)) > 0 and τ(X(t+1)) > 0.  
4. Regime ordering is preserved:
   \[
   \mathcal{R}(t+1) \ge \mathcal{R}(t)
   \]
5. μ(X(t+1)) is within [0, 1].  
6. No forbidden domain violation.  
7. Morphology class correctly matches μ(X(t+1)).  
8. Collapse geometry is consistent if κ(t+1) = 0.

If any condition fails, the event must be rejected.

## 7.3 Forbidden Domain Definition
The forbidden domain consists of all states where:

- κ ≤ 0 (except legal collapse → κ = 0)  
- det g ≤ 0 for κ>0  
- τ ≤ 0 for κ>0  
- any NaN or Inf  
- invalid morphology μ outside [0,1]  
- inconsistent collapse geometry  
- invalid regime transition

FMRT must not output any state inside the forbidden domain.

## 7.4 Collapse-State Validity
If κ(t+1) = 0, FMRT must ensure:

- R = ∞  
- det g = 0  
- τ = 0  
- μ = 1  
- regime = COL  

If collapse geometry is inconsistent → reject event.

## 7.5 RESET State Validity
STRUCT_RESET must produce a fully valid minimal state satisfying:

- κ > 0  
- det g > 0  
- τ > 0  
- μ within [0,1]  
- regime = ACC  
- no NaN or Inf  
- all structural fields finite

If reset initialization results in an invalid state, FMRT must treat it as a fatal configuration error.

## 7.6 Continuous Validity Guarantee
FMRT must ensure that:

- X(t) valid → X(t+1) valid  
- no event ever transitions the organism into an invalid structural state  
- rejected events must not alter X(t)

There must be no intermediate invalid states during evolution.

---

# Summary — State Validity (FMRT Specification V2.2)
FMRT must guarantee that both input and output structural states always remain valid within the FMT 3.1 living domain, unless a controlled collapse occurs.  
Any next state that would violate domain constraints or invariants must cause deterministic rejection of the event.

---

# 8. Performance Requirements — FMRT Specification V2.2

FMRT must meet strict performance guarantees to ensure safe, real-time structural evolution in high-frequency and embedded environments.

## 8.1 Time Complexity
Each event must be processed in:
- O(1) worst-case time  
- no variable-sized loops  
- no data structures that grow with runtime  

FMRT must never allocate memory proportional to the number of processed events.

## 8.2 Memory Constraints
FMRT must use:
- constant memory footprint  
- no dynamic heap allocations during event processing  
- no persistent buffers  
- no caching mechanisms that depend on history length  

All necessary data must be contained in X(t) and E(t).

## 8.3 Throughput Requirements
FMRT must support sustained processing of:
- millions of events per second on modern CPUs  
- tens of thousands per second on embedded runtimes  
without degradation of determinism or safety.

## 8.4 Numerical Stability Under Load
Across large batches of events:
- no drift into NaN or Inf is allowed  
- rounding error must not accumulate into invariant violations  
- internal operations must remain within safe numeric ranges  

If numeric instability is detected → reject event with NUMERIC_ERROR.

## 8.5 Latency Requirements
FMRT must not introduce:
- blocking operations  
- network or filesystem access  
- variable-latency computations  
- synchronization waits  

Event processing must be strictly CPU-local and deterministic in latency—bounded within microseconds per event.

## 8.6 Reset Latency
STRUCT_RESET must be processed with the same O(1) performance as any other event:
- no full-engine reinitialization  
- no expensive cleanup operations  
- no scaling with historical length

Reset must immediately produce a valid baseline structure.

## 8.7 Stability Under High-Frequency Noise
FMRT must remain stable when receiving:
- rapid alternating events  
- extended periods of STRUCT_HEARTBEAT  
- large sequences of STRUCT_UPDATE with small dt  
- GAPS of varying lengths  

The organism must not drift into invalid or forbidden state solely due to event pressure.

## 8.8 Resource Safety
FMRT must:
- never leak memory  
- never perform unbounded recursion  
- never use unbounded internal queues  
- never spawn threads internally  

FMRT must be safe to embed in real-time systems.

## 8.9 Consistency Under Parallel Invocation
If FMRT is called from multiple threads:
- each call must be independent  
- shared state is forbidden  
- deterministic behavior must remain guaranteed  

The core must be re-entrant and side-effect-free.

---

# Summary — Performance Requirements (FMRT Specification V2.2)
FMRT must process each event in constant time, with constant memory, strict determinism, and full numerical stability.  
It must tolerate high-frequency event streams and remain safe, invariant-preserving, and platform-independent under all runtime conditions.

---

# 9. Forbidden Behaviors — FMRT Specification V2.2

FMRT must never perform or allow any operation that violates theoretical, numerical, or structural safety constraints.  
Any attempt to execute a forbidden behavior must result in deterministic event rejection.

## 9.1 Modification of State Without Event
FMRT must not:
- evolve X(t) automatically  
- update any field without an explicit event  
- generate internal events  
- apply background drift or timers  

Only explicit events may change the organism.

## 9.2 Use of Raw Market Data
FMRT must never process:
- prices  
- volumes  
- order flow  
- candles  
- timestamps  
- spreads or liquidity metrics  

Only normalized structural events are allowed.

## 9.3 External Time Dependency
FMRT must not:
- read system clock  
- infer time from the environment  
- depend on real-time wall clock  

dt must come exclusively from event parameters.

## 9.4 Stochastic or Approximate Behavior
Forbidden:
- randomness or pseudo-random generators  
- probabilistic heuristics  
- nondeterministic math operations  
- adaptive learning or self-adjustment  

FMRT must be strictly deterministic.

## 9.5 Hidden Internal State
FMRT must not:
- store memory outside X(t)  
- cache implicit history  
- maintain global mutable variables  
- alter behavior based on event count or runtime duration  

No hidden state is permitted.

## 9.6 Silent Correction of Invalid Inputs
FMRT must not:
- sanitize or repair invalid values  
- guess missing fields  
- clamp fields silently  
- continue execution after invariant violation  

Invalid input must be rejected explicitly.

## 9.7 Exiting the Structural Domain
FMRT must never allow:
- κ ≤ 0 for a living state  
- det g ≤ 0 for κ>0  
- τ ≤ 0 for κ>0  
- μ outside [0,1]  
- undefined regime transitions  

Any attempt to produce such output must cause rejection.

## 9.8 Ignoring Collapse Geometry
Forbidden:
- producing κ = 0 without enforcing collapse geometry  
- producing μ ≠ 1 at collapse  
- producing det g > 0 at collapse  
- evolving a collapsed organism (except RESET)

Collapse is terminal.

## 9.9 Arithmetic and Numeric Violations
Forbidden:
- division by zero  
- overflow/underflow compromising invariants  
- NaN or Inf in any computation  
- undefined float operations  

Numeric failure requires event rejection.

## 9.10 Non-O(1) Processing
FMRT must not:
- allocate memory proportional to history length  
- perform unbounded loops  
- scale event-processing time with past inputs  

FMRT must always operate in constant time.

## 9.11 Unbounded Recursion or Concurrency
FMRT must not:
- spawn threads  
- use recursion  
- depend on OS scheduling  
- perform blocking or asynchronous operations  

FMRT must remain a pure, single-step synchronous engine.

---

# Summary — Forbidden Behaviors (FMRT Specification V2.2)
FMRT must never modify state without events, use raw market data, rely on external time, introduce nondeterminism, maintain hidden state, silently repair invalid inputs, exit the structural domain, or ignore collapse geometry.  
Any such attempt must result in deterministic rejection of the event.

---

