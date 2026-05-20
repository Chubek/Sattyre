#include "sattyre/LuaRuntime.hpp"
#include "sattyre/Log.hpp"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace sattyre {

LuaRuntime::LuaRuntime() : L_(nullptr) {}
LuaRuntime::~LuaRuntime() { shutdown(); }

bool LuaRuntime::init() {
  if (L_) return true;
  L_ = luaL_newstate();
  if (!L_) {
    log::error("Failed to create Lua state");
    return false;
  }
  luaL_openlibs(L_);
  log::info("Lua runtime initialized");
  return true;
}

void LuaRuntime::shutdown() {
  if (L_) {
    lua_close(L_);
    L_ = nullptr;
    log::info("Lua runtime shutdown");
  }
}

lua_State* LuaRuntime::state() const {
  return L_;
}

} // namespace sattyre
