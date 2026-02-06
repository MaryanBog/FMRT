# FMRT Core V2.2 — Data Structures (Full API Surface)

This document defines the complete and frozen external API of FMRT Core V2.2.
It specifies all data structures exchanged with the engine and the single entry
point used to advance the structural state according to Flexion Market Theory (FMT 3.1).

The API is deterministic, side-effect-free, and has no hidden state.

---

## 1. Public API

```cpp
StateEnvelope FMRT_Step(
    const StructuralState& X,
    const StructEvent&     E
);
```

FMRT_Step performs a single deterministic structural transition:

X(t), E(t) → X(t+1)

No other functions mutate or evolve the structural state.

---

## 2. Input: StructuralState (X)

```cpp
struct StructuralState
{
    std::array<double, DELTA_DIM> Delta {}; // Δ(t)
    double Phi   = 0.0;                     // Φ(t)
    double M     = 0.0;                     // M(t)
    double Kappa = 1.0;                     // κ(t)

    Regime RegimePrev = Regime::ACC;        // previous regime

    void reset()
    {
        for (auto &v : Delta) v = 0.0;

        Phi        = RESET_PHI;
        M          = 0.0;
        Kappa      = RESET_KAPPA;
        RegimePrev = Regime::ACC;
    }

    bool isFinite() const noexcept
    {
        if (!is_finite(Phi) || !is_finite(M) || !is_finite(Kappa))
            return false;

        for (auto v : Delta)
            if (!is_finite(v))
                return false;

        return true;
    }

    bool isLiving() const noexcept    { return Kappa > 0.0; }
    bool isCollapsed() const noexcept { return Kappa == 0.0; }
};
```

Rules:
- All numeric fields must be finite IEEE-754 values.
- κ < 0 is forbidden.
- κ == 0 represents a terminal collapse state.

---

## 3. Input: StructEvent (E)

```cpp
struct StructEvent
{
    EventType type = EventType::Heartbeat;    // Update / Gap / Heartbeat / Reset
    double dt = 0.0;                          // required for non-RESET events
    std::array<double, DELTA_DIM> stimulus{}; // deformation for UPDATE only
    const char* reason = nullptr;             // optional RESET diagnostic

    bool isFinite() const noexcept
    {
        if (!is_finite(dt))
            return false;

        for (auto v : stimulus)
            if (!is_finite(v))
                return false;

        return true;
    }

    bool hasValidDt() const noexcept
    {
        if (type == EventType::Reset)
            return true; // dt is ignored for RESET but must be finite

        return dt > 0.0;
    }

    bool stimulusNeeded() const noexcept
    {
        return type == EventType::Update;
    }
};
```

Notes:
- For RESET events, dt is ignored by the evolution logic but must be finite.
- stimulus is only used for UPDATE events.

---

## 4. Output: DerivedMetrics (part of StateEnvelope)

```cpp
struct DerivedMetrics
{
    // Geometry / structure
    double curvature_R = 0.0;   // scalar curvature
    double det_g       = 0.0;   // metric determinant
    double tau         = 0.0;   // temporal density τ
    double mu          = 0.0;   // morphology index μ ∈ [0,1]

    // Classification
    MorphologyClass morph_class = MorphologyClass::Elastic;
    Regime          regime      = Regime::ACC;

    // Collapse indicators
    bool   is_collapse        = false; // κ == 0
    double collapse_distance  = 0.0;   // equal to κ
    double collapse_speed     = 0.0;   // ||Δ_next − Δ|| / dt
    double collapse_intensity = 0.0;   // derived from curvature

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
```

DerivedMetrics contains only values computed by the engine.
No input parameter directly sets these fields.

---

## 5. Output: InvariantStatus (part of StateEnvelope)

```cpp
struct InvariantStatus
{
    uint32_t flags = 0;   // bitmask of InvariantBits
    bool     all_ok = false;

    inline void set(uint32_t bit) noexcept { flags |= bit; }

    inline bool check(uint32_t bit) const noexcept
    {
        return (flags & bit) != 0;
    }

    inline void clear() noexcept
    {
        flags = 0;
        all_ok = false;
    }
};
```

InvariantStatus reports which structural invariants were evaluated
and whether the resulting state satisfies all of them.

---

## 6. Output: StateEnvelope (step result)

```cpp
struct StateEnvelope
{
    StructuralState state;      // X(t+1) or X(t) if rejected
    DerivedMetrics  metrics;    // geometry, regime, collapse data
    InvariantStatus invariants; // invariant evaluation result

    StepStatus     status = StepStatus::OK;          // OK / COLLAPSE / REJECT / ERROR
    ErrorCategory  error_category = ErrorCategory::None;
    const char*    error_reason   = nullptr;         // static constant string

    EventType event_type = EventType::Heartbeat;     // processed event type

    bool isFinite() const noexcept
    {
        return state.isFinite() && metrics.isFinite();
    }
};
```

Notes:
- error_reason always points to a static constant string.
- The caller must not free or modify error_reason.
- isFinite() validates numerical fields only.

---

## 7. Error Strings (Diagnostics)

```cpp
static constexpr const char* ERR_NONE                = "no_error";
static constexpr const char* ERR_INVALID_EVENT       = "invalid_event";
static constexpr const char* ERR_INVALID_STATE       = "invalid_state";
static constexpr const char* ERR_INVARIANT_VIOLATION = "invariant_violation";
static constexpr const char* ERR_FORBIDDEN_DOMAIN    = "forbidden_domain_transition";
static constexpr const char* ERR_NUMERIC_ERROR       = "numeric_error";
static constexpr const char* ERR_POST_COLLAPSE       = "post_collapse_event_rejected";
static constexpr const char* ERR_UNSUPPORTED         = "unsupported_operation";

inline constexpr const char* errorCategoryToString(ErrorCategory cat) noexcept
{
    switch (cat)
    {
        case ErrorCategory::None:                 return ERR_NONE;
        case ErrorCategory::InvalidEvent:         return ERR_INVALID_EVENT;
        case ErrorCategory::InvalidState:         return ERR_INVALID_STATE;
        case ErrorCategory::InvariantViolation:   return ERR_INVARIANT_VIOLATION;
        case ErrorCategory::ForbiddenDomain:      return ERR_FORBIDDEN_DOMAIN;
        case ErrorCategory::NumericError:         return ERR_NUMERIC_ERROR;
        case ErrorCategory::PostCollapse:         return ERR_POST_COLLAPSE;
        case ErrorCategory::UnsupportedOperation: return ERR_UNSUPPORTED;
    }
    return ERR_UNSUPPORTED;
}
```

---

## 8. Summary

FMRT Core exposes a single deterministic transition function.

Input:
- StructuralState X
- StructEvent E

Output:
- StateEnvelope containing:
  - next state
  - derived metrics
  - invariant evaluation
  - step status and error diagnostics

No other data enters or leaves the engine.
This API boundary is final and stable for FMRT Core V2.2.
