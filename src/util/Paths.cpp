#include "sattyre/Paths.hpp"

#include <cstdlib>

namespace sattyre::paths {

static std::filesystem::path env_or_default(const char* key, const char* fallback) {
  const char* val = std::getenv(key);
  return val ? std::filesystem::path(val) : std::filesystem::path(fallback);
}

std::filesystem::path home() {
  const char* sattyre_home = std::getenv("SATTYRE_HOME");
  if (sattyre_home && sattyre_home[0] != '\0') {
    return std::filesystem::path(sattyre_home);
  }
  const char* user_home = std::getenv("HOME");
  if (user_home && user_home[0] != '\0') {
    return std::filesystem::path(user_home) / ".sattyre";
  }
  return std::filesystem::path("/tmp/.sattyre");
}
std::filesystem::path include_dir(){ return home() / "include"; }
std::filesystem::path lib_dir()    { return home() / "lib"; }
std::filesystem::path solver_dir() { return lib_dir() / "solvers"; }
std::filesystem::path plugin_dir() { return lib_dir() / "plugins"; }
std::filesystem::path lua_dir()    { return lib_dir() / "lua"; }
std::filesystem::path package_dir(){ return home() / "share" / "packages"; }
std::filesystem::path doc_dir()    { return home() / "doc"; }

} // namespace sattyre::paths
