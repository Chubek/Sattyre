#pragma once

namespace sattyre {

class LuaRuntime;

void bind_lsat(LuaRuntime& runtime);
void bind_lsmt(LuaRuntime& runtime);
void bind_lsattyre(LuaRuntime& runtime);

} // namespace sattyre
