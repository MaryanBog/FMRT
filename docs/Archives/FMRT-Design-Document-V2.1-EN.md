# FMRT Design Document — Version 2.1

---

## 0. Overview

The FMRT Design Document describes the architecture and internal structure of the FMRT Core, providing guidance for implementation, integration, and maintenance.  
It translates the theoretical concepts from FMRT Specification (V2.1) into a practical, modular, and deterministic software design suitable for real-world deployment.

### 0.1 Purpose

The purpose of this document is to:

- Define the high-level architecture of FMRT Core.
- Describe modules, their responsibilities, and interactions.
- Detail data flows from input events to output envelopes.
- Establish interface contracts for SDKs and external components.
- Provide guidelines for error handling, determinism, and performance.

### 0.2 Scope

This document covers:

- Internal modular structure of FMRT Core.
- Event handling pipelines, including GAP and HEARTBEAT processing.
- Interfaces between core modules and external SDKs.
- Fault tolerance, invariants enforcement, and diagnostic propagation.
- Design decisions supporting deterministic evolution and safe multi-threaded operation.

It does **not** include:

- Implementation-specific code or algorithms (covered in Implementation Document).  
- Theoretical background (covered in Specification Document).  
- Platform-specific optimizations (may be added later in separate implementation notes).

### 0.3 Design Goals

- **Modularity:** Each functional unit is self-contained with clearly defined responsibilities.  
- **Determinism:** Identical inputs produce identical outputs across platforms.  
- **Invariant Preservation:** Structural invariants are maintained throughout all computations.  
- **Fault Tolerance:** The system isolates errors and prevents corruption of X(t).  
- **Extensibility:** New modules or features can be integrated without breaking existing functionality.  
- **Clarity:** Architecture must be transparent for developers, testers, and integrators.

> Note: This document forms the foundation for all subsequent design sections, including module descriptions, data flows, and API contracts.

---

# 1. Architecture

The FMRT Core is designed as a modular, deterministic, and fault-tolerant system.  
Its architecture separates functional responsibilities, ensures invariant preservation, and supports seamless integration with SDKs and external components.

## 1.1 High-Level Architecture

FMRT Core consists of the following primary modules:

1. **Tick Adapter**
   - Converts raw market ticks into normalized structural events.
   - Performs input validation, minimal filtering, and timestamp correction.
   - Generates four event types: STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET.

2. **Event Sequencer**
   - Orders events to maintain monotonic structural time.
   - Detects duplicates, missing events, and out-of-order arrivals.
   - Ensures deterministic delivery of events to FMRT Core Engine.

3. **FMRT Core Engine**
   - Computes the evolution of X(t) → X(t+1) using theoretical rules from FMRT Specification V2.1.
   - Updates structural state, morphology, regime, curvature, temporal metrics, viability, and collapse indicators.
   - Enforces all invariants and minimal engineering constraints.

4. **Invariant Checker**
   - Validates that structural invariants (Section 4 of Specification) hold after each step.
   - Flags any violation for diagnostics and error handling.
   - Prevents invalid states from propagating.

5. **Diagnostics Module**
   - Collects and organizes step_status, error_reason, error_code, error_category, and event_type.
   - Provides structured output for SDKs and automated testing frameworks.

## 1.2 Module Interactions

- Tick Adapter → Event Sequencer → FMRT Core Engine → Invariant Checker → Diagnostics Module → Output Envelope
- The flow ensures that all events are processed deterministically and validated before producing output.
- GAP and HEARTBEAT events are handled internally in the FMRT Core Engine without violating invariants.
- RESET events trigger a controlled reinitialization of the structural state, ensuring consistency across modules.

## 1.3 Design Principles

- **Modularity:** Each module has clearly defined responsibilities, facilitating testing and maintenance.
- **Isolation:** Modules communicate only through well-defined interfaces; no hidden shared state.
- **Determinism:** Sequencer ensures event ordering; Core Engine computes identical X(t+1) for identical inputs.
- **Fault Tolerance:** Invariant Checker and Diagnostics prevent propagation of errors.
- **Extensibility:** New modules can be added (e.g., predictive analytics, multi-symbol support) without altering core invariants.

