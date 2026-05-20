# AGENTS.md

## 1. Mission, Working Style, and Success Criteria

This document is for **GPT Codex** working on **Sattyre**.

Sattyre is a modular SAT/SMT framework in C and C++ with:

- core SAT and SMT interfaces
- solver ABI and adapters
- plugin ABI
- library ABI
- package bundling and installation
- package registry client/server
- Lua embedding and bindings
- CLI tooling

The current repository is a **scaffold with real structure but partial implementation**. Your job is to turn it into a real library **incrementally**, **safely**, and **without breaking architecture**.

### Primary goals

1. Keep public API clean and stable.
2. Prefer small, verifiable changes over sweeping rewrites.
3. Preserve the existing directory structure unless there is a strong reason to change it.
4. Implement the project in stages.
5. Add tests as features become real.
6. Avoid speculative abstractions unless they solve an immediate need.

### Definition of done for any change

A task is only done if:

- code builds
- headers remain coherent
- naming remains consistent
- code matches the architecture in this repository
- there is at least minimal validation, test, or example coverage
- docs are updated if behavior changes

### General operating rules

- Do not replace the project’s architecture with a different one.
- Do not silently rename public headers or tools.
- Do not invent new subsystems unless required.
- Prefer finishing one subsystem properly before touching three others halfway.
- When uncertain, implement the smallest useful version first.

---

## 2. Repository Map and Ownership of Each Area

Use this section to understand what each directory is supposed to become.

### Root files

- `CMakeLists.txt`  
  Main build definition. Prefer CMake as the source of truth.
- `Makefile`  
  Convenience wrapper or compatibility layer.
- `build.sh`  
  Human-friendly build entrypoint.
- `README.md`  
  User-facing overview.
- `AGENTS.md`  
  This file; your operating manual.

### `cmake/`

- `BuildThirdParty.cmake`  
  Dependency setup policy.
- `MakeDocs.cmake`  
  Documentation generation hooks.
- `sattyre.pc.in`  
  pkg-config metadata template.

### `include/sattyre/`

This is the **public API surface**. Treat changes here as important.

Key headers:

- `SattyreSAT.hpp`  
  SAT domain model and solver-facing abstraction.
- `SattyreSMT.hpp`  
  SMT domain model and solver-facing abstraction.
- `Sattyre-Solver.h` / `.hpp`  
  C ABI and C++ adapter surface for solver loading/integration.
- `Sattyre-Plugin.h` / `.hpp`  
  Plugin ABI surface.
- `Sattyre-Library.h` / `.hpp`  
  Shared library runtime ABI surface.
- `Manifest.hpp`, `Bundle.hpp`, `Installer.hpp`, `Registry.hpp`  
  Package and registry APIs.
- `LuaRuntime.hpp`, `LuaBindings.hpp`  
  Lua embedding and registration APIs.
- `Log.hpp`, `Paths.hpp`, `Dylib.hpp`, `Version.hpp`  
  Utility-level public facilities.

### `src/`

- `core/`  
  DIMACS parsing, SMT-LIB loading, later core shared logic.
- `solver/`  
  C ABI implementation, adapter logic, built-in/reference solvers.
- `plugin/`  
  Plugin lifecycle implementation.
- `library/`  
  Reusable runtime library ABI support.
- `package/`  
  Manifest parsing, bundle creation/extraction, install/remove.
- `registry/`  
  Registry HTTP server and client behavior.
- `lua/`  
  Lua runtime initialization and module binding.
- `util/`  
  Logging, paths, dynamic library loading.
- `cli/`  
  End-user tools.

### `examples/`

Keep these runnable and useful. They are smoke tests for users.

### `docs/`

Design intent lives here. Update when major behavior stabilizes.

### `share/packages/`

Example package manifests and later sample package layouts.

### `scripts/`

Scaffolding and helper scripts. Avoid putting core build logic here if CMake can own it.

---

## 3. Architecture Rules and Non-Negotiable Design Constraints

