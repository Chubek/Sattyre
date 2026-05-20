#pragma once

struct lua_State;

namespace sattyre {

class LuaRuntime {
public:
  LuaRuntime();
  ~LuaRuntime();

  bool init();
  void shutdown();
  lua_State* state() const;

private:
  lua_State* L_;
};

} // namespace sattyre
