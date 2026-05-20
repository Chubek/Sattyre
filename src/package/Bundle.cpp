#include "sattyre/Bundle.hpp"
#include "sattyre/Log.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>

#if defined(SATTYRE_HAVE_LIBARCHIVE)
#include <archive.h>
#include <archive_entry.h>
#endif

namespace sattyre {

#if defined(SATTYRE_HAVE_LIBARCHIVE)
namespace {

bool archive_write_file(struct archive* writer, const std::filesystem::path& file_path, const std::filesystem::path& relative_path) {
  std::ifstream in(file_path, std::ios::binary);
  if (!in) {
    return false;
  }

  const auto size = static_cast<la_int64_t>(std::filesystem::file_size(file_path));
  archive_entry* entry = archive_entry_new();
  if (entry == nullptr) {
    return false;
  }
  archive_entry_set_pathname(entry, relative_path.generic_string().c_str());
  archive_entry_set_filetype(entry, AE_IFREG);
  archive_entry_set_perm(entry, 0644);
  archive_entry_set_size(entry, size);

  if (archive_write_header(writer, entry) != ARCHIVE_OK) {
    archive_entry_free(entry);
    return false;
  }

  std::array<char, 16 * 1024> buffer {};
  while (in.good()) {
    in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    const std::streamsize got = in.gcount();
    if (got <= 0) {
      break;
    }
    if (archive_write_data(writer, buffer.data(), static_cast<std::size_t>(got)) < 0) {
      archive_entry_free(entry);
      return false;
    }
  }

  archive_entry_free(entry);
  return true;
}

bool safe_relative_archive_path(const char* path_name, std::filesystem::path& out_relative) {
  if (path_name == nullptr || path_name[0] == '\0') {
    return false;
  }
  const std::filesystem::path candidate(path_name);
  if (candidate.is_absolute()) {
    return false;
  }
  for (const auto& part : candidate) {
    if (part == "..") {
      return false;
    }
  }
  out_relative = candidate.lexically_normal();
  return !out_relative.empty();
}

} // namespace
#endif

bool create_bundle(const std::string& source_dir, const std::string& out_file) {
  log::info("Bundling package from " + source_dir + " -> " + out_file);

  namespace fs = std::filesystem;
  const fs::path source_path(source_dir);
  if (!fs::exists(source_path) || !fs::is_directory(source_path)) {
    return false;
  }

#if defined(SATTYRE_HAVE_LIBARCHIVE)
  struct archive* writer = archive_write_new();
  if (writer == nullptr) {
    return false;
  }

  const bool configured =
    (archive_write_set_format_pax_restricted(writer) == ARCHIVE_OK) &&
    (archive_write_add_filter_gzip(writer) == ARCHIVE_OK) &&
    (archive_write_open_filename(writer, out_file.c_str()) == ARCHIVE_OK);
  if (!configured) {
    archive_write_free(writer);
    return false;
  }

  bool ok = true;
  for (const auto& entry : fs::recursive_directory_iterator(source_path)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const fs::path relative = fs::relative(entry.path(), source_path);
    if (!archive_write_file(writer, entry.path(), relative)) {
      ok = false;
      break;
    }
  }

  archive_write_close(writer);
  archive_write_free(writer);
  return ok;
#else
  std::ofstream out(out_file, std::ios::binary);
  if (!out) {
    return false;
  }

  for (const auto& entry : fs::recursive_directory_iterator(source_path)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const fs::path relative = fs::relative(entry.path(), source_path);
    out << "FILE " << relative.generic_string() << "\n";

    std::ifstream in(entry.path(), std::ios::binary);
    if (!in) {
      return false;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    const std::string content = buffer.str();
    out << content.size() << "\n";
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    out << "\n";
  }
  return true;
#endif
}

bool extract_bundle(const std::string& bundle_file, const std::string& out_dir) {
  log::info("Extracting bundle " + bundle_file + " -> " + out_dir);

  namespace fs = std::filesystem;
#if defined(SATTYRE_HAVE_LIBARCHIVE)
  fs::create_directories(out_dir);
  const fs::path out_root = fs::weakly_canonical(fs::path(out_dir));

  struct archive* reader = archive_read_new();
  if (reader == nullptr) {
    return false;
  }
  archive_read_support_filter_all(reader);
  archive_read_support_format_tar(reader);

  if (archive_read_open_filename(reader, bundle_file.c_str(), 10240) != ARCHIVE_OK) {
    archive_read_free(reader);
    return false;
  }

  bool ok = true;
  archive_entry* entry = nullptr;
  while (archive_read_next_header(reader, &entry) == ARCHIVE_OK) {
    std::filesystem::path relative;
    if (!safe_relative_archive_path(archive_entry_pathname(entry), relative)) {
      ok = false;
      break;
    }

    fs::path destination = out_root / relative;
    destination = destination.lexically_normal();
    fs::path canonical_parent = fs::weakly_canonical(destination.parent_path());
    if (canonical_parent.native().compare(0, out_root.native().size(), out_root.native()) != 0) {
      ok = false;
      break;
    }

    fs::create_directories(destination.parent_path());
    std::ofstream out(destination, std::ios::binary);
    if (!out) {
      ok = false;
      break;
    }

    std::array<char, 16 * 1024> buffer {};
    la_ssize_t read_size = 0;
    while ((read_size = archive_read_data(reader, buffer.data(), buffer.size())) > 0) {
      out.write(buffer.data(), read_size);
      if (!out) {
        ok = false;
        break;
      }
    }
    if (!ok || read_size < 0) {
      ok = false;
      break;
    }
  }

  archive_read_close(reader);
  archive_read_free(reader);
  return ok;
#else
  std::ifstream in(bundle_file, std::ios::binary);
  if (!in) {
    return false;
  }

  fs::create_directories(out_dir);
  const fs::path out_root = fs::weakly_canonical(fs::path(out_dir));

  std::string marker;
  while (std::getline(in, marker)) {
    if (marker.empty()) {
      continue;
    }
    if (marker.rfind("FILE ", 0) != 0) {
      return false;
    }
    const std::string relative = marker.substr(5);
    fs::path destination = out_root / fs::path(relative);
    destination = fs::weakly_canonical(destination);

    if (destination.native().compare(0, out_root.native().size(), out_root.native()) != 0) {
      return false;
    }

    std::string size_line;
    if (!std::getline(in, size_line)) {
      return false;
    }
    std::size_t content_size = 0;
    try {
      content_size = static_cast<std::size_t>(std::stoull(size_line));
    } catch (...) {
      return false;
    }

    std::string content(content_size, '\0');
    in.read(content.data(), static_cast<std::streamsize>(content_size));
    if (in.gcount() != static_cast<std::streamsize>(content_size)) {
      return false;
    }
    if (in.get() == EOF) {
      return false;
    }

    fs::create_directories(destination.parent_path());
    std::ofstream out(destination, std::ios::binary);
    if (!out) {
      return false;
    }
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
  }
  return true;
#endif
}

} // namespace sattyre
