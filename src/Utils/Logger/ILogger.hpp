#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>
#include <cstdint>
#include <sstream>
#include <memory>
#include "fmt/core.h"

namespace dotnamecpp::logging {

  /**
   * @brief Logging levels 
   * 
   */
  enum class Level : std::uint8_t { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL };

  /**
   * @brief Interface for a logger
   */
  class ILogger;

  /**
   * @brief Helper class for streaming log messages
   */
  class LogStream {
  public:
    LogStream (std::shared_ptr<ILogger> logger, Level level, std::string caller)
      : logger_ (std::move (logger))
      , level_ (level)
      , caller_ (std::move (caller)) {
    }

    ~LogStream ();

    /**
     * @brief Stream a value into the log message
     * 
     * @tparam T 
     * @param value 
     * @return LogStream& 
     */
    template <typename T>
    LogStream& operator<< (const T& value) {
      oss_ << value;
      return *this;
    }

    /**
     * @brief Stream a manipulator into the log message (e.g., std::endl)
     * 
     * @param manip 
     * @return LogStream& 
     */
    LogStream& operator<< (std::ostream& (*manip) (std::ostream&)) {
      oss_ << manip;
      return *this;
    }

  private:
    std::shared_ptr<ILogger> logger_;
    Level level_;
    std::string caller_;
    std::ostringstream oss_;
  };

  class ILogger : public std::enable_shared_from_this<ILogger> {
  public:
    ILogger () = default;
    virtual ~ILogger () = default;

    /**
     * @brief Log a debug message
     * 
     * @param message 
     * @param caller 
     */
    virtual void debug (const std::string& message, const std::string& caller = "") = 0;

    /**
     * @brief Log an info message
     * 
     * @param message 
     * @param caller 
     */
    virtual void info (const std::string& message, const std::string& caller = "") = 0;

    /**
     * @brief Log a warning message
     * 
     * @param message 
     * @param caller 
     */
    virtual void warning (const std::string& message, const std::string& caller = "") = 0;

    /**
     * @brief Log an error message
     * 
     * @param message 
     * @param caller 
     */
    virtual void error (const std::string& message, const std::string& caller = "") = 0;

    /**
     * @brief Log a critical message
     * 
     * @param message 
     * @param caller 
     */
    virtual void critical (const std::string& message, const std::string& caller = "") = 0;

    /**
     * @brief Set the Level object  
     * 
     * @param level 
     */
    virtual void setLevel (Level level) = 0;

    /**
     * @brief Get the Level object  
     * 
     * @return Level 
     */
    [[nodiscard]]
    virtual Level getLevel () const = 0;

    /**
     * @brief Set the application prefix for log messages
     * 
     * @param prefix 
     */
    virtual void setAppPrefix (const std::string& prefix) = 0;

    /**
     * @brief Get the App Prefix object 
     * 
     * @return std::string 
     */
    virtual std::string getAppPrefix () const = 0;

    /**
     * @brief Enable logging to a file
     * 
     * @param filename 
     * @return true 
     * @return false 
     */
    virtual bool enableFileLogging (const std::string& filename) = 0;

    /**
     * @brief Disable logging to a file
     * 
     */
    virtual void disableFileLogging () = 0;

    /**
     * @brief Create a LogStream for streaming log messages
     * 
     * @param level 
     * @param caller 
     * @return LogStream 
     */
    LogStream stream (Level level, const std::string& caller = "") {
      return LogStream{ shared_from_this (), level, caller };
    }

    // Convenience methods for each log level

    /**
     * @brief Create a LogStream for streaming debug messages
     * 
     * @param caller 
     * @return LogStream 
     */
    LogStream debugStream (const std::string& caller = "") {
      return stream (Level::LOG_DEBUG, caller);
    }

    /**
     * @brief Create a LogStream for streaming info messages
     * 
     * @param caller 
     * @return LogStream 
     */
    LogStream infoStream (const std::string& caller = "") {
      return stream (Level::LOG_INFO, caller);
    }

    /**
     * @brief Create a LogStream for streaming warning messages
     * 
     * @param caller 
     * @return LogStream 
     */
    LogStream warningStream (const std::string& caller = "") {
      return stream (Level::LOG_WARNING, caller);
    }

    /**
     * @brief Create a LogStream for streaming error messages
     * 
     * @param caller 
     * @return LogStream 
     */
    LogStream errorStream (const std::string& caller = "") {
      return stream (Level::LOG_ERROR, caller);
    }

    /**
     * @brief Create a LogStream for streaming critical messages
     * 
     * @param caller 
     * @return LogStream 
     */
    LogStream criticalStream (const std::string& caller = "") {
      return stream (Level::LOG_CRITICAL, caller);
    }

    // Fmt convenience methods (non-virtual templates)
    /**
     * @brief Create a formatted debug message
     * 
     * @tparam Args 
     * @param format 
     * @param args 
     */
    template <typename... Args>
    void debugFmt (const std::string& format, Args&&... args);

    /**
     * @brief Create a formatted info message
     * 
     * @tparam Args 
     * @param format 
     * @param args 
     */
    template <typename... Args>
    void infoFmt (const std::string& format, Args&&... args);

    /**
     * @brief Create a formatted warning message
     * 
     * @tparam Args 
     * @param format 
     * @param args 
     */
    template <typename... Args>
    void warningFmt (const std::string& format, Args&&... args);

    /**
     * @brief Create a formatted error message
     * 
     * @tparam Args 
     * @param format 
     * @param args 
     */
    template <typename... Args>
    void errorFmt (const std::string& format, Args&&... args);

    /**
     * @brief Create a formatted critical message
     * 
     * @tparam Args 
     * @param format 
     * @param args 
     */
    template <typename... Args>
    void criticalFmt (const std::string& format, Args&&... args);
  };

  /**
   * @brief Destroy the Log Stream:: Log Stream object  
   * 
   */
  inline LogStream::~LogStream () {
    const std::string message = oss_.str ();
    switch (level_) {
    case Level::LOG_DEBUG:
      logger_->debug (message, caller_);
      break;
    case Level::LOG_INFO:
      logger_->info (message, caller_);
      break;
    case Level::LOG_WARNING:
      logger_->warning (message, caller_);
      break;
    case Level::LOG_ERROR:
      logger_->error (message, caller_);
      break;
    case Level::LOG_CRITICAL:
      logger_->critical (message, caller_);
      break;
    }
  }

  // Implementations of the fmt convenience methods
  template <typename... Args>
  inline void ILogger::debugFmt (const std::string& format, Args&&... args) {
    std::string message = fmt::vformat (format, fmt::make_format_args (args...));
    debug (message, "");
  }

  template <typename... Args>
  inline void ILogger::infoFmt (const std::string& format, Args&&... args) {
    std::string message = fmt::vformat (format, fmt::make_format_args (args...));
    info (message, "");
  }

  template <typename... Args>
  inline void ILogger::warningFmt (const std::string& format, Args&&... args) {
    std::string message = fmt::vformat (format, fmt::make_format_args (args...));
    warning (message, "");
  }

  template <typename... Args>
  inline void ILogger::errorFmt (const std::string& format, Args&&... args) {
    std::string message = fmt::vformat (format, fmt::make_format_args (args...));
    error (message, "");
  }

  template <typename... Args>
  inline void ILogger::criticalFmt (const std::string& format, Args&&... args) {
    std::string message = fmt::vformat (format, fmt::make_format_args (args...));
    critical (message, "");
  }

} // namespace dotnamecpp::logging

#endif