This project already implies a strong architecture. Keep it coherent.

### 3.1 Public interface policy

- Public headers under `include/sattyre/` are the canonical API.
- C ABI headers must remain conservative and stable.
- C++ wrappers may be nicer, but must not undermine the C ABI model.
- Do not let internal implementation details leak into public headers.

### 3.2 ABI philosophy

There are three major native extension types:

1. **Solver**
2. **Plugin**
3. **Library**

Each should have:

- clear metadata
- lifecycle functions where relevant
- version checks
- stable calling conventions
- minimal required exported symbols

If ABI versioning is missing, add it early.

### 3.3 SAT vs SMT

Keep SAT and SMT separated conceptually.

- SAT:
  - DIMACS parsing
  - CNF representation
  - literal/clause/problem abstractions
  - SAT model representation
- SMT:
  - SMT-LIB text or AST-based loading
  - theory-aware future expansion
  - model and result handling independent of SAT

Do not force them into the same abstraction too early.

### 3.4 Internal vs external dependencies

Current dependency intent:

- **AzmaTest**: unit tests, fuzz tests
- **cpp-httplib**: registry HTTP serving/client logic
- **fmtlib**: formatting
- **klyspec**: CLI construction
- **libarchive**: bundles/packages
- **libssh2**: authenticated deployment
- **log4cplus**: logging backend
- **lua**: embedded scripting runtime
- **serdetk**: JSON/YAML/S-Expr/XML manifests
- **sol2**: high-level Lua C++ bindings
- **dynalo**: cross-platform shared library loading

Since `dynalo` is now part of the plan, prefer it over maintaining too much manual platform-specific loading logic in `Dylib.cpp`, unless you intentionally keep a fallback layer.

### 3.5 Error handling

Prefer explicit failures.

- library code: return typed status or throw well-defined exceptions in C++ API
- C ABI: return error codes/status enums
- CLI: convert failures into readable messages and nonzero exit codes

Do not swallow errors.

### 3.6 Logging

Use logging intentionally.

- debug data belongs in logs, not in API return values
- avoid noisy logging in hot paths
- log lifecycle, IO, dependency, loading, install, and registry events

### 3.7 Naming discipline

Keep naming consistent:

- namespace: `sattyre`
- CLI names: `sattyre-cli`, `sattyre-packman`, `sattyre-registry`
- no alternate spellings
- preserve header names unless a migration plan exists

---

## 4. Token Economics

You use GPT Codex in **low reasoning mode**, so work must be token-efficient without becoming careless.

### Core principle

Use **short thinking loops, not giant speculative plans**.

The repository is structured enough that most progress should come from:

- inspecting one subsystem
- making one targeted change
- building/tests
- repeating

### Rules for efficient token use

#### 4.1 Read only what you need

Before editing, inspect only:

- the relevant header
- the matching source file
- the build entry that includes it
- nearby tests/examples if they exist

Do not repeatedly scan the whole repo unless the task truly spans subsystems.

#### 4.2 Keep outputs compact while coding

Prefer this style internally:

- summarize current file purpose in 1–3 lines
- list exact changes to make
- make the patch
- verify compile impact

Avoid essay-like self-explanations during routine edits.

#### 4.3 Work in bounded tasks

Good task sizes:

- implement DIMACS parser edge cases
- add ABI version field to solver metadata
- replace manual dylib code with dynalo wrapper
- wire `fmt::format` into CLI output
- parse one manifest format correctly before all four

Bad task sizes:

- “finish the whole package system”
- “implement all solvers”
- “rewrite everything for elegance”

#### 4.4 Prefer local reasoning over global redesign

If a header and one `.cpp` file can solve the issue, do that first.

Do not redesign package architecture to fix a parser bug.

#### 4.5 Minimize repeated context restatement

Do not re-describe the entire project to yourself every step. Use a short working summary like:

- subsystem
- current gap
- expected behavior
- files to edit
- validation method

#### 4.6 Ask for or perform verification quickly

After a change, immediately try to verify with:

