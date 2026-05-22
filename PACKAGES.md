# Sattyre Packages

This document explains how Sattyre packages are structured, how to stage a package, how to bundle it, and how to install or remove it again.

The main example in this repository is `examples/package-exampe/picoSAT`, which is a small and practical SAT solver distribution. Use it as a template for packaging other solvers, shared libraries, and plugins.

That example now includes:

- `manifest.json`
- `package.sh`

## What a package is

A Sattyre package is a directory tree plus a manifest.

At minimum, a package should contain:

- a manifest file
- the payload to install
- a stable package name
- a version
- a package type

Sattyre currently recognizes these package types:

- `solver`
- `plugin`
- `library`

The package type controls where the installer places the payload.

## The package API

The public package API lives in:

- `include/sattyre/Manifest.hpp`
- `include/sattyre/Bundle.hpp`
- `include/sattyre/Installer.hpp`

The core entry points are:

- `parse_manifest_text(...)`
- `load_manifest(...)`
- `create_bundle(...)`
- `extract_bundle(...)`
- `install_package(...)`
- `remove_package(...)`

## Manifest format

A manifest describes the package metadata.

The current structure is:

- `name`
- `version`
- `type`
- `dependencies`
- `description`

Example JSON manifest:

```json
{
  "name": "picosat",
  "version": "965",
  "type": "solver",
  "dependencies": [],
  "description": "The picoSAT SAT solver"
}
```

Example YAML manifest:

```yaml
name: picosat
version: "965"
type: solver
dependencies: []
description: The picoSAT SAT solver
```

The installer expects the manifest to be part of the package payload, usually as one of:

- `manifest.json`
- `manifest.yaml`
- `manifest.yml`
- `manifest.xml`
- `manifest.sexp`

## The picoSAT example

`examples/package-exampe/picoSAT` is a compact solver tree that makes a good packaging example because it is small, easy to build, and easy to verify.

The included `package.sh` script demonstrates the full flow:

1. stage the solver and docs
2. create a bundle with `sattyre-packman pack`
3. install it with `sattyre-packman install`
4. clean the generated staging and bundle files

The important idea is not picoSAT itself, but the packaging pattern:

1. build the upstream project
2. stage the files you want installed
3. add a manifest
4. bundle the staged tree
5. install the bundle into the Sattyre package root

That pattern works for:

- a solver binary
- a solver library
- a reusable runtime library
- a plugin module

## Recommended package layout

For a solver package such as picoSAT, a good staged tree looks like this:

```text
staging/
  manifest.json
  bin/
    picosat
  include/
    picosat.h
  lib/
    ...
```

For a library package, the staged tree usually looks like:

```text
staging/
  manifest.json
  include/
    mylib.hpp
  lib/
    libmylib.so
```

For a plugin package:

```text
staging/
  manifest.json
  lib/
    plugins/
      myplugin.so
```

The exact payload is up to you, but the manifest `type` should match what the package actually contains.

## Building picoSAT for packaging

The `picoSAT` example ships with its own build scripts, so the normal workflow is:

1. build the solver with the example’s local instructions
2. collect the built artifacts
3. stage them into a package directory

In practice, you usually want to stage only the files that consumers need:

- the solver executable
- public headers, if any
- shared libraries, if the solver is shipped as a library
- license files
- the manifest

Do not bundle intermediate object files, temporary build directories, or generated compiler scratch files.

## How to stage a package

A package should be prepared in a clean staging directory before bundling.

Suggested workflow:

1. create a fresh directory
2. copy the installable files into it
3. write `manifest.json`
4. verify the directory tree
5. create the bundle

Example:

```text
stage/
  manifest.json
  bin/
    picosat
  LICENSE
```

If you are packaging a library, keep the public headers and shared objects together in a predictable layout.

## Bundling

Sattyre exposes `create_bundle(source_dir, out_file)`.

That function packages a directory tree into a distributable archive-like bundle.

Conceptually:

- `source_dir` is your staged package directory
- `out_file` is the bundle you will publish or install

The bundle should be created from the staged tree, not from the live build directory.

That is important because the staged tree is:

- reproducible
- easier to audit
- easier to sign or checksum
- safer to install

## Installing

Sattyre exposes `install_package(bundle_file, root)` and `install_package(bundle_file, root, overwrite)`.

The installer:

1. checks that the bundle exists
2. extracts into a staging directory
3. finds the manifest
4. parses the manifest
5. chooses an install destination from the package type
6. moves the staged tree into place

Current destination rules:

- `solver` → `lib/solvers/<name>`
- `plugin` → `lib/plugins/<name>`
- `library` → `lib/<name>`

If you do not pass a root path, Sattyre uses the default home package root.

## Removing

Sattyre exposes `remove_package(package_name, root)`.

Removal is name-based and searches the standard locations for:

- solver packages
- plugin packages
- library packages

This means package names should be stable and unique.

## A practical picoSAT workflow

Here is the most useful way to think about `picoSAT` as a package:

1. build picoSAT
2. stage it into `stage/`
3. add `manifest.json`
4. bundle `stage/` into `picosat.sattyre`
5. install the bundle into the package root
6. verify the installed solver files are present

In pseudocode:

```text
build picoSAT
copy outputs into stage/
write stage/manifest.json
bundle stage/ -> picosat.bundle
install picosat.bundle into ~/.sattyre
```

Or use the helper directly:

```sh
cd examples/package-exampe/picoSAT
./configure.sh && make
./package.sh
```

## Example manifest for picoSAT

```json
{
  "name": "picosat",
  "version": "965",
  "type": "solver",
  "dependencies": [],
  "description": "picoSAT packaged for Sattyre"
}
```

If you are shipping a library instead of a solver, change only the `type` field and the staged payload.

## Common mistakes

- forgetting the manifest
- mismatching the manifest `type` and the payload
- bundling from the build directory instead of the staged directory
- shipping temporary files
- using an unstable package name
- installing into the wrong root

## Good packaging rules

- keep the package tree small
- keep the manifest explicit
- keep the install layout predictable
- keep names lowercase and stable
- include a license file when redistributing third-party code
- test install and remove before publishing

## Notes on the current implementation

The current package code is intentionally small and conservative.

That means:

- manifests are validated for required fields
- invalid package types are rejected
- bundles are extracted through a staging area
- installation is type-aware
- removal is name-based

This is enough to support real package workflows while the rest of Sattyre’s packaging system grows.

## Suggested next step

If you are packaging more than one project, keep one shared staging convention for all of them.

For example:

- `manifest.json` at the root
- `bin/` for executables
- `include/` for public headers
- `lib/` for shared objects
- `share/` for docs and examples

That convention makes packaging easier to automate later with CLI tooling.
