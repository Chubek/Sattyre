#pragma once

#include <string>
#include "sattyre/Sattyre-Library.h"

namespace sattyre {

class Library {
public:
  virtual ~Library() = default;
  virtual std::string name() const = 0;
  virtual std::string version() const = 0;
};

bool library_abi_compatible(unsigned int abi_version);
bool library_abi_compatible(const SattyreLibraryInfo& info);

} // namespace sattyre