- build
- test
- example run
- static review of obvious compile errors

Fast feedback saves more tokens than long up-front planning.

#### 4.7 Use staged implementation

If a full feature is large, split it:

1. define data structures
2. implement core function
3. add error handling
4. add tests
5. expose through CLI if needed

This avoids wasting tokens on code that gets replaced.

#### 4.8 Avoid over-commenting generated code

Comments should explain:

- ABI expectations
- invariants
- file format assumptions
- portability traps

Do not write filler comments on every line.

#### 4.9 Be honest about unknowns

If a dependency API is uncertain, do not invent it confidently. Instead:

- leave a thin integration seam
- note what must be confirmed
- keep the code compilable if possible

That is cheaper than fabricating a wrong full integration.

#### 4.10 Preferred execution loop

For almost any task:

1. inspect target header/source
2. identify exact missing behavior
3. patch minimally
4. update build/tests/examples if needed
5. summarize what changed and what remains

This is the right cost/quality balance.

---

## 5. Implementation Plan in 8 Stages

Implement the project in these **eight stages**, in order. Do not jump to late-stage polish while early-stage foundations are still hollow.

### Stage 1 — Build System, Dependency Wiring, and Developer Baseline

This stage makes the repo dependable to work in.

#### Goals

- Make CMake the authoritative build.
- Ensure every dependency has a defined integration policy.
- Make basic targets compile or fail clearly.
- Set up tests and optional docs generation.

#### Tasks

- audit `CMakeLists.txt`
- wire all include paths and library links
- define options for optional features:
  - Lua
  - registry
  - docs
  - tests
- integrate AzmaTest
- ensure `build.sh` is a thin wrapper around CMake
- ensure pkg-config generation works via `sattyre.pc.in`

#### Dependency intent

- `fmtlib`: formatting utilities
- `log4cplus`: logging backend
- `dynalo`: dynamic loading abstraction
- `cpp-httplib`, `libssh2`: registry features
- `libarchive`: bundles
- `lua`, `sol2`: Lua support
- `serdetk`: manifest parsing
- `klyspec`: CLI
- `AzmaTest`: tests/fuzzing

#### Tips

- Prefer feature flags like `SATTYRE_ENABLE_LUA`, `SATTYRE_ENABLE_REGISTRY`.
- Do not hard-require everything for a minimal local build if optionality is feasible.
- If some dependencies are not always available, make those components compile conditionally.

#### Exit criteria

- configure works
- build targets are organized
- tests can be enabled
- optional subsystems are explicit

---

### Stage 2 — Core SAT and SMT Front-End Correctness

This stage turns the public SAT/SMT front door into something real.

#### Goals

- make `SattyreSAT.hpp/.cpp` useful and reliable
- make `SattyreSMT.hpp/.cpp` minimally correct
- define result/model semantics clearly

#### SAT tasks

- harden DIMACS parser:
  - comments
  - problem line parsing
  - clause termination
  - malformed input handling
  - variable count consistency checks
- define clear semantics for:
  - empty formula
  - empty clause
  - out-of-range literals

#### SMT tasks

- keep initial SMT support text-oriented
- load SMT-LIB robustly
- add basic validation/error reporting
- defer deep SMT AST work unless needed

#### Tips

- SAT parser behavior should be deterministic and well-tested.
- Start with text-preserving SMT-LIB ingestion before theory semantics.
- Use AzmaTest to add parser tests and malformed input tests.

#### Exit criteria

- SAT parser is not a stub
- SMT loader is not a stub
- tests cover common and malformed cases
- API behavior is documented

---

### Stage 3 — Solver ABI and Native Solver Adapters

This is the backbone of the BYOS model.

#### Goals

- make the C ABI stable and versioned
- make the C++ adapter usable
- define lifecycle and capability contracts

#### Tasks

- extend `Sattyre-Solver.h` with:
  - ABI version
  - feature/capability metadata
  - structured error/status behavior
