\page chapter3 SMT Front-End and SMT-LIB Loading

\brief Stage 2 SMT ingestion and minimal model/result pipeline

## Scope

This chapter focuses on `sattyre::SMTProblem`, `parse_smtlib_file`, and basic `SMTSolver` contracts.
Early SMT support is text-oriented and intentionally conservative.
The target is robust ingestion and explicit errors, not full theory reasoning.

```text
#include <sattyre/SattyreSMT.hpp>

sattyre::SMTProblem smt = sattyre::parse_smtlib_file("examples/example.smt2");
if (smt.smtlib.empty()) {
  throw std::runtime_error("SMT input unexpectedly empty");
}
```

### Quick recap

- Keep this section aligned with Stage 2 SMT ingestion and minimal model/result pipeline.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Input validation

Missing files, empty content, or malformed command framing should report actionable errors.
The loader should preserve original text for later processing.
Do not collapse SMT and SAT semantics in this stage.

```text
(set-logic QF_UF)
(declare-fun a () Bool)
(assert a)
(check-sat)
(get-model)
```

### Quick recap

- Keep this section aligned with Stage 2 SMT ingestion and minimal model/result pipeline.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## SMT result semantics

`SMTSolveResult` supports `Sat`, `Unsat`, and `Unknown`.
`SMTModel` currently stores bindings as textual items.
The model representation can evolve without invalidating this basic contract.

```text
class MySMTSolver : public sattyre::SMTSolver {
public:
  void load(const sattyre::SMTProblem& problem) override { text_ = problem.smtlib; }
  sattyre::SMTSolveResult solve() override { return sattyre::SMTSolveResult::Unknown; }
  sattyre::SMTModel model() const override { return {}; }
private:
  std::string text_;
};
```

### Quick recap

- Keep this section aligned with Stage 2 SMT ingestion and minimal model/result pipeline.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## CLI interplay

`sattyre-cli` should detect `.smt2` inputs and route to the SMT parse path.
If solver support is not implemented, surface `Unknown` or an explicit message.
Never silently treat SMT as SAT DIMACS input.

```text
sattyre-cli examples/example.smt2
# Expect parse success and a clear SMT status path
```

### Quick recap

- Keep this section aligned with Stage 2 SMT ingestion and minimal model/result pipeline.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Test strategy

Create good and malformed SMT-LIB examples under tests fixtures.
Validate that load failures are deterministic.
Keep early tests parser-centric and lightweight.

```text
cmake -S . -B build/smt-tests -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/smt-tests -j
ctest --test-dir build/smt-tests -R sattyre-cli-smoke-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 2 SMT ingestion and minimal model/result pipeline.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

SMT stage success means dependable ingestion and clear status reporting.
Deep theory support comes later; robust loading is the immediate win.
Keep SAT and SMT pathways isolated and explicit.

```text
// Preserve the text for future theory-aware parsing layers.
```

### Quick recap

- Keep this section aligned with Stage 2 SMT ingestion and minimal model/result pipeline.
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
