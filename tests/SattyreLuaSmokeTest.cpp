#include "sattyre/LuaBindings.hpp"
#include "sattyre/LuaRuntime.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

int main() {
  sattyre::LuaRuntime runtime;
  if (!runtime.init()) {
    throw std::runtime_error("Lua runtime init failed");
  }

  sattyre::bind_lsat(runtime);
  sattyre::bind_lsmt(runtime);
  sattyre::bind_lsattyre(runtime);

  lua_State* L = runtime.state();
  if (!L) {
    throw std::runtime_error("Lua state missing");
  }

  if (luaL_dostring(L, "return lsat_ping()") != LUA_OK) {
    throw std::runtime_error("lsat_ping invocation failed");
  }
  std::string lsat = lua_tostring(L, -1);
  lua_pop(L, 1);
  if (lsat != "lsat") {
    throw std::runtime_error("lsat_ping result mismatch");
  }

  if (luaL_dostring(L, "return lsattyre_version()") != LUA_OK) {
    throw std::runtime_error("lsattyre_version invocation failed");
  }
  const char* version = lua_tostring(L, -1);
  if (version == nullptr || std::string(version).empty()) {
    throw std::runtime_error("lsattyre_version result missing");
  }
  lua_pop(L, 1);

  runtime.shutdown();
  return EXIT_SUCCESS;
}