- define what a solver must export
- make `NativeSolverAdapter` load and call a real or mock native solver
- clarify ownership and lifetime rules
- define model extraction semantics

#### Built-in solvers

The existing files suggest reference solvers:

- `BruteForceSolver.cpp`
- `DPLLSolver.cpp`
- `CDCLSolver.cpp`

Implement in that order:

1. brute-force as correctness oracle for tiny cases
2. DPLL as first practical baseline
3. CDCL later as optimized solver

#### Tips

- Brute-force solver is not for performance; it is for correctness.
- DPLL should be clean and understandable before CDCL begins.
- Keep ABI thin; complexity belongs behind it.
- Add capability flags such as:
  - SAT-only
  - incremental support
  - assumptions support
  - model production

#### Exit criteria

- C ABI documented and versioned
- C++ adapter functional
- at least one solver genuinely solves small SAT instances
- tests compare expected SAT/UNSAT outcomes

---

### Stage 4 — Plugin System, Library System, and Dynamic Loading

This stage activates modularity beyond solvers.

#### Goals

- define plugin and library contracts clearly
- unify loading behavior
- replace ad hoc platform loading with `dynalo` where appropriate

#### Tasks

- refine `Sattyre-Plugin.h/.hpp`
- refine `Sattyre-Library.h/.hpp`
- define:
  - metadata
  - init/shutdown
  - compatibility checks
  - symbol naming conventions
- refactor `Dylib.hpp/.cpp` to use `dynalo` as the preferred backend
- document loading search paths

#### Important distinction

Keep these concepts separate:

- **solver**: solves SAT/SMT problems
- **plugin**: extends behavior/tooling/runtime
- **library**: reusable runtime component exposed to Sattyre ecosystem

Do not collapse them into one vague “module” type.

#### Tips

- Add explicit compatibility verification before initialization.
- Fail early on missing symbols.
- Expose minimal helper code in C++ wrappers, but keep C ABI stable.

#### Exit criteria

- shared library loading is consistent cross-platform
- plugin and library ABIs are no longer vague stubs
- at least one test/demo module can be loaded

---

### Stage 5 — Packaging: Manifests, Bundles, and Installation

This stage makes distribution real.

#### Goals

- make manifests parse correctly
- make bundles round-trip correctly
- make install/remove behavior deterministic

#### Tasks

- implement `Manifest.cpp` using `serdetk`
- support declared formats:
  - JSON
  - YAML
  - S-Expr
  - XML
- normalize parsed data into `PackageManifest`
- validate required fields:
  - name
  - version
  - type
- implement bundle creation/extraction using `libarchive`
- define destination layout for:
  - solvers
  - plugins
  - libraries
  - Lua addons
  - docs/examples if packaged

#### Installer tasks

- inspect manifest before installation
- verify type and destination
- support overwrite policy
- support safe removal
- prepare for dependency resolution even if full resolver comes later

#### Tips

- Manifest normalization is key: parse many formats, produce one internal struct.
- Bundle logic should preserve permissions and paths safely.
- Guard against archive traversal issues like `../` paths.

#### Exit criteria

- all supported manifest formats parse into the same internal representation
- bundle create/extract works
- install/remove works on local packages
- tests cover malformed manifests and path safety

---

### Stage 6 — Registry Client/Server and Remote Package Flow

This stage adds package hosting and transport.

#### Goals

- implement a basic usable registry
- support publish and fetch workflows
- keep security considerations explicit

#### Tasks

- implement HTTP routes with `cpp-httplib`
- define a simple package index format
- implement package upload/download
- integrate `libssh2` for authenticated deployment where needed
- clarify difference between:
  - serving packages over HTTP
  - publishing packages over authenticated channels

#### Recommended first registry features

- list packages
- fetch manifest
- fetch bundle
- upload bundle
- simple health endpoint

#### Tips

- Start local-first: localhost registry before remote deployment polish.
- Keep metadata format simple and versioned.
- Log every upload/download/install event meaningfully.
- Design registry endpoints to be scriptable.

#### Exit criteria

