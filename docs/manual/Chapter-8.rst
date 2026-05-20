\page chapter8 Bundle Create/Extract and Path Safety

\brief Stage 5 archive implementation details

## Scope

This chapter explains `create_bundle` and `extract_bundle` behavior.
Archives should round-trip package contents safely.
Path traversal defenses are mandatory.

```text
bool create_bundle(const std::string& source_dir, const std::string& out_file);
bool extract_bundle(const std::string& bundle_file, const std::string& out_dir);
```

### Quick recap

- Keep this section aligned with Stage 5 archive implementation details.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Round-trip expectation

Create archive from package root containing manifest and payload files.
Extract into destination root with normalized internal paths.
Reject entries escaping `out_dir` via `../` or absolute paths.

```text
sattyre-packman pack ./package-root ./demo.satpkg
sattyre-packman unpack ./demo.satpkg ./extracted
```

### Quick recap

- Keep this section aligned with Stage 5 archive implementation details.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Path safety checks

Reject archive entries with `..` segments after lexical normalization.
Reject absolute path entries.
Create parent directories before writing extracted files.

```text
if (!sattyre::extract_bundle(bundle, out)) {
  std::cerr << "Extract failed; inspect archive for invalid entries\n";
}
```

### Quick recap

- Keep this section aligned with Stage 5 archive implementation details.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Archive reproducibility

Prefer deterministic file ordering for reproducible bundles.
Preserve permissions where meaningful, but never at the cost of safety.
Keep archive naming and payload layout stable across builds.

```text
// Reproducible archives make package flow debugging much easier.
// Stable ordering also helps tests remain deterministic.
```

### Quick recap

- Keep this section aligned with Stage 5 archive implementation details.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Troubleshooting

If libarchive is unavailable, ensure fallback behavior remains explicit.
Verify executable bits and text files are preserved where expected.
Confirm extraction never writes outside the target tree.

```text
cmake -S . -B build/bundle -DSATTYRE_ENABLE_LIBARCHIVE=ON
cmake --build build/bundle -j
```

### Quick recap

- Keep this section aligned with Stage 5 archive implementation details.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

Bundle logic must be correct and safe before registry publishing is trusted.
Archive path handling is a security boundary.
Round-trip tests should run as part of package flow checks.

```text
// Treat archive paths as untrusted input, even for local packages.
```

### Quick recap

- Keep this section aligned with Stage 5 archive implementation details.
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
