\page chapter2 SAT Core and DIMACS Parsing

\brief Stage 2 SAT front-end correctness

## Scope

This chapter covers SAT structures, DIMACS ingestion, and deterministic parser failures.
The parser must reject malformed inputs early and preserve clear semantics for callers.
Examples here map directly to `sattyre::SATProblem` and `parse_dimacs_file` behavior.

```text
#include <sattyre/SattyreSAT.hpp>

sattyre::SATProblem p = sattyre::parse_dimacs_file("examples/example.cnf");
std::size_t vars = p.variable_count;
std::size_t clauses = p.clauses.size();
```

### Quick recap

- Keep this section aligned with Stage 2 SAT front-end correctness.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## DIMACS fundamentals

Comment lines begin with `c` and can appear before the problem line.
The problem line defines variable and clause declarations and should be validated.
Each clause must terminate with literal `0`.

```text
c tiny satisfiable sample
p cnf 3 2
1 -2 0
2 3 0
```

### Quick recap

- Keep this section aligned with Stage 2 SAT front-end correctness.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Validation expectations

Out-of-range literal ids must fail when absolute literal exceeds declared variable count.
Clause lines without terminating zero must fail.
Mismatch between declared and parsed clause counts should fail clearly.

```text
try {
  auto p = sattyre::parse_dimacs_file("broken.cnf");
  (void)p;
} catch (const std::exception& ex) {
  std::cerr << "DIMACS parse error: " << ex.what() << "\n";
}
```

### Quick recap

- Keep this section aligned with Stage 2 SAT front-end correctness.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Edge semantics

An empty formula with zero clauses is SAT by convention but still parsed as a valid problem.
An empty clause denotes UNSAT structure and should be preserved as an empty clause entry.
Literal sign encodes polarity; absolute value encodes variable index.

```text
sattyre::SATProblem p;
p.variable_count = 2;
p.clauses = { {1}, {-1, 2} };
```

### Quick recap

- Keep this section aligned with Stage 2 SAT front-end correctness.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Test design

Write tests for normal, malformed, and boundary cases.
Use small CNF files so failures are obvious and reproducible.
Check exact exceptions or statuses to prevent silent behavior drift.

```text
cmake -S . -B build/sat-tests -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/sat-tests -j
ctest --test-dir build/sat-tests -R sattyre-sat-parser-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 2 SAT front-end correctness.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

SAT correctness begins with a strict parser, not with advanced solver heuristics.
If parser behavior is unstable, downstream solver and CLI behavior cannot be trusted.
Treat parse errors as first-class API outcomes.

```text
// Good parser output should be deterministic for a given input.
// Keep regression coverage close to edge-case inputs.
```

### Quick recap

- Keep this section aligned with Stage 2 SAT front-end correctness.
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
