# FMRT-Integration-Document-V2.2

---

# 0. Purpose & Scope — FMRT Integration V2.2 (Compact Edition)

This document defines the minimal and strict rules required to correctly integrate FMRT Core V2.2 into:

- higher-level systems,
- SDK components,
- runtime environments,
- data pipelines,
- algorithmic modules.

The purpose is to ensure that:
- FMRT remains deterministic,
- FMRT API remains pure,
- integration does not corrupt internal state,
- system-level interactions do not violate FMRT invariants.

Scope excludes:
- algorithmic correctness (covered by Theoretical Validation),
- static correctness (covered by Static Analysis),
- runtime testing (covered by Test Suite).

---

# 1. Integration Principles

1. FMRT is a **pure transformation engine**:  
   \[
   (X(t), E(t)) \rightarrow StateEnvelope(t+1)
   \]

2. All integration must preserve:
   - determinism,
   - purity,
   - isolation of state,
   - invariant enforcement.

3. FMRT Core must never depend on:
   - OS environment,
   - hardware specifics,
   - external data sources,
   - mutable global state,
   - runtime configuration files.

4. Integration must not introduce:
   - shared mutable memory,
   - side effects,
   - nondeterministic scheduling,
   - floating-point inconsistencies.

These principles are mandatory.

---

# 2. FMRT Core Boundary Definition

FMRT Core exposes exactly two inputs:

1. **State X(t)** — complete structural state.  
2. **Event E(t)** — normalized structural event.

FMRT Core outputs exactly one object:

3. **StateEnvelope(t+1)** — containing:
   - next state X(t+1),
   - status code,
   - metadata (optional but deterministic).

No additional I/O is permitted.

Integration must respect:
- no mutation of X(t) after passing to FMRT,
- no mutation of returned state,
- no internal caching between calls.

---

# 3. API Purity Requirements

Integration must ensure:

- FMRT.step() has no side-effects,
- no external state influences FMRT,
- no hidden parameters are injected via integration layer.

Forbidden during integration:
- logging FMRT internals,
- altering FP environment,
- using timestamps or randomness,
- calling system APIs inside FMRT context.

---

# 4. State Ownership Rules

1. FMRT owns **no state outside X(t)**.  
2. Integration layer must:
   - allocate, store, and pass X(t),
   - never mutate FMRT internals,
   - never retain internal FMRT pointers.

3. Returned StateEnvelope(t+1):
   - must be treated as immutable,
   - must not be modified or partially reused.

State ownership violations → undefined runtime behavior → non-compliant integration.

---

# 5. Event Normalization Requirements

Integration must supply **normalized, validated events**:

- dt must be finite, > 0,
- fields must be NaN/Inf-free,
- types must match FMRT Specification,
- irrelevant external data must be stripped before entering FMRT.

Event normalization must occur **outside** FMRT Core.

FMRT must always operate on structurally valid events.

---

# 6. Error Handling & Fail-Safe Behavior

Integration must ensure:

- FMRT errors do not propagate destructively,
- FMRT step_status is always checked,
- forbidden-domain outputs trigger deterministic recovery.

FMRT may produce:

- OK,
- REJECT (event rejected),
- COLLAPSE (terminal state),
- ERROR (integration error).

Integration must handle each explicitly.

---

# 7. Concurrency Integration Rules

1. Parallel FMRT usage must use **independent FMRT instances**.  
2. No shared mutable state may flow through integration.  
3. Integration layer must not:
   - cache FMRT buffers,
   - share state envelopes,
   - use multi-threading inside FMRT calls.

Parallel safety is the responsibility of integration.

---

# 8. Allowed & Forbidden Integration Patterns

### Allowed:
- stateless wrappers,
- pure function adapters,
- deterministic data pipelines,
- event batching (if order is preserved),
- thread-parallel independent FMRT instances.

### Forbidden:
- global mutable caches,
- runtime configuration affecting FMRT,
- external I/O inside FMRT flow,
- mutation of FMRT internal state,
- altering FP mode before/after FMRT,
- mixing FMRT versions in one system.

---

# 9. Integration Validation Checklist

Integration is compliant if:

- FMRT.step is called in pure form,
- X(t) lifecycle is explicit and owned by integration,
- E(t) is validated before FMRT call,
- no side-effects occur inside FMRT boundary,
- concurrency model is isolation-safe,
- invariant checks remain intact,
- FP environment is unchanged before/after FMRT use,
- all error codes are processed deterministically.

---

# 10. Certification Requirements

To certify an integration:

1. Provide integration-layer source snapshot.  
2. Provide dependency map and call graph.  
3. Prove absence of:
   - global mutable state,
   - FP environment changes,
   - hidden caches,
   - side-effects,
   - nondeterministic behavior.

4. Show:
   - FMRT version used,
   - consistency with Specification/Implementation,
   - correct X(t) lifecycle management,
   - correct event normalization pipeline.

Certification is binary:
**PASS** (fully compliant) / **FAIL** (any violation).