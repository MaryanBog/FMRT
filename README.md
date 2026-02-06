# FMRT Core V2.2
Deterministic Structural Evolution Engine for Flexion Market Theory (FMT 3.1)

FMRT Core V2.2 is the certified structural evolution engine implementing the complete mathematical framework of Flexion Market Theory (FMT 3.1).

It provides deterministic, invariant-preserving, numerically stable computation of the structural state:

X(t) = (Δ(t), Φ(t), M(t), κ(t))

FMRT Core is a pure mathematical engine: no I/O, no randomness, no global state.

---

## Guarantees

FMRT Core V2.2 guarantees:

- strict enforcement of all structural invariants (κ, det g, τ, μ)
- deterministic output for identical inputs
- identical results across compilers and platforms
- IEEE-754 strict double-precision arithmetic
- zero undefined behavior and zero hidden state
- correct collapse geometry according to FMT 3.1
- O(1) execution and constant memory usage
- full functional purity

---

## Development Lifecycle

FMRT Core is developed under the FMRT Development & Certification Lifecycle V2.2:

Specification → Design → Implementation → Code Review →  
Static Analysis → Integration Validation → Auto-Test Suite →  
Theoretical Validation → Compliance Matrix → Certification

---

## Features

- Deterministic structural evolution engine
- Exact implementation of all FMT 3.1 equations
- Full invariant enforcement and forbidden-domain detection
- Explicit collapse-state modeling (κ = 0 → COL)
- Pure functional C++ API
- Identical behavior on GCC, Clang, MSVC
- Identical results on x86-64 and ARM64

---

## Build Instructions (REPRODUCIBLE)

This is the single supported and reproducible build method.

### Requirements
- CMake
- MinGW-w64 compatible g++
- Git Bash or equivalent shell

### Build

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ ..
mingw32-make -j
```

This produces:
- static library: libfmrt_core.a
- test executable: fmrt_tests.exe

---

## Running the Auto-Test Suite

FMRT includes the complete Auto-Test Suite V2.2 validating:

- invariant preservation (κ, det g, τ, μ)
- algorithmic correctness (Δ, Φ, M, κ, R, τ, μ)
- collapse geometry correctness
- forbidden-domain rejection rules
- numerical safety (NaN, Inf, denormals, overflow, underflow)
- determinism across runs
- absence of hidden state

Run:

```bash
./fmrt_tests
```

Expected output:

```
ALL TESTS PASSED
```

---

## Determinism Rules

FMRT Core must:

- produce identical output for identical inputs
- never use nondeterministic operations
- remain pure and side-effect-free
- produce identical results across GCC, Clang, MSVC
- produce identical results on x86-64 and ARM64

These guarantees are enforced by the implementation and validated by the Auto-Test Suite.

---

## Directory Structure

```
FMRT/
 ├─ include/        Public API headers
 ├─ src/            FMRT Core implementation
 ├─ tests/          Auto-Test Suite V2.2
 ├─ docs/           Specifications, lifecycle, theory, compliance
 ├─ CMakeLists.txt  Build configuration
 └─ LICENSE         Apache 2.0
```

---

## Certification Status

FMRT Core V2.2 is intended for full certification under the official lifecycle:

CERTIFIED-V2.2-<BUILD_ID>

Certification requires:

- full Auto-Test Suite pass
- full Static Analysis pass
- integration validation
- theoretical validation
- completed Compliance Matrix
- FQEE package assembly

Once certified, the build becomes immutable.

---

## License

FMRT Core V2.2 is licensed under the Apache License 2.0.
See the LICENSE file for details.

---

## Project Ownership

FMRT Core V2.2 is part of the Flexion Universe project.
All rights reserved to the project owner.
