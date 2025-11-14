#include "LoggerFactory.hpp"
#include "ConsoleLogger.hpp"
#include "NullLogger.hpp"

namespace dotnamecpp::logging {

  std::shared_ptr<ILogger> LoggerFactory::create (LoggerType type, const LoggerConfig& config) {

    switch (type) {
    case LoggerType::Console:
      return createConsole (config);
    case LoggerType::Null:
      return createNull ();
    default:
      return createConsole (config);
    }
  }

  std::shared_ptr<ILogger> LoggerFactory::createConsole (const LoggerConfig& config) {
    auto logger = std::make_shared<ConsoleLogger> ();
    logger->setLevel (config.level);

    if (config.enableFileLogging && !config.logFilePath.empty ()) {
      logger->enableFileLogging (config.logFilePath);
    }

    return logger;
  }

  std::shared_ptr<ILogger> LoggerFactory::createNull () {
    return std::make_shared<NullLogger> ();
  }

} // namespace dotnamecpp::logging