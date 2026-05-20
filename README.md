# Sattyre

**Sattyre** is a modular SAT and SMT framework for C and C++.

It provides:

- a minimal core API for SAT and SMT,
- interchangeable solver backends,
- native plugins and libraries,
- Lua-based extensions,
- command-line tools for solving, packaging, and registry hosting.

Sattyre follows a **Bring Your Own Solver (BYOS)** model: the core framework defines interfaces and loading mechanisms, while concrete solvers are provided as dynamically loadable modules.

---

## Features

- Header-oriented SAT and SMT interfaces for C++
- Stable C ABI for solver, plugin, and library modules
- Optional C++ convenience interfaces
- Support for CNF, DIMACS CNF, and SMT-LIB
- Dynamically loadable solver backends
- Plugin system for tooling and extensions
- Package manager and package bundle format
- Registry server for hosting packages
- Lua bindings for writing addons

---

## Project Layout

```text
$SATTYRE_HOME/
├── bin/
│   ├── sattyre-cli
│   ├── sattyre-packman
│   └── sattyre-registry
├── include/
│   ├── SattyreSAT.hpp
│   ├── SattyreSMT.hpp
│   ├── Sattyre-Solver.h
│   ├── Sattyre-Solver.hpp
│   ├── Sattyre-Plugin.h
│   ├── Sattyre-Plugin.hpp
│   ├── Sattyre-Library.h
│   └── Sattyre-Library.hpp
├── lib/
│   ├── solvers/
│   ├── plugins/
│   ├── lua/
│   └── ...
├── share/
│   └── packages/
└── doc/
```

---

## Core Interfaces

### SAT

The SAT interface is provided by:

- `SattyreSAT.hpp`

It supports:

- constructing SAT problems,
- loading CNF formulas,
- parsing DIMACS CNF,
- submitting problems to a solver,
- retrieving satisfiability results and models.

### SMT

The SMT interface is provided by:

- `SattyreSMT.hpp`

It supports:

- constructing SMT problems,
- parsing SMT-LIB input,
- submitting problems to a solver,
- retrieving satisfiability results and models where supported.

---

## Solver Interfaces

Sattyre is solver-agnostic. Solvers can be implemented independently and loaded at runtime.

### C Solver Interface

The primary native solver ABI is defined by:

- `Sattyre-Solver.h`
- `Sattyre-Solver.c`

Use this interface when you want:

- maximum compatibility,
- a stable ABI,
- instrumentation and low-level control.

Current C solver ABI version:

- `SATTYRE_SOLVER_ABI_VERSION = 1`

Compiled solver modules are installed in:

```text
$SATTYRE_HOME/lib/solvers
```

### C++ Solver Interface

The C++ convenience interface is defined by:

- `Sattyre-Solver.hpp`

This interface is easier to use from C++, but is less ABI-stable than the C interface.

### Included Solvers

Sattyre may ship with reference solver implementations such as:

- BruteForce
- DPLL
- CDCL

These serve as examples, fallback implementations, and test backends.

---

## Plugins

Plugins extend Sattyre with features other than solving.

Plugin interfaces are defined by:

- `Sattyre-Plugin.h`
- `Sattyre-Plugin.c`
- `Sattyre-Plugin.hpp`

Examples of plugin responsibilities:

- CLI extensions
- import/export helpers
- analysis tools
- build helpers
- integration adapters

Compiled plugin modules are installed in:

```text
$SATTYRE_HOME/lib/plugins
```

---

## Libraries

Libraries are reusable runtime components shared by solvers, plugins, and applications.

Library interfaces are defined by:

- `Sattyre-Library.h`
- `Sattyre-Library.c`
- `Sattyre-Library.hpp`

Installed location:

```text
$SATTYRE_HOME/lib
```

---

## Command-Line Tools

### `sattyre-cli`

Primary frontend for solving SAT and SMT problems.

Example uses:

```sh
sattyre-cli solve example.cnf
sattyre-cli solve --solver DPLL example.dimacs
sattyre-cli solve example.smt2
```

### `sattyre-packman`

Package management tool for Sattyre packages.

Current behavior:

```sh
sattyre-packman validate manifest.json
sattyre-packman pack ./package ./package.tar.gz
sattyre-packman unpack ./package.tar.gz ./unpacked
sattyre-packman install ./package.tar.gz --root ~/.sattyre
sattyre-packman remove my-package --root ~/.sattyre
```

### `sattyre-registry`

Registry server for hosting package bundles and indexes.

Current behavior:

```sh
sattyre-registry serve --root ./registry-root
```

Programmatic registry flow is available via `sattyre::RegistryClient` and
`sattyre::RegistryServer`:

- `RegistryClient::deploy(bundle, registry_root)` publishes a bundle file into a local registry root.
- `RegistryServer::list_packages(registry_root, out)` lists bundles in a local registry root.
- `RegistryClient::download(package_file, registry_root)` fetches a bundle into the current working directory.
- All registry operations return `RegistryStatus` for explicit failure semantics.

---

## Package System

Sattyre packages are used to distribute:

- solvers
- plugins
- libraries

A package contains:

- payload files,
- a manifest,
- optional documentation,
- optional build/install metadata.

### Package Types

Each package declares one of:

- `solver`
- `plugin`
- `library`

### Supported Manifest Formats

A package manifest may be written in:

- JSON
- YAML
- S-expression
- XML

### Install Destinations

By package type:

- solver → `$SATTYRE_HOME/lib/solvers`
- plugin → `$SATTYRE_HOME/lib/plugins`
- library → `$SATTYRE_HOME/lib`

Installer operations return `InstallerStatus` so callers can distinguish:

- destination already exists (`AlreadyExists`)
- parse/extract errors (`ManifestParseFailed`, `ExtractFailed`)
- filesystem failures (`MoveFailed`, `CreateDestinationFailed`, etc.)

Package archives and metadata are stored under:

- packages → `$SATTYRE_HOME/share/packages`
- documentation → `$SATTYRE_HOME/doc`

### Dependency Resolution

Dependencies are resolved by the Sattyre package management system during installation and deployment.

---

## Registry System

Sattyre registries host package bundles and package metadata.

A registry may be:

- local,
- private,
- remote.

Registries support:

- package hosting,
- package download,
- package deployment by authorized publishers.

Configured registries are listed in:

```text
/etc/sattyre/registries
```

Authentication may be SSH-based, depending on registry configuration.

---

## Lua Addons

Sattyre supports writing addons in Lua.

Available Lua modules include:

- `lsat`
- `lsmt`
- `lsattyre`

These modules can be used to create:

- solvers,
- plugins,
- libraries.

Example Lua namespaces:

- `lsattyre.solver`
- `lsattyre.plugin`
- `lsattyre.library`

Lua modules are installed in:

```text
$SATTYRE_HOME/lib/lua
```

User-level addon data may be stored in:

```text
~/.sattyre
```

---

## Quick Start

### Solve a SAT problem

```sh
sattyre-cli solve problem.cnf
```

### Solve with a specific solver

```sh
sattyre-cli solve --solver CDCL problem.cnf
```

### Solve an SMT-LIB problem

```sh
sattyre-cli solve problem.smt2
```

### Create a new solver package

```sh
sattyre-packman validate manifest.json
```

### Bundle a package

```sh
sattyre-packman pack ./package ./package.tar.gz
```

### Deploy to a registry

```sh
sattyre-packman install ./package.tar.gz --root ~/.sattyre
```

---

## Design Goals

- Minimal core API
- Solver independence
- Stable native ABI
- Dynamic extensibility
- Scriptable addons
- Tool-driven packaging and deployment

---

## Status

Sattyre is designed as a modular framework specification and implementation platform for SAT and SMT tooling. Exact API details, manifest schemas, and registry protocol details may evolve over time.

---

## Planned Clarifications

Future revisions should define:

- exact SAT and SMT APIs,
- plugin lifecycle,
- package manifest schema,
- registry protocol,
- Lua embedding contract,
- versioning and compatibility guarantees.

---

## License

Add your license here.

---

## Contributing

Add contribution guidelines here.

---

If you want, I can also make this into a **more polished GitHub-style README** with:
- badges,
- installation section,
- build section,
- example C++ code,
- example package manifest,
- and nicer command examples.
