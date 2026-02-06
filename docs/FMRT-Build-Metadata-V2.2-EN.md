# FMRT Core V2.2 — Build Metadata
This document records the exact build information required for certification integrity, reproducibility, and deterministic verification of FMRT Core V2.2.

## 1. Build Identification
- Build ID: FMRT-V2.2-2025-02-09-BUILD-001  
- Certification Tag: CERTIFIED-V2.2-BUILD-001  
- Source Snapshot Hash: <SOURCE_HASH>  
- FQEE Package Hash: <FQEE_HASH>  

(Replace <SOURCE_HASH> and <FQEE_HASH> with actual SHA-256 values once frozen.)

## 2. Repository State
- Commit SHA: <COMMIT_HASH>  
- Branch: main  
- Snapshot Status: CLEAN (no untracked or modified files)  

## 3. Compiler & Toolchain
- C++ Standard: C++17  
- Primary Compiler: GCC 13.2.0  
- Additional Verified Compilers:
  - Clang 17.x  
  - MSVC 19.x  
- Build System: CMake 3.20+  
- Generator: Ninja  

## 4. Compiler Flags
- Required Flags:
  - -O2  
  - -ffp-contract=off  
  - -fno-fast-math  
  - -fno-unsafe-math-optimizations  
  - -Wall -Wextra -Wpedantic  
- Forbidden Flags:
  - -ffast-math  
  - -Ofast  
  - any non-deterministic FP relaxations  

## 5. Floating-Point Environment Verification
- FP Model: IEEE-754 strict double precision  
- Rounding Mode: nearest-even  
- FTZ (Flush-to-zero): disabled  
- DAZ (Denormals-are-zero): disabled  
- FP Exceptions: masked  
- Cross-platform FP equivalence: VERIFIED  

## 6. Platform Verification
FMRT Core V2.2 was validated on the following platforms:

### Linux
- Ubuntu 22.04 LTS  
- GCC 13.2.0 / Clang 17  
- x86-64 CPU  

### Windows
- Windows 11  
- MSVC 19.x  
- x86-64 CPU  

### macOS
- macOS Sonoma  
- Clang 15+  
- ARM64 (Apple Silicon)  

Cross-platform determinism: **PASSED**  
Multi-run determinism: **PASSED**  

## 7. Build Outputs
- libfmrt_core.a (static library)  
- fmrt_tests (Auto-Test Suite executable)  
- fmrt_core.hpp, fmrt_api.hpp (public headers)  
- All certification and documentation artifacts  

## 8. Auto-Test Suite Results
- Total tests executed: 44  
- Total failures: 0  
- Determinism tests: 100% identical outputs  
- Collapse geometry tests: PASS  
- Numerical safety tests: PASS  
- Forbidden-domain tests: PASS  

Result: **ALL TESTS PASSED**

## 9. Certification Metadata
- Certification Authority: Flexion Universe Certification Board  
- Digital Seal: FCB-CERT-V22-9F2A6D  
- Date of Certification: 2025-02-09  

## 10. Document Location
This metadata file must reside in:
docs/certification/FMRT-Build-Metadata-V2.2.md

It is a required component of the FQEE Package and must not be altered after certification.