- local registry can serve packages
- client can publish/download a package
- manifests and bundles integrate with installer
- docs explain the workflow

---

### Stage 7 — Lua Runtime and Scripting Surface

This stage enables scripting and addon ergonomics.

#### Goals

- make embedded Lua stable
- expose useful bindings
- keep scripting API small at first

#### Tasks

- harden `LuaRuntime.cpp`
- formalize initialization/shutdown ownership
- use `sol2` to expose clean bindings
- implement:
  - `lsat`
  - `lsmt`
  - `lsattyre`

#### Recommended first bindings

- create/load SAT problem
- create/load SMT problem
- invoke a solver
- inspect result
- inspect simple model
- query runtime/package paths

#### Tips

- Expose a small clean Lua API instead of mirroring all C++ internals.
- Ensure Lua state errors surface clearly to logs and callers.
- Avoid binding unstable internals too early.

#### Exit criteria

- Lua runtime initializes reliably
- at least one binding namespace is genuinely useful
- example Lua-driven workflow can be demonstrated

---

### Stage 8 — CLI Polish, Testing Depth, Documentation, and Release Readiness

This is the consolidation stage.

#### Goals

- make the tools useful
- make the project testable and releasable
- align docs with reality

#### CLI tasks

Use `klyspec` to define robust interfaces for:

- `sattyre-cli`
  - solve SAT/SMT inputs
  - choose solver
  - choose output mode
- `sattyre-packman`
  - validate manifest
  - pack
  - unpack
  - install
  - remove
  - list
- `sattyre-registry`
  - serve
  - publish
  - fetch
  - inspect

Use `fmtlib` for consistent output formatting.

#### Test tasks

Use AzmaTest for:

- unit tests
- parser tests
- ABI contract tests
- bundle/install tests
- registry smoke tests
- Lua smoke tests
- fuzz targets for DIMACS and manifests

#### Documentation tasks

Update:

- `README.md`
- `docs/architecture.md`
- `docs/dependencies.md`

Add examples that match actual behavior.

#### Release tasks

- verify install rules
- verify pkg-config metadata
- verify public headers install cleanly
- define semantic versioning policy
- define compatibility policy

#### Exit criteria

- user-facing tools are functional
- tests cover critical flows
- docs match implementation
- project can be packaged and installed

---

## 6. Coding Standards, File-by-File Expectations, and Practical Tactics

This section tells you how to write code in this repo without making a mess.

### 6.1 General coding style

- Prefer modern C++ for implementation files.
- Keep C ABI files simple and conservative.
- Avoid clever metaprogramming unless it clearly reduces complexity.
- Prefer readable code over compressed code.

### 6.2 Header hygiene

For public headers:

- minimize includes
- use forward declarations where reasonable
- keep implementation-only types out of headers
- document ownership and lifetime

### 6.3 Source file expectations

Each `.cpp` or `.c` should have a focused responsibility.

Examples:

- `Manifest.cpp` should parse and normalize manifests, not also install packages.
- `RegistryClient.cpp` should not become a catch-all deployment utility kitchen sink.
- `LuaBindings.cpp` should register bindings, not own Lua runtime lifecycle.

### 6.4 Testing tactics

Whenever implementing a feature, think immediately:

- what are the normal cases?
- what are the malformed cases?
- what are boundary cases?
- what are portability traps?

Examples:

- DIMACS parser:
  - extra spaces
  - missing `p` line
  - invalid literals
  - zero-only clause
- manifest parsing:
  - missing required fields
  - wrong type values
  - malformed documents
- bundle extraction:
  - nested paths
  - duplicate entries
  - path traversal attempts

### 6.5 Portability tactics

Because this is a systems library:

- avoid Unix-only assumptions in paths and loading
- prefer `std::filesystem` where practical
- let `dynalo` own platform-specific shared library pain
- keep shell scripts auxiliary, not foundational

### 6.6 Performance tactics

Do not optimize prematurely.

Priority order:

