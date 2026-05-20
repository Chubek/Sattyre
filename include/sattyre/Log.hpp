#pragma once

#include <string>

namespace sattyre::log {

void init();
void info(const std::string& msg);
void warn(const std::string& msg);
void error(const std::string& msg);

} // namespace sattyre::log
