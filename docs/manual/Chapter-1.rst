\page chapter1 Build, Layout, and Feature Flags

\brief Stage 1 baseline: CMake authority, options, and project structure

## Why this chapter

Sattyre starts with a build system that fails clearly and enables optional subsystems deliberately.
This chapter focuses on CMake options and directory ownership so contributors do not blur boundaries.
The goal is a stable developer baseline before touching parser, solver, plugin, or packaging internals.

```text
cmake -S . -B build/default -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/default -j
ctest --test-dir build/default --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 1 baseline: CMake authority, options, and project structure.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Project map in practice

Treat `include/sattyre` as the contract and `src/*` as implementation details.
Examples and tests are used as smoke and regression checks for staged progress.
The docs folder defines documentation sources and Doxygen integration.

```text
cat docs/FrontPage.rst
find docs/manual -maxdepth 1 -name "Chapter-*.rst" | sort
```

### Quick recap

- Keep this section aligned with Stage 1 baseline: CMake authority, options, and project structure.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Feature toggles

Enable only the subsystem being implemented to keep feedback loops short.
Lua and registry can stay optional while SAT/SMT core and package flow are hardened.
Documentation generation stays conditional but on by default in project options.

```text
cmake -S . -B build/lean \
  -DSATTYRE_ENABLE_TESTS=ON \
  -DSATTYRE_ENABLE_DOCS=ON \
  -DSATTYRE_ENABLE_LUA=OFF \
  -DSATTYRE_ENABLE_REGISTRY=ON
```

### Quick recap

- Keep this section aligned with Stage 1 baseline: CMake authority, options, and project structure.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## CMake intent

The top-level `CMakeLists.txt` defines core targets: `sattyre`, `sattyre-cabi`, and CLI binaries.
Optional docs logic should include `docs/CMakeFiles.txt` only when Doxygen exists.
The package metadata file `sattyre.pc` is configured from `cmake/sattyre.pc.in`.

```text
cat build/default/sattyre.pc
pkg-config --cflags --libs build/default/sattyre.pc
```

### Quick recap

- Keep this section aligned with Stage 1 baseline: CMake authority, options, and project structure.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Operational invariants

The build must configure deterministically on a clean checkout.
No public header should require private source-only includes.
A missing optional dependency should disable only that subsystem, not the entire project.

```text
cmake --install build/default --prefix /tmp/sattyre-install
find /tmp/sattyre-install -maxdepth 3 -type f | sort
```

### Quick recap

- Keep this section aligned with Stage 1 baseline: CMake authority, options, and project structure.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Troubleshooting

Confirm the selected compiler supports C++20 for public C++ APIs and internal implementation.
Confirm include paths for third-party dependencies are only added when corresponding features are enabled.
Confirm optional targets are not compiled when feature flags are disabled.

```text
cmake -S . -B build/check -DSATTYRE_ENABLE_DOCS=ON
cmake --build build/check --target sattyre-docs
```

### Quick recap

- Keep this section aligned with Stage 1 baseline: CMake authority, options, and project structure.
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

### Supplemental example

```text
// Minimal reference snippet for this chapter
// Replace with project-specific values in your local environment.
std::cout << "sattyre manual example" << std::endl;
```
