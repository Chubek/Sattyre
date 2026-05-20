\page chapter7 Package Manifests and Normalization

\brief Stage 5 manifest structure and validation

## Scope

This chapter focuses on `PackageManifest`, parsing, and canonical JSON output.
The parser should normalize supported input formats into one internal shape.
Required fields are `name`, `version`, and `type`.

```text
sattyre::PackageManifest parse_manifest_text(const std::string&, const std::string& source = {});
sattyre::PackageManifest load_manifest(const std::string& path);
std::string manifest_to_json(const sattyre::PackageManifest& manifest);
```

### Quick recap

- Keep this section aligned with Stage 5 manifest structure and validation.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Canonical manifest fields

`name`: package identifier
`version`: package version string
`type`: solver | plugin | library | lua-addon
`dependencies`: optional list
`description`: optional text

```text
{
  "name": "demo-solver",
  "version": "1.0.0",
  "type": "solver",
  "dependencies": ["core-lib"],
  "description": "tiny example package"
}
```

### Quick recap

- Keep this section aligned with Stage 5 manifest structure and validation.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Validation strategy

Reject missing required fields with source-aware errors.
Reject unknown type values unless explicitly supported.
Normalize dependencies to deterministic order where needed for reproducible outputs.

```text
// Canonicalization makes comparison and registry indexing predictable.
// Do not preserve parser-specific quirks in the normalized struct.
```

### Quick recap

- Keep this section aligned with Stage 5 manifest structure and validation.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Normalization flow

Load a manifest from JSON, YAML, S-expression, or XML.
Convert it into `PackageManifest` with consistent field names and semantics.
Re-emit canonical JSON for inspection or registry use.

```text
auto mf = sattyre::load_manifest("share/packages/demo/manifest.json");
std::string canonical = sattyre::manifest_to_json(mf);
std::cout << canonical << "\n";
```

### Quick recap

- Keep this section aligned with Stage 5 manifest structure and validation.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Manifest tests

Positive tests for valid manifests.
Negative tests for missing name/version/type.
Malformed document tests for each supported parser path.

```text
cmake -S . -B build/manifest -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/manifest -j
ctest --test-dir build/manifest -R sattyre-manifest-test --output-on-failure
```

### Quick recap

- Keep this section aligned with Stage 5 manifest structure and validation.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

Manifest normalization is the backbone of package, install, and registry flows.
Keep the parser strict and outputs canonical.
Prefer explicit parse failures over fallback guesses.

```text
// Normalized manifests should be easy to serialize, compare, and store.
```

### Quick recap

- Keep this section aligned with Stage 5 manifest structure and validation.
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
