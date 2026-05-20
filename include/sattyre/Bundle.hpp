#pragma once

#include <string>

namespace sattyre {

bool create_bundle(const std::string& source_dir, const std::string& out_file);
bool extract_bundle(const std::string& bundle_file, const std::string& out_dir);

} // namespace sattyre
