\page chapter6 Plugin ABI, Library ABI, and Dynamic Loading

\brief Stage 4 modular extension contracts

## Scope

This chapter covers plugin/library metadata, compatibility helpers, and `DynamicLibrary` usage.
Solvers, plugins, and libraries are distinct extension classes and must remain separate.
Loader behavior should fail early on missing symbols or ABI mismatches.

```text
#define SATTYRE_PLUGIN_ABI_VERSION 1u
int sattyre_plugin_init(void);
void sattyre_plugin_shutdown(void);
SattyrePluginInfo sattyre_plugin_info(void);
int sattyre_plugin_abi_compatible(unsigned int abi_version);
```

### Quick recap

- Keep this section aligned with Stage 4 modular extension contracts.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Library ABI

`SATTYRE_LIBRARY_ABI_VERSION` provides the matching version contract for reusable libraries.
Compatibility checks should be performed before initialization or symbol use.
Library metadata should include name and version like plugin metadata.

```text
SattyreLibraryInfo info = sattyre_library_info();
if (!sattyre_library_abi_compatible(info.abi_version)) {
  throw std::runtime_error("library ABI mismatch");
}
```

### Quick recap

- Keep this section aligned with Stage 4 modular extension contracts.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Dynamic loader usage

Use `sattyre::DynamicLibrary` to load modules by path.
Fetch symbols lazily and check each required export before calling it.
Keep platform-specific logic behind the loader abstraction.

```text
sattyre::DynamicLibrary lib;
if (!lib.open(path)) {
  throw std::runtime_error("failed to open module");
}
auto info = reinterpret_cast<SattyrePluginInfo(*)()>(lib.symbol("sattyre_plugin_info"));
```

### Quick recap

- Keep this section aligned with Stage 4 modular extension contracts.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Search path conventions

Use `sattyre::paths::plugin_dir` and `sattyre::paths::lib_dir` for stable layouts.
Document where modules are loaded from during CLI/runtime flows.
Avoid hard-coded absolute paths in source code.

```text
// Search paths should come from paths helpers, not literals.
// This keeps loaders portable across install layouts.
```

### Quick recap

- Keep this section aligned with Stage 4 modular extension contracts.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Compatibility gate

Compatibility helpers reduce repeated checks and make loaders easier to audit.
Fail immediately on version mismatch instead of continuing with undefined behavior.
The ABI boundary is the right place for this validation.

```text
SattyrePluginInfo info = sattyre_plugin_info();
if (!sattyre::plugin_abi_compatible(info)) {
  throw std::runtime_error("plugin ABI mismatch");
}
```

### Quick recap

- Keep this section aligned with Stage 4 modular extension contracts.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

Stage 4 is successful when loader behavior is predictable and compatibility-driven.
A minimal but strict ABI yields safer extension loading.
Do not collapse plugin and library APIs into a single vague module type.

```text
// Keep the loader small, explicit, and easy to test.
```

### Quick recap

- Keep this section aligned with Stage 4 modular extension contracts.
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
