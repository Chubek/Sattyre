#include "sattyre/Manifest.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using sattyre::load_manifest;
using sattyre::parse_manifest_text;

static std::filesystem::path write_fixture(const std::string& contents, const char* name) {
  const auto path = std::filesystem::temp_directory_path() / name;
  std::ofstream out(path);
  out << contents;
  return path;
}

static void expect_throw(const std::string& contents, const char* name) {
  const auto path = write_fixture(contents, name);
  bool threw = false;
  try {
    (void)load_manifest(path.string());
  } catch (const std::runtime_error&) {
    threw = true;
  }
  if (!threw) {
    throw std::runtime_error(std::string("expected failure for ") + name);
  }
}

int main() {
  {
    const auto path = write_fixture(
      "name: example-solver\nversion: 0.1.0\ntype: solver\ndescription: Example\ndependencies: [fmt, log4cplus]\n",
      "sattyre-manifest-yaml.yaml");
    const auto manifest = load_manifest(path.string());
    if (manifest.name != "example-solver" || manifest.version != "0.1.0" || manifest.type != "solver") {
      throw std::runtime_error("unexpected manifest parse result");
    }
    if (manifest.dependencies.size() != 2) {
      throw std::runtime_error("unexpected dependency count");
    }
  }

  expect_throw("version: 0.1.0\ntype: solver\n", "sattyre-manifest-missing-name.yaml");
  expect_throw("name: example\nversion: 0.1.0\ntype: invalid\n", "sattyre-manifest-bad-type.yaml");

  {
    const auto manifest = parse_manifest_text(
      "{\n"
      "  \"name\":\"inline-plugin\",\n"
      "  \"version\":\"1.0.0\",\n"
      "  \"type\":\"plugin\",\n"
      "  \"dependencies\":[\"a\",\"b\"]\n"
      "}\n",
      "inline.json");
    if (manifest.name != "inline-plugin" || manifest.type != "plugin" || manifest.dependencies.size() != 2) {
      throw std::runtime_error("unexpected inline manifest parse result");
    }
  }

  std::cout << "Manifest tests passed\n";
  return EXIT_SUCCESS;
}
