#include "sattyre/Paths.hpp"

#include <cstdlib>

namespace sattyre::paths {

static std::filesystem::path env_or_default(const char* key, const char* fallback) {
  const char* val = std::getenv(key);
  return val ? std::filesystem::path(val) : std::filesystem::path(fallback);
}

std::filesystem::path home()       { return env_or_default("SATTYRE_HOME", "/usr/local/sattyre"); }
std::filesystem::path include_dir(){ return home() / "include"; }
std::filesystem::path lib_dir()    { return home() / "lib"; }
std::filesystem::path solver_dir() { return lib_dir() / "solvers"; }
std::filesystem::path plugin_dir() { return lib_dir() / "plugins"; }
std::filesystem::path lua_dir()    { return lib_dir() / "lua"; }
std::filesystem::path package_dir(){ return home() / "share" / "packages"; }
std::filesystem::path doc_dir()    { return home() / "doc"; }

} // namespace sattyre::paths
