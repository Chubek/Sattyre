\page chapter11 Lua Runtime and Binding Surface

\brief Stage 7 embedded scripting with controlled API

## Scope

This chapter documents `LuaRuntime` lifecycle and binding registration functions.
The scripting API should be intentionally small and useful.
Expose stable operations before internal details.

```text
sattyre::LuaRuntime rt;
if (!rt.init()) {
  throw std::runtime_error("Lua init failed");
}
sattyre::bind_lsat(rt);
sattyre::bind_lsmt(rt);
sattyre::bind_lsattyre(rt);
```

### Quick recap

- Keep this section aligned with Stage 7 embedded scripting with controlled API.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Binding namespaces

`lsat` for SAT loading and solving.
`lsmt` for SMT loading and solve status pathways.
`lsattyre` for runtime paths and package environment helpers.

```text
-- solve_sat.lua
local p = lsat.parse_dimacs("examples/example.cnf")
local solver = lsat.native_solver()
solver:load(p)
local result = solver:solve()
print("result:", result)
```

### Quick recap

- Keep this section aligned with Stage 7 embedded scripting with controlled API.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Error reporting

Surface Lua errors to callers and logs explicitly.
Do not swallow script failures inside binding wrappers.
Keep ownership clear: runtime owns `lua_State` lifecycle.

```text
// Lua errors should propagate to the caller in a clear way.
// The binding layer should not hide the failure origin.
```

### Quick recap

- Keep this section aligned with Stage 7 embedded scripting with controlled API.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Scripting patterns

Expose create/load/solve/inspect primitives first.
Keep module tables small and discoverable.
Reuse the same data flow as the C++ APIs.

```text
print(lsat.version())
print(lsatyre and "runtime ready" or "runtime missing")
```

### Quick recap

- Keep this section aligned with Stage 7 embedded scripting with controlled API.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Testing suggestions

Use a Lua smoke test that initializes runtime and executes a tiny script.
Validate both success path and deliberate failure path.
Keep scripts in examples or tests for reproducibility.

```text
cmake -S . -B build/lua -DSATTYRE_ENABLE_LUA=ON -DSATTYRE_ENABLE_TESTS=ON
cmake --build build/lua -j
```

### Quick recap

- Keep this section aligned with Stage 7 embedded scripting with controlled API.
- Prefer explicit statuses and deterministic behavior.
- Promote examples into tests when behavior stabilizes.

## Chapter summary

Stage 7 success means reliable runtime init and practical first bindings.
Small, focused bindings are easier to keep stable.
This forms the scripting bridge for advanced user workflows.

```text
// Bindings should reflect stable public APIs, not internal prototypes.
```

### Quick recap

- Keep this section aligned with Stage 7 embedded scripting with controlled API.
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
