#pragma once

#include <filesystem>
#include <string>

namespace sattyre::paths {

std::filesystem::path home();
std::filesystem::path include_dir();
std::filesystem::path lib_dir();
std::filesystem::path solver_dir();
std::filesystem::path plugin_dir();
std::filesystem::path lua_dir();
std::filesystem::path package_dir();
std::filesystem::path doc_dir();

} // namespace sattyre::paths
