#include "sattyre/Manifest.hpp"
#include "sattyre/Log.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <regex>
#include <sstream>
#include <stdexcept>

#if defined(SATTYRE_HAVE_SERDETK)
#include "SerdeTk.hpp"
#endif

namespace sattyre {

namespace {

std::string trim(const std::string& value) {
  const auto begin = std::find_if_not(value.begin(), value.end(), [](unsigned char c) { return std::isspace(c) != 0; });
  const auto end = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char c) { return std::isspace(c) != 0; }).base();
  if (begin >= end) {
    return {};
  }
  return std::string(begin, end);
}

std::string strip_quotes(const std::string& value) {
  const std::string trimmed = trim(value);
  if (trimmed.size() >= 2 &&
      ((trimmed.front() == '"' && trimmed.back() == '"') ||
       (trimmed.front() == '\'' && trimmed.back() == '\''))) {
    return trimmed.substr(1, trimmed.size() - 2);
  }
  return trimmed;
}

std::string extract_field(const std::string& content, const std::string& key) {
  std::smatch match;

  const std::regex json_re("\"" + key + "\"\\s*:\\s*\"([^\"]*)\"");
  if (std::regex_search(content, match, json_re) && match.size() > 1) {
    return match[1].str();
  }

  const std::regex yaml_re("(^|\\n)\\s*" + key + "\\s*:\\s*([^\\n]+)");
  if (std::regex_search(content, match, yaml_re) && match.size() > 2) {
    return strip_quotes(match[2].str());
  }

  const std::regex xml_re("<" + key + ">\\s*([^<]+)\\s*</" + key + ">");
  if (std::regex_search(content, match, xml_re) && match.size() > 1) {
    return trim(match[1].str());
  }

  const std::regex sexpr_re("\\(" + key + "\\s+\"?([^\\)\\\"]+)\"?\\)");
  if (std::regex_search(content, match, sexpr_re) && match.size() > 1) {
    return trim(match[1].str());
  }

  return {};
}

std::vector<std::string> extract_dependencies(const std::string& content) {
  std::smatch match;
  std::vector<std::string> deps;

  const std::regex json_or_yaml_list_re("\"dependencies\"\\s*:\\s*\\[([^\\]]*)\\]|(^|\\n)\\s*dependencies\\s*:\\s*\\[([^\\]]*)\\]");
  if (std::regex_search(content, match, json_or_yaml_list_re)) {
    std::string body;
    if (match.size() > 1 && match[1].matched) {
      body = match[1].str();
    } else if (match.size() > 3 && match[3].matched) {
      body = match[3].str();
    }
    std::stringstream stream(body);
    std::string item;
    while (std::getline(stream, item, ',')) {
      const std::string dep = strip_quotes(item);
      if (!dep.empty()) {
        deps.push_back(dep);
      }
    }
    return deps;
  }

  const std::regex xml_dep_re("<dependency>\\s*([^<]+)\\s*</dependency>");
  auto begin = std::sregex_iterator(content.begin(), content.end(), xml_dep_re);
  auto end = std::sregex_iterator();
  for (auto it = begin; it != end; ++it) {
    const std::string dep = trim((*it)[1].str());
    if (!dep.empty()) {
      deps.push_back(dep);
    }
  }
  return deps;
}

void validate_manifest(const PackageManifest& manifest, const std::string& path) {
  if (manifest.name.empty()) {
    throw std::runtime_error("Manifest missing required field 'name': " + path);
  }
  if (manifest.version.empty()) {
    throw std::runtime_error("Manifest missing required field 'version': " + path);
  }
  if (manifest.type.empty()) {
    throw std::runtime_error("Manifest missing required field 'type': " + path);
  }
  if (manifest.type != "solver" && manifest.type != "plugin" && manifest.type != "library") {
    throw std::runtime_error("Manifest has invalid type '" + manifest.type + "': " + path);
  }
}

PackageManifest parse_manifest_text_legacy(const std::string& content, const std::string& source_name) {
  PackageManifest m;
  m.name = extract_field(content, "name");
  m.version = extract_field(content, "version");
  m.type = extract_field(content, "type");
  m.description = extract_field(content, "description");
  m.dependencies = extract_dependencies(content);
  validate_manifest(m, source_name);
  return m;
}

} // namespace

PackageManifest parse_manifest_text(const std::string& text, const std::string& source_name) {
#if defined(SATTYRE_HAVE_SERDETK)
  const std::string ext = std::filesystem::path(source_name).extension().string();
  try {
    serdetk::Document doc;
    if (ext == ".json") {
      doc = serdetk::json::from_string(text);
    } else if (ext == ".yaml" || ext == ".yml") {
      doc = serdetk::yaml::from_string(text);
    } else if (ext == ".xml") {
      doc = serdetk::xml::from_string(text);
    } else if (ext == ".sexp" || ext == ".scm" || ext == ".lisp") {
      doc = serdetk::sexpr::from_string(text);
    } else {
      doc = serdetk::json::from_string(text);
    }

    if (!doc.root.is_object()) {
      throw std::runtime_error("Manifest root must be an object");
    }

    const serdetk::Object& root = doc.root.as_object();
    auto get_string = [&root](const std::string& key) -> std::string {
      auto it = root.fields.find(key);
      if (it == root.fields.end() || !it->second.is_string()) {
        return {};
      }
      return trim(it->second.as_string());
    };

    PackageManifest m;
    m.name = get_string("name");
    m.version = get_string("version");
    m.type = get_string("type");
    m.description = get_string("description");

    auto dep_it = root.fields.find("dependencies");
    if (dep_it != root.fields.end() && dep_it->second.is_array()) {
      for (const auto& value : dep_it->second.as_array().items) {
        if (value.is_string()) {
          const std::string dep = trim(value.as_string());
          if (!dep.empty()) {
            m.dependencies.push_back(dep);
          }
        }
      }
    }
    if (m.name.empty()) {
      m.name = extract_field(text, "name");
    }
    if (m.version.empty()) {
      m.version = extract_field(text, "version");
    }
    if (m.type.empty()) {
      m.type = extract_field(text, "type");
    }
    if (m.description.empty()) {
      m.description = extract_field(text, "description");
    }
    if (m.dependencies.empty()) {
      m.dependencies = extract_dependencies(text);
    }

    validate_manifest(m, source_name);
    return m;
  } catch (const std::exception& error) {
    if (!source_name.empty()) {
      log::warn(std::string("serdetk manifest parse failed for ") + source_name + ": " + error.what());
    }
  }
#endif
  return parse_manifest_text_legacy(text, source_name);
}

PackageManifest load_manifest(const std::string& path) {
  log::info("Loading package manifest: " + path);

  std::ifstream input(path);
  if (!input) {
    throw std::runtime_error("Failed to open manifest: " + path);
  }
  std::stringstream buffer;
  buffer << input.rdbuf();
  return parse_manifest_text(buffer.str(), path);
}

std::string manifest_to_json(const PackageManifest& manifest) {
  return "{"
"\"name\":\"" + manifest.name + "\","
"\"version\":\"" + manifest.version + "\","
"\"type\":\"" + manifest.type + "\""
"}";
}

} // namespace sattyre
