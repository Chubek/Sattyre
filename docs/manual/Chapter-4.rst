\page chapter4 Solver C ABI Contract

\brief Stage 3 ABI versioning, metadata, and compatibility checks

## Scope

This chapter documents `include/sattyre/Sattyre-Solver.h` and C ABI expectations.
The ABI should be small, stable, and explicit about compatibility.
C++ wrappers should consume this ABI rather than bypass it.

```text
#define SATTYRE_SOLVER_ABI_VERSION 1u

typedef struct {
  unsigned int abi_version;
  const char* name;
  const char* version;
} SattyreSolverInfo;
```

### Quick recap

- Keep this section aligned with Stage 3 ABI versioning, metadata, and compatibility checks.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Mandatory symbols

`sattyre_solver_create`
`sattyre_solver_destroy`
`sattyre_solver_solve`
`sattyre_solver_info`
`sattyre_solver_abi_compatible`

```text
SattyreSolver* solver = sattyre_solver_create();
SattyreSolverResult result = sattyre_solver_solve(solver);
sattyre_solver_destroy(solver);
```

### Quick recap

- Keep this section aligned with Stage 3 ABI versioning, metadata, and compatibility checks.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Compatibility helper

The helper accepts an ABI version and returns nonzero for compatibility.
Call it before solver use in adapters and module loaders.
Avoid duplicating version checks in multiple places.

```text
SattyreSolverInfo info = sattyre_solver_info();
if (!sattyre_solver_abi_compatible(info.abi_version)) {
  fprintf(stderr, "Incompatible solver ABI: %u\n", info.abi_version);
  return 1;
}
```

### Quick recap

- Keep this section aligned with Stage 3 ABI versioning, metadata, and compatibility checks.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Result codes

`SattyreSolverResult` maps to SAT, UNSAT, UNKNOWN.
Adapters should map these exactly to `sattyre::SolveResult`.
Unknown must remain distinguishable from failure paths.

```text
// Keep Unknown for indeterminate or unsupported cases.
// Do not repurpose it as a generic error bucket.
```

### Quick recap

- Keep this section aligned with Stage 3 ABI versioning, metadata, and compatibility checks.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## ABI tests

A dedicated solver ABI test should verify version and metadata fields.
Compatibility helper should be checked with current and mismatched versions.
C and C++ surfaces should agree on results.

```text
cmake -S . -B build/abi -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/abi -j
ctest --test-dir build/abi -R sattyre-solver-abi-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 3 ABI versioning, metadata, and compatibility checks.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

A tiny C ABI is the backbone of extensible solver integration.
Compatibility checks prevent confusing runtime misuse.
Stage 3 is complete when adapters rely on this contract consistently.

```text
// Version checks belong at the ABI boundary, not scattered in solver code.
```

### Quick recap

- Keep this section aligned with Stage 3 ABI versioning, metadata, and compatibility checks.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```