1. correctness
2. API coherence
3. testability
4. portability
5. performance

But for solvers, avoid obviously wasteful data copying in hot paths once functionality exists.

### 6.7 Documentation tactics

When stabilizing a subsystem, update docs immediately.

If behavior differs from older docs, the docs are wrong until fixed.

---

## 7. Common Pitfalls, Recovery Strategies, and Tips for Smart Progress

Here is where Codex can save time by not making predictable mistakes.

### Pitfall 1: Building features through the CLI first

Do not start by making the CLI “look complete” while library internals are fake.

**Correct order:** library core first, CLI exposure second.

### Pitfall 2: Treating every component as the same kind of module

Solvers, plugins, libraries, and Lua addons are different. Preserve the distinction.

### Pitfall 3: Overdesigning manifests before parsing one correctly

First get one canonical internal structure working. Then support all formats.

### Pitfall 4: Implementing CDCL before parser and DPLL correctness

Do not chase solver sophistication while the front-end and baseline correctness are weak.

### Pitfall 5: Locking everything to mandatory dependencies

Optional features should remain optional when feasible.

### Pitfall 6: Letting docs drift from code

As soon as implementation becomes real, update docs.

### Pitfall 7: Using too much logging in hot or low-level loops

Log lifecycle and events, not every literal or clause in production paths.

### Pitfall 8: Making ABI changes casually

Any public ABI change should be deliberate, versioned, and documented.

---

### Smart progress tips

#### Tip A: Use one “reference truth” per subsystem

- SAT solving: brute-force on tiny problems
- manifests: normalized `PackageManifest`
- loading: one dynalo-backed loader abstraction
- registry: one versioned package metadata shape

#### Tip B: Build tiny vertical slices

Example good vertical slice:

- parse YAML manifest
- create bundle
- install package locally
- load a plugin from installed path

That proves more than implementing 40% of five independent files.

#### Tip C: Keep examples executable

`examples/example.cnf` and `examples/example.smt2` should remain useful smoke tests.

#### Tip D: Add internal helper layers only when duplication appears twice or more

Do not build abstract factories for imaginary future needs.

#### Tip E: For every subsystem, define invariants

Examples:

- every installed package has a valid manifest
- every loaded solver passes ABI compatibility check
- every extracted archive path stays under destination root

---

## 8. Preferred Work Sequence for Codex on This Repository

When asked to implement or improve Sattyre, use this operating sequence.

### Step 1: Classify the request

Which subsystem is it?

- build
- SAT/SMT core
- solver ABI
- plugin/library
- package
- registry
- Lua
- CLI/docs/tests

### Step 2: Read the minimum required context

Usually:

- one public header
- one source file
- relevant CMake section
- existing test/example if present

### Step 3: State the exact target behavior

In a few lines only.

Example:

- “Make DIMACS parser reject unterminated clauses and validate variable bounds.”
- “Replace manual dynamic loading backend with dynalo while preserving `DynamicLibrary` interface.”

### Step 4: Implement the smallest correct change

Prefer:

- precise edits
- no architecture churn
- preserve names and file layout

### Step 5: Verify immediately

Use whichever applies:

- compile target
- run test
- inspect example
- static API review

### Step 6: Leave the repo better than you found it

If you touch behavior, consider also touching:

- test
- docs
- example
- build wiring

### Step 7: Report succinctly

When done, summarize:

- what changed
- what remains
- what to verify next

---

## Final guidance to Codex

Be deliberate, not grandiose.

This repository already has a strong skeleton. Your job is not to invent a new framework. Your job is to **turn the scaffold into a working Sattyre** with discipline.

If you are unsure what to do next, pick the earliest incomplete stage and make it real before moving on. The best progress here will come from:

- stable build
- real parser
- real solver baseline
- real loader ABI
- real package flow
- real registry basics
- real Lua bindings
- then polished CLI/tests/docs

Implement it stage by stage, carefully, like a mouse inspecting cheese: close, methodical, suspicious of traps, and always looking for the cleanest bite first.