\page chapter9 Installer API, Status Codes, and Local Package Layout

\brief Stage 5 install/remove determinism

## Scope

This chapter covers `InstallerStatus` and install/remove package operations.
Boolean success is insufficient for CLI and API callers.
Status enums let callers distinguish already-exists, parse, extract, and I/O failures.

```text
enum class InstallerStatus {
  Ok = 0, BundleNotFound, RootPathError, ExtractFailed, ManifestMissing,
  ManifestParseFailed, AlreadyExists, RemoveExistingFailed,
  CreateDestinationFailed, MoveFailed, RemoveFailed, NotFound
};
```

### Quick recap

- Keep this section aligned with Stage 5 install/remove determinism.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Installation flow

Validate bundle path and install root.
Extract into staging path.
Load and validate manifest.
Move staged package into destination by package name.
Handle overwrite policy explicitly.

```text
auto st = sattyre::install_package(bundle, root, false);
if (st == sattyre::InstallerStatus::AlreadyExists) {
  std::cout << "Package already installed; use overwrite if desired\n";
}
```

### Quick recap

- Keep this section aligned with Stage 5 install/remove determinism.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Removal flow

Map missing package to `InstallerStatus::NotFound`.
Return `RemoveFailed` for filesystem errors.
Do not silently ignore remove failures.

```text
auto st = sattyre::remove_package("demo-solver", root);
if (st != sattyre::InstallerStatus::Ok) {
  std::cerr << "remove failed with status code" << static_cast<int>(st) << "\n";
}
```

### Quick recap

- Keep this section aligned with Stage 5 install/remove determinism.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Layout expectations

Packages should live in a destination tree that separates solver, plugin, library, and Lua addon installs.
Keep manifest and payload paths predictable.
Provide a removal path that can undo a successful install.

```text
// Package layout should be easy to inspect with `find` or a file browser.
// Stable destinations reduce surprises in packman and registry flows.
```

### Quick recap

- Keep this section aligned with Stage 5 install/remove determinism.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## CLI mapping

`sattyre-packman` install/remove should print user-facing text by status.
Exit codes should be nonzero for non-`Ok` statuses.
Keep status-to-message mapping centralized.

```text
cmake -S . -B build/install -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/install -j
ctest --test-dir build/install -R sattyre-package-flow-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 5 install/remove determinism.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

Installer status enums make failures diagnosable.
The local package flow becomes deterministic and script-friendly.
This is critical before building remote registry pipelines.

```text
// Distinguish already-exists from actual extraction failures.
```

### Quick recap

- Keep this section aligned with Stage 5 install/remove determinism.
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
