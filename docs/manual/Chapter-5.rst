\page chapter5 C++ Solver Adapter and Built-in Solvers

\brief Stage 3 C++ consumption of C ABI and baseline solving

## Scope

This chapter explains `NativeSolverAdapter` and baseline built-in solver behavior.
Focus first on correctness, then sophistication.
Brute-force and DPLL are useful reference points before CDCL optimization.

```text
class NativeSolverAdapter : public sattyre::SATSolver {
public:
  void load(const sattyre::SATProblem& problem) override;
  sattyre::SolveResult solve() override;
  sattyre::SATModel model() const override;
};
```

### Quick recap

- Keep this section aligned with Stage 3 C++ consumption of C ABI and baseline solving.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Compatibility-first flow

On construction, adapter queries `sattyre_solver_info` and verifies ABI compatibility.
If incompatible, fail early with a clear exception.
Solver creation and destruction should follow RAII semantics.

```text
sattyre::NativeSolverAdapter solver;
sattyre::SATProblem p = sattyre::parse_dimacs_file("examples/example.cnf");
solver.load(p);
sattyre::SolveResult r = solver.solve();
```

### Quick recap

- Keep this section aligned with Stage 3 C++ consumption of C ABI and baseline solving.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Built-in solver sequencing

Brute-force solver: correctness oracle for tiny formulas.
DPLL solver: practical baseline and readable branching logic.
CDCL solver: staged optimization once baseline semantics are trusted.

```text
sattyre::SATProblem p;
p.variable_count = 2;
p.clauses = {{1}, {-1, 2}};
```

### Quick recap

- Keep this section aligned with Stage 3 C++ consumption of C ABI and baseline solving.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Model extraction

Model assignments should correspond to `variable_count`.
Unknown result may return empty model by design.
Do not expose inconsistent assignment vector lengths.

```text
// The model must not exceed the declared number of variables.
// Preserve a direct mapping from variable index to assignment.
```

### Quick recap

- Keep this section aligned with Stage 3 C++ consumption of C ABI and baseline solving.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Testing adapters

Use small satisfiable and unsatisfiable instances.
Verify result mapping and exception behavior.
Confirm adapter refuses incompatible ABI metadata.

```text
cmake -S . -B build/solver -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/solver -j
ctest --test-dir build/solver -R sattyre-cli-smoke-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 3 C++ consumption of C ABI and baseline solving.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

C++ adapter reliability depends on strict C ABI checks.
Built-in solvers should be layered by correctness maturity.
This chapter’s examples form the minimum solve smoke workflow.

```text
// Keep the adapter thin and let the solver implementation carry the logic.
```

### Quick recap

- Keep this section aligned with Stage 3 C++ consumption of C ABI and baseline solving.
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

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```
