#pragma once
#include "Logger/ILogger.hpp"
#include <memory>
#include <string>

namespace dotnamecpp::logging {

  enum class LoggerType : uint8_t { Console, File, Null };

  /**
   * @brief Configuration options for creating a logger
   */
  struct LoggerConfig {
    Level level = Level::LOG_INFO;
    bool enableFileLogging = false;
    std::string logFilePath;
    bool colorOutput = true;
  };

  class LoggerFactory {
  public:
    /**
     * @brief Create a logger instance based on the specified type and configuration
     * @param type The type of logger to create (Console, File, Null)
     * @param config Configuration options for the logger
     * @return std::shared_ptr<ILogger> A shared pointer to the created logger instance
     */
    static std::shared_ptr<ILogger> create (LoggerType type = LoggerType::Console,
        const LoggerConfig& config = LoggerConfig{});

    /**
      * @brief Create a Console object logger
      * @param config 
      * @return std::shared_ptr<ILogger> 
      */
    static std::shared_ptr<ILogger> createConsole (const LoggerConfig& config = LoggerConfig{});

    /**
      * @brief Create a File logger
      * @param config 
      * @return std::shared_ptr<ILogger> 
      */
    static std::shared_ptr<ILogger> createFile (const LoggerConfig& config = LoggerConfig{});

    /**
      * @brief Create a Null logger
      * @return std::shared_ptr<ILogger> 
      */
    static std::shared_ptr<ILogger> createNull ();
  };

} // namespace dotnamecpp::logging