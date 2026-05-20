#include "sattyre/LuaBindings.hpp"
#include "sattyre/LuaRuntime.hpp"
#include "sattyre/Log.hpp"
#include "sattyre/Version.hpp"

extern "C" {
#include <lua.h>
}

namespace sattyre {

namespace {

int lsat_ping(lua_State* L) {
  lua_pushstring(L, "lsat");
  return 1;
}

int lsmt_ping(lua_State* L) {
  lua_pushstring(L, "lsmt");
  return 1;
}

int lsattyre_version(lua_State* L) {
  lua_pushstring(L, SATTYRE_VERSION_STRING);
  return 1;
}

} // namespace

void bind_lsat(LuaRuntime& runtime) {
  if (!runtime.state()) {
    log::warn("Cannot bind lsat: Lua runtime is not initialized");
    return;
  }
  lua_register(runtime.state(), "lsat_ping", lsat_ping);
  log::info("Bound lsat namespace");
}

void bind_lsmt(LuaRuntime& runtime) {
  if (!runtime.state()) {
    log::warn("Cannot bind lsmt: Lua runtime is not initialized");
    return;
  }
  lua_register(runtime.state(), "lsmt_ping", lsmt_ping);
  log::info("Bound lsmt namespace");
}

void bind_lsattyre(LuaRuntime& runtime) {
  if (!runtime.state()) {
    log::warn("Cannot bind lsattyre: Lua runtime is not initialized");
    return;
  }
  lua_register(runtime.state(), "lsattyre_version", lsattyre_version);
  log::info("Bound lsattyre namespace");
}

} // namespace sattyre
