#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>
#include <cstdint>
#include <sstream>
#include "fmt/core.h"

namespace dotnamecpp::logging {

  enum class Level : std::uint8_t { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL };

  // Forward declaration
  class ILogger;

  // LogStream helper class - can be used by any ILogger implementation
  class LogStream {
  public:
    LogStream (ILogger& logger, Level level, std::string caller)
        : logger_ (logger), level_ (level), caller_ (std::move (caller)) {
    }

    // Destructor declared here, defined after ILogger
    ~LogStream ();

    template <typename T>
    LogStream& operator<< (const T& value) {
      oss_ << value;
      return *this;
    }
    // Overload also for manipulators like std::endl
    LogStream& operator<< (std::ostream& (*manip) (std::ostream&)) {
      oss_ << manip;
      return *this;
    }

  private:
    ILogger& logger_;
    Level level_;
    std::string caller_;
    std::ostringstream oss_;
  };

  class ILogger {
  public:
    ILogger () = default;
    virtual ~ILogger () = default;

    virtual void debug (const std::string& message, const std::string& caller = "") = 0;
    virtual void info (const std::string& message, const std::string& caller = "") = 0;
    virtual void warning (const std::string& message, const std::string& caller = "") = 0;
    virtual void error (const std::string& message, const std::string& caller = "") = 0;
    virtual void critical (const std::string& message, const std::string& caller = "") = 0;

    virtual void setLevel (Level level) = 0;
    [[nodiscard]]
    virtual Level getLevel () const
        = 0;

    virtual bool enableFileLogging (const std::string& filename) = 0;
    virtual void disableFileLogging () = 0;

    // Non-virtual convenience methods - available through interface
    LogStream stream (Level level, const std::string& caller = "") {
      return LogStream{ *this, level, caller };
    }

    LogStream debugStream (const std::string& caller = "") {
      return stream (Level::LOG_DEBUG, caller);
    }
    LogStream infoStream (const std::string& caller = "") {
      return stream (Level::LOG_INFO, caller);
    }
    LogStream warningStream (const std::string& caller = "") {
      return stream (Level::LOG_WARNING, caller);
    }
    LogStream errorStream (const std::string& caller = "") {
      return stream (Level::LOG_ERROR, caller);
    }
    LogStream criticalStream (const std::string& caller = "") {
      return stream (Level::LOG_CRITICAL, caller);
    }

    // Fmt convenience methods (non-virtual templates)
    template <typename... Args>
    void debugFmt (const std::string& format, Args&&... args);
    template <typename... Args>
    void infoFmt (const std::string& format, Args&&... args);
    template <typename... Args>
    void warningFmt (const std::string& format, Args&&... args);
    template <typename... Args>
    void errorFmt (const std::string& format, Args&&... args);
    template <typename... Args>
    void criticalFmt (const std::string& format, Args&&... args);
  };

  // LogStream destructor implementation - defined after ILogger
  inline LogStream::~LogStream () {
    const std::string message = oss_.str ();
    switch (level_) {
    case Level::LOG_DEBUG:
      logger_.debug (message, caller_);
      break;
    case Level::LOG_INFO:
      logger_.info (message, caller_);
      break;
    case Level::LOG_WARNING:
      logger_.warning (message, caller_);
      break;
    case Level::LOG_ERROR:
      logger_.error (message, caller_);
      break;
    case Level::LOG_CRITICAL:
      logger_.critical (message, caller_);
      break;
    }
  }

  // Fmt convenience method implementations
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