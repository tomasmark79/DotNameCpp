#pragma once
#include "Logger/ILogger.hpp"
#include <memory>
#include <string>

namespace dotnamecpp::logging {

  enum class LoggerType : uint8_t { Console, File, Null };

  struct LoggerConfig {
    Level level = Level::LOG_INFO;
    bool enableFileLogging = false;
    std::string logFilePath;
    bool colorOutput = true;
  };

  class LoggerFactory {
  public:
    static std::shared_ptr<ILogger> create (LoggerType type = LoggerType::Console,
        const LoggerConfig& config = LoggerConfig{});

    static std::shared_ptr<ILogger> createConsole (const LoggerConfig& config = LoggerConfig{});

    static std::shared_ptr<ILogger> createNull ();
  };

} // namespace dotnamecpp::logging