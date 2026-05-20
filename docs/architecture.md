# Architecture

Planned subsystems:

- SAT interface
- SMT interface
- solver ABI
- plugin ABI
- library ABI
- package manager
- package registry
- Lua addon system

Third-party integrations:

- `cpp-httplib` for registry HTTP serving/client work
- `fmt` for formatting
- `klyspec` for CLI definitions
- `libarchive` for bundle creation and extraction
- `libssh2` for deployment/authentication
- `log4cplus` for logging
- `lua` and `sol2` for addon embedding
- `serdetk` for JSON/XML/YAML/S-Expr manifests