> Note: This high-level architecture provides the foundation for subsequent sections describing data flows, interfaces, and module-specific responsibilities.

---

# 2. Modules Description

This section provides detailed descriptions of each primary module in the FMRT Core architecture, including responsibilities, inputs, outputs, and interaction with other modules.

## 2.1 Tick Adapter

**Purpose:**  
- Converts raw market data into normalized structural events compatible with FMRT Core.

**Responsibilities:**  
- Validate incoming tick data for correctness (no NaN, Inf, negative timestamps, or invalid BID/ASK).  
- Apply minimal filtering according to the Minimal Filtering Principle (Specification V2.1, Section 5.2).  
- Generate structural events: STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET.  
- Attach timestamps and sequence IDs to maintain monotonic order.

**Inputs:**  
- Raw market ticks (price, volume, timestamp, source_id).

**Outputs:**  
- Normalized structural events (as defined in Specification V2.1, Section 2.5).

---

## 2.2 Event Sequencer

**Purpose:**  
- Ensures deterministic, ordered delivery of structural events to FMRT Core Engine.

**Responsibilities:**  
- Detect duplicate events and remove them.  
- Correct out-of-order arrivals using sequence IDs or timestamps.  
- Handle event batching when high-frequency events exceed processing capacity.  
- Ensure monotonic progression of structural time.

**Inputs:**  
- Normalized structural events from Tick Adapter.

**Outputs:**  
- Ordered structural events delivered to FMRT Core Engine.

---

## 2.3 FMRT Core Engine

**Purpose:**  
- Computes structural evolution from X(t) to X(t+1) based on FMRT theory.

**Responsibilities:**  
- Apply theoretical update rules for Δ, Φ, M, κ.  
- Update derived fields: morphology, regime, curvature, temporal density, viability, collapse indicators.  
- Enforce minimal engineering requirements (Section 5, Specification V2.1).  
- Handle GAP and HEARTBEAT events gracefully.  
- Process RESET events and reinitialize structural state.

**Inputs:**  
- Ordered structural events from Event Sequencer.  
- Current structural state X(t).

**Outputs:**  
- Updated structural state X(t+1) passed to Invariant Checker.  

---

## 2.4 Invariant Checker

**Purpose:**  
- Ensure all structural invariants are preserved for each evolution step.

**Responsibilities:**  
- Verify memory monotonicity, viability non-negativity, metric positivity, temporal density, regime ordering, and continuity.  
- Flag any violations for diagnostics and error handling.  
- Prevent invalid states from being propagated to Diagnostics or external modules.

**Inputs:**  
- Updated structural state X(t+1) from FMRT Core Engine.

**Outputs:**  
- Validated structural state.  
- Invariant violation flags.

---

## 2.5 Diagnostics Module

**Purpose:**  
- Collects and organizes diagnostic information for SDKs, testing, and monitoring.

**Responsibilities:**  
- Record step_status, error_reason, error_code, error_category, event_type.  
- Package diagnostics into State Envelope.  
- Ensure deterministic reporting without altering core computations.

**Inputs:**  
- Validated structural state and invariant flags from Invariant Checker.  
- Structural event metadata.

**Outputs:**  
- Diagnostics data integrated into State Envelope for SDK or testing frameworks.

> Note: Each module is designed to operate independently with clearly defined interfaces, supporting modular testing and future extensibility.

---

# 3. Data Flow

This section describes the flow of data and events through the FMRT Core modules, ensuring deterministic processing and invariant preservation.

## 3.1 Overview

The FMRT Core processes data in a linear pipeline:

1. **Tick Adapter** receives raw market ticks.
2. **Event Sequencer** orders and validates structural events.
3. **FMRT Core Engine** computes X(t+1) and derived structural metrics.
4. **Invariant Checker** validates all invariants.
5. **Diagnostics Module** collects step results and prepares State Envelope for SDK or testing frameworks.

This flow ensures that each event is processed in a deterministic and validated manner, with error handling applied at each stage.

---

## 3.2 Tick Processing

- Raw market ticks enter the Tick Adapter.  
- Tick Adapter performs:
  - Validation (NaN, Inf, timestamp checks, BID/ASK consistency)
  - Minimal filtering according to Specification V2.1
  - Event generation (STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET)  
- Generated structural events include sequence IDs to maintain order.

---

## 3.3 Event Sequencing

- Event Sequencer ensures all events are delivered in monotonic order.  
- Duplicate events are removed, and out-of-order events are corrected.  
- Sequencer may batch events when the incoming rate exceeds processing capacity.  
- Ordered events are passed to FMRT Core Engine.

---

## 3.4 Core Engine Processing

- FMRT Core Engine applies theoretical evolution rules from Specification V2.1.  
- Updates include:
  - X(t) → X(t+1)  
  - Derived metrics: morphology, regime, curvature, temporal density, viability, collapse indicators  
- GAP and HEARTBEAT events result in minimal evolution without violating invariants.  
- RESET events trigger controlled reinitialization of X(t).

---

## 3.5 Invariant Checking

- Updated structural state is validated by Invariant Checker.  
- All structural invariants (Section 4 of Specification) are verified.  
- Any violations are flagged for diagnostics.  
- Invalid states are rejected and previous state preserved.

---

## 3.6 Diagnostics and Output

- Diagnostics Module collects:
  - step_status, error_reason, error_code, error_category, event_type  
- State Envelope is constructed combining X(t+1), invariants, diagnostics.  
- State Envelope is sent to SDK or testing frameworks for monitoring or further processing.

---

## 3.7 Summary

The FMRT data flow ensures:

- Deterministic processing: identical inputs → identical outputs  
- Preservation of structural invariants  
- Safe handling of gaps, heartbeats, and resets  
- Accurate and structured diagnostics for monitoring and testing

---

# 4. Interfaces / API Contracts

This section defines the interfaces and API contracts between FMRT Core modules and external components such as SDKs or testing frameworks.  
All interactions are deterministic, type-safe, and consistent with Specification V2.1.

## 4.1 Tick Adapter Interface

**Function:** `processRawTick(tick) → Event`

- **Input:**  
  - tick: { symbol, bid, ask, volume, timestamp, source_id }  
- **Output:**  
  - Event: { event_type, price_delta?, dt?, reason? }  
- **Constraints:**  
  - Must reject invalid ticks (NaN, Inf, negative timestamps, BID > ASK)  
  - Must produce one of four event types: STRUCT_UPDATE, STRUCT_GAP, STRUCT_HEARTBEAT, STRUCT_RESET  
  - Must attach sequence ID for ordering  

---

## 4.2 Event Sequencer Interface

**Function:** `orderEvent(event) → OrderedEvent`

- **Input:**  
  - Event from Tick Adapter  
- **Output:**  
  - OrderedEvent with monotonic timestamp or sequence ID  
- **Constraints:**  
  - Remove duplicates  
  - Correct out-of-order events  
  - Batch events if rate exceeds processing capacity  

---

## 4.3 FMRT Core Engine Interface

**Function:** `computeNextState(X_current, OrderedEvent) → X_next`

- **Input:**  
  - X_current: { Delta, Phi, M, Kappa }  
  - OrderedEvent: normalized structural event  
- **Output:**  
  - X_next: updated structural state  
  - Derived metrics: morphology, regime, curvature, temporal density, viability, collapse indicators  
- **Constraints:**  
  - Must enforce minimal engineering rules (Specification V2.1 Section 5)  
  - Must handle GAP, HEARTBEAT, and RESET events  
  - Must not violate structural invariants  

---

## 4.4 Invariant Checker Interface

**Function:** `validateInvariants(X_next) → ValidatedState`

- **Input:**  
  - X_next: updated structural state from Core Engine  
- **Output:**  
  - ValidatedState: X_next plus invariant flags  
- **Constraints:**  
  - Reject invalid states  
  - Preserve previous state if any invariant is violated  

---

## 4.5 Diagnostics Module Interface

**Function:** `collectDiagnostics(ValidatedState, Event) → StateEnvelope`

- **Input:**  
  - ValidatedState: X_next with invariant flags  
  - Event metadata: type, timestamp, source_id  
- **Output:**  
  - StateEnvelope: structured output containing X_next, derived metrics, invariants, diagnostics  
- **Constraints:**  
  - Diagnostics must be informative only; cannot alter X_next  
  - Include error_code and error_category if any fault occurs  

---

## 4.6 SDK / External Interface

- **Function:** `getStateEnvelope() → StateEnvelope`  
- Provides read-only access to latest StateEnvelope.  
- Guarantees deterministic and consistent data for SDKs and testing frameworks.  
- No direct modification of X(t) allowed from external components.  

---

> Note: All interfaces must be deterministic, type-safe, and enforce proper validation to ensure safe and correct operation of FMRT Core in real-time environments.

---

# 5. Error Handling / Fault Tolerance

This section describes how FMRT Core handles errors, ensures fault isolation, and maintains structural integrity under abnormal conditions.

## 5.1 Error Handling Principles

- **Soft Failure:** Minor or recoverable errors (e.g., invalid event parameters, temporary spikes) are logged and rejected without modifying X(t). The previous state is preserved, and diagnostics indicate the reason.
- **Hard Failure (DEAD State):** If κ(t+1) reaches zero, the organism enters a terminal DEAD state. All fields remain constant, and no subsequent events modify the state until a STRUCT_RESET occurs.
- **Numerical Hazards:** NaN, Inf, division by zero, invalid metrics, or temporal density violations trigger step rejection. The previous valid state is preserved, and diagnostics are generated.
- **Invariant Violations:** Any violation of structural invariants triggers immediate step rejection and diagnostic reporting.

---

## 5.2 Fault Isolation

- Each module isolates failures from propagating to other modules.  
- Tick Adapter and Event Sequencer prevent invalid or misordered events from reaching Core Engine.  
- Invariant Checker prevents invalid structural states from being propagated to Diagnostics or SDK.  
- Diagnostics Module collects and reports errors without modifying X(t).

---

## 5.3 Handling GAP and HEARTBEAT Events

- GAP and HEARTBEAT events are processed to evolve memory (M) and viability (κ) minimally, without violating invariants.  
- The system guarantees that no structural field is corrupted due to missing or delayed events.

---

## 5.4 Reset Events

- STRUCT_RESET events trigger a controlled reinitialization of X(t).  
- All invariants are revalidated after reset.  
- Diagnostics record the reset reason and timestamp.

---

## 5.5 Deterministic Recovery

- Identical sequences of events must produce identical error handling behavior across platforms.  
- Soft or hard failures must never compromise deterministic evolution of the organism.  
- Modules must ensure atomicity of state updates to prevent race conditions or partial application of events.

---

> Note: These fault tolerance principles ensure that FMRT Core remains reliable, deterministic, and safe under all operational conditions.

---

# 6. Determinism & Concurrency

This section defines how FMRT Core guarantees deterministic outputs and safe concurrent operation across all supported platforms.

## 6.1 Determinism Principles

- For identical inputs (X(t) and event sequences), FMRT Core must produce **bitwise identical outputs**.  
- Determinism must hold across:
  - different hardware architectures,
  - operating systems,
  - compiler versions and optimization levels.

## 6.2 Concurrency Model

- FMRT Core must support concurrent read operations from SDK or external modules without modifying X(t).  
- Internal state updates (X(t) → X(t+1)) are atomic and isolated to prevent race conditions.  
- Event processing is serialized internally via Event Sequencer to maintain monotonic structural time.

## 6.3 Module Isolation

- Each module operates independently with no shared mutable state beyond defined interfaces.  
- Tick Adapter, Event Sequencer, Core Engine, Invariant Checker, and Diagnostics Module communicate only through well-defined, thread-safe APIs.  
- Parallelism is allowed for read-only operations and diagnostics, but updates to X(t) are always serialized.

## 6.4 Handling High-Frequency Events

- Event sequencing ensures proper ordering and prevents nondeterministic results caused by rapid or overlapping events.  
- Batching or throttling is applied if event rate exceeds processing capacity (Specification V2.1, Section 8.5).

## 6.5 Verification

- Determinism and concurrency correctness must be verified via automated tests:
  - identical event sequences produce identical StateEnvelopes,
  - multi-threaded read access does not alter structural state,
  - GAP, HEARTBEAT, and RESET events maintain deterministic behavior.

> Note: These principles ensure FMRT Core is both deterministic and safe for concurrent operation in real-time environments.

---

# 7. Performance Considerations

This section describes design choices and recommendations to ensure FMRT Core operates efficiently in real-time environments, while maintaining determinism and invariant preservation.

## 7.1 Event Processing Efficiency

- Each structural event must be processed in **O(1)** time complexity.  
- Hot path computations within FMRT Core Engine must avoid dynamic memory allocation.  
- Pre-allocated buffers and stack-only structures should be used wherever possible to ensure predictable execution times.

## 7.2 Memory Management

- Memory usage must be bounded and deterministic.  
- No unbounded growth of internal buffers or state structures.  
- Data structures must be designed to handle long-running simulations without degradation.

## 7.3 Numerical Stability

- All computations use **IEEE-754 double precision** arithmetic.  
- Compiler flags that introduce non-deterministic floating-point behavior (e.g., fast-math) are forbidden.  
- Round-off errors must not violate invariants or affect the deterministic output.

## 7.4 Event Rate Handling

- FMRT Core must handle high-frequency event streams without backlog or dropped events.  
- Tick Adapter is responsible for batching or throttling events when the input rate exceeds processing capacity.  
- Recommended target parameters for initial implementation:
  - Maximum Δt between processed events: 1 ms  
  - Minimum heartbeat interval: 0.5 ms  
  - Maximum supported event rate: 1000 events/sec per symbol

## 7.5 Diagnostics and Monitoring Overhead

- Diagnostics collection must not block or slow down event processing.  
- Read-only diagnostic operations may be performed in parallel without affecting core computations.  
- Logging and monitoring must be lightweight to avoid impacting deterministic performance.

## 7.6 Scalability

- Architecture supports scaling to multiple symbols or instruments by instantiating independent FMRT Core instances.  
- Multi-symbol or multi-core deployments must preserve deterministic processing for each organism instance independently.

> Note: These considerations ensure that FMRT Core maintains real-time responsiveness, accuracy, and reliability in high-frequency operational environments.

---

# 8. Extensibility

This section defines design principles and guidelines for extending FMRT Core with new modules, features, or capabilities without compromising determinism, invariants, or core functionality.

## 8.1 Modular Design

- FMRT Core modules are self-contained with clearly defined interfaces.  
- New modules must communicate only through existing APIs or properly defined new interfaces.  
- Modules must not directly modify internal state of other modules.

## 8.2 Adding New Features

- Any new feature must respect structural invariants defined in Specification V2.1, Section 4.  
- New computations or derived metrics must be appended to the output envelope without altering existing fields.  
- GAP, HEARTBEAT, and RESET event handling must remain compatible with new modules.

## 8.3 SDK Integration

- New modules must provide documented API contracts to SDKs.  
- Backward compatibility with existing SDKs and external consumers must be maintained.  
- Modules must support deterministic output even with optional new features enabled.

## 8.4 Testing and Verification

- Each new module must include unit tests and integration tests verifying:
  - Deterministic behavior
  - Invariant preservation
  - Compatibility with existing event flow
- Regression tests must confirm that core functionality and outputs remain unchanged for existing features.

## 8.5 Guidelines

- Avoid introducing hidden state or side-effects.  
- Maintain modular isolation to prevent cascading failures.  
- Document all new interfaces and dependencies clearly.  
- Ensure resource usage (CPU, memory) remains bounded and deterministic.

> Note: Following these extensibility principles ensures FMRT Core can evolve and scale over time without compromising reliability, determinism, or theoretical correctness.
