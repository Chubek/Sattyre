#pragma once

#include <string>

namespace sattyre {

class DynamicLibrary {
public:
  DynamicLibrary();
  ~DynamicLibrary();

  bool open(const std::string& path);
  void close();
  void* symbol(const std::string& name) const;
  bool loaded() const;

private:
  struct Impl;
  Impl* impl_;
};

} // namespace sattyre
