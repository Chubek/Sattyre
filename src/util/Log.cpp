#include "sattyre/Log.hpp"

#if defined(SATTYRE_USE_LOG4CPLUS)
#  include <log4cplus/configurator.h>
#  include <log4cplus/logger.h>
#  include <log4cplus/loggingmacros.h>
#else
#  include <iostream>
#endif

namespace sattyre::log {

#if defined(SATTYRE_USE_LOG4CPLUS)
static log4cplus::Logger logger = log4cplus::Logger::getInstance("sattyre");
#endif

void init() {
#if defined(SATTYRE_USE_LOG4CPLUS)
  log4cplus::BasicConfigurator config;
  config.configure();
#endif
}

void info(const std::string& msg)  {
#if defined(SATTYRE_USE_LOG4CPLUS)
  LOG4CPLUS_INFO(logger, msg);
#else
  std::clog << "[info] " << msg << '\n';
#endif
}
void warn(const std::string& msg)  {
#if defined(SATTYRE_USE_LOG4CPLUS)
  LOG4CPLUS_WARN(logger, msg);
#else
  std::clog << "[warn] " << msg << '\n';
#endif
}
void error(const std::string& msg) {
#if defined(SATTYRE_USE_LOG4CPLUS)
  LOG4CPLUS_ERROR(logger, msg);
#else
  std::cerr << "[error] " << msg << '\n';
#endif
}

} // namespace sattyre::log
