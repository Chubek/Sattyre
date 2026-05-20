\page chapter10 Registry Server and Client Flow

\brief Stage 6 local-first registry and HTTP endpoints

## Scope

This chapter covers `RegistryServer` and `RegistryClient` APIs and staged HTTP features.
Start with local, scriptable behavior before distributed deployment polish.
`RegistryStatus` values should describe not-found and I/O failures clearly.

```text
sattyre::RegistryServer server;
sattyre::RegistryClient client;
std::vector<std::string> packages;
auto st = server.list_packages(registry_root, packages);
```

### Quick recap

- Keep this section aligned with Stage 6 local-first registry and HTTP endpoints.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Recommended endpoints

GET /health
GET /packages
GET /packages/{name}/manifest
GET /packages/{name}/bundle
POST /packages

```text
sattyre-registry serve --host 127.0.0.1 --port 8080 --root ./registry-root
```

### Quick recap

- Keep this section aligned with Stage 6 local-first registry and HTTP endpoints.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Client operations

`deploy_http` uploads a bundle to registry.
`download_http` pulls package bundle by name.
`list_http` fetches package list for inspection and automation.

```text
sattyre-registry publish ./demo.satpkg --url http://127.0.0.1:8080
sattyre-registry fetch demo-solver --url http://127.0.0.1:8080
```

### Quick recap

- Keep this section aligned with Stage 6 local-first registry and HTTP endpoints.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Validation concerns

Treat malformed manifests or missing bundle files as explicit status failures.
Log uploads/downloads with package name and version.
Keep endpoint payloads stable and versioned.

```text
// Use a simple, scriptable registry format first.
// Add auth and deployment polish after the basic flow works.
```

### Quick recap

- Keep this section aligned with Stage 6 local-first registry and HTTP endpoints.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Health and listing

Provide a health endpoint to verify the registry is alive.
Ensure package listings are deterministic so scripting is easy.
Use small JSON payloads that are easy to inspect and test.

```text
curl http://127.0.0.1:8080/health
curl http://127.0.0.1:8080/packages
```

### Quick recap

- Keep this section aligned with Stage 6 local-first registry and HTTP endpoints.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

A usable local registry closes the package distribution loop.
Status-driven errors keep CLI and CI behavior predictable.
Security hardening can build on this stable baseline.

```text
// Registry logic should stay transparent and script-friendly.
```

### Quick recap

- Keep this section aligned with Stage 6 local-first registry and HTTP endpoints.
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
