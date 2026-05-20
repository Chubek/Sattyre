#pragma once

#include <string>
#include "sattyre/Sattyre-Plugin.h"

namespace sattyre {

class Plugin {
public:
  virtual ~Plugin() = default;
  virtual std::string name() const = 0;
  virtual std::string version() const = 0;
  virtual bool init() = 0;
  virtual void shutdown() = 0;
};

bool plugin_abi_compatible(unsigned int abi_version);
bool plugin_abi_compatible(const SattyrePluginInfo& info);

} // namespace sattyre
