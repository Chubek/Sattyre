#pragma once

#include <string>
#include <vector>

namespace sattyre {

struct PackageManifest {
  std::string name;
  std::string version;
  std::string type;
  std::vector<std::string> dependencies;
  std::string description;
};

PackageManifest parse_manifest_text(const std::string& text, const std::string& source_name = {});
PackageManifest load_manifest(const std::string& path);
std::string manifest_to_json(const PackageManifest& manifest);

} // namespace sattyre
