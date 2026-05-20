\page chapter12 CLI Workflows, Testing Matrix, and Release Readiness

\brief Stage 8 user tooling and project polish

## Scope

This chapter covers `sattyre-cli`, `sattyre-packman`, `sattyre-registry`, and release checks.
The CLI should reflect real library behavior, not stubs.
Testing and docs must align with implementation state.

```text
sattyre-cli examples/example.cnf
sattyre-cli --solver native examples/example.cnf
sattyre-cli examples/example.smt2
```

### Quick recap

- Keep this section aligned with Stage 8 user tooling and project polish.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## sattyre-packman subcommands

validate checks manifest structure.
pack creates an archive.
unpack extracts a bundle.
install stages and places package files.
remove deletes a package by name.

```text
sattyre-packman validate ./manifest.json
sattyre-packman pack ./pkg-root ./pkg.satpkg
sattyre-packman unpack ./pkg.satpkg ./out
sattyre-packman install ./pkg.satpkg ./local-root
sattyre-packman remove demo-solver ./local-root
```

### Quick recap

- Keep this section aligned with Stage 8 user tooling and project polish.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## sattyre-registry examples

Serve packages locally.
Publish archives to the registry.
Fetch packages by name.
List available packages.

```text
sattyre-registry serve --host 127.0.0.1 --port 8080 --root ./registry-root
sattyre-registry publish ./pkg.satpkg --url http://127.0.0.1:8080
sattyre-registry fetch demo-solver --url http://127.0.0.1:8080
sattyre-registry list --url http://127.0.0.1:8080
```

### Quick recap

- Keep this section aligned with Stage 8 user tooling and project polish.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Testing matrix

Parser tests: DIMACS and manifest parsing behavior.
ABI tests: solver/plugin/library compatibility and metadata.
Package flow tests: bundle, install, remove.
Registry smoke tests: local and optional HTTP checks.
CLI smoke tests for each tool.

```text
ctest --test-dir build/release --output-on-failure
ctest --test-dir build/release -R sattyre-packman-cli-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 8 user tooling and project polish.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Release checklist

Public headers install cleanly under `include/sattyre`.
`sattyre.pc` is generated and installable.
Docs build produces HTML, LaTeX, DocBook, and XML outputs.
Version and compatibility policy are documented.

```text
cmake -S . -B build/release -DSATTYRE_ENABLE_TESTS=ON -DSATTYRE_ENABLE_DOCS=ON
cmake --build build/release -j
cmake --build build/release --target sattyre-docs
```

### Quick recap

- Keep this section aligned with Stage 8 user tooling and project polish.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

Stage 8 completes when tooling, tests, and docs are coherent.
Use this chapter as the pre-release gate.
Keep examples executable and synchronized with real behavior.

```text
// Release readiness means the docs say the same thing the code does.
```

### Quick recap

- Keep this section aligned with Stage 8 user tooling and project polish.
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
