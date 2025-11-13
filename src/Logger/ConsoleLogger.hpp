#ifndef CONSOLELOGGER_HPP
#define CONSOLELOGGER_HPP

#include "ILogger.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>

#include "fmt/core.h"

#ifdef _WIN32
  #include "../WindowsHeaders.hpp"
#endif

class ConsoleLogger : public dotnamecpp::logging::ILogger {
private:
  std::mutex logMutex_;
  std::ofstream logFile_;
  bool addNewLine_ = true;
#ifdef DEBUG
  dotnamecpp::logging::Level currentLevel_ = dotnamecpp::logging::Level::LOG_DEBUG;
#else
  dotnamecpp::logging::Level currentLevel_ = dotnamecpp::logging::Level::LOG_INFO;
#endif

public:
  ConsoleLogger () = default;
  ~ConsoleLogger () {
    std::lock_guard<std::mutex> lock (logMutex_);
    if (logFile_.is_open ()) {
      logFile_.close ();
    }
  }

  // Non-copyable, movable
  ConsoleLogger (const ConsoleLogger&) = delete;
  ConsoleLogger& operator= (const ConsoleLogger&) = delete;
  ConsoleLogger (ConsoleLogger&& other) noexcept : logFile_ (std::move (other.logFile_)),
                                                   addNewLine_ (other.addNewLine_) {
  }
  ConsoleLogger& operator= (ConsoleLogger&& other) noexcept {
    if (this != &other) {
      std::lock_guard<std::mutex> lock1 (logMutex_);
      std::lock_guard<std::mutex> lock2 (other.logMutex_);
      logFile_ = std::move (other.logFile_);
      addNewLine_ = other.addNewLine_;
    }
    return *this;
  }

  void debug (const std::string& message, const std::string& caller = "") override {
    log (dotnamecpp::logging::Level::LOG_DEBUG, message, caller);
  };
  void info (const std::string& message, const std::string& caller = "") override {
    log (dotnamecpp::logging::Level::LOG_INFO, message, caller);
  };
  void warning (const std::string& message, const std::string& caller = "") override {
    log (dotnamecpp::logging::Level::LOG_WARNING, message, caller);
  };
  void error (const std::string& message, const std::string& caller = "") override {
    log (dotnamecpp::logging::Level::LOG_ERROR, message, caller);
  };
  void critical (const std::string& message, const std::string& caller = "") override {
    log (dotnamecpp::logging::Level::LOG_CRITICAL, message, caller);
  };
  void log (dotnamecpp::logging::Level level, const std::string& message,
      const std::string& caller) {
    // thread-safety
    std::lock_guard<std::mutex> lock (logMutex_);

    // Get current time
    auto now = std::chrono::system_clock::now ();
    std::time_t now_c = std::chrono::system_clock::to_time_t (now);
    std::tm now_tm;
#ifdef _WIN32
    localtime_s (&now_tm, &now_c);
#else
    localtime_r (&now_c, &now_tm);
#endif

    // Create log header
    std::ostringstream header;
    header << "[" << std::put_time (&now_tm, "%Y-%m-%d %H:%M:%S") << "]";
    header << "[" << levelToString (level) << "]";
    if (!caller.empty ()) {
      header << "[" << caller << "]";
    }
    header << " ";

    // Log to console
    setConsoleColor (level);
    std::cout << header.str () << message;
    if (addNewLine_) {
      std::cout << "\n";
    }
    resetConsoleColor ();

    // Log to file if enabled
    if (logFile_.is_open ()) {
      logFile_ << header.str () << message;
      if (addNewLine_) {
        logFile_ << "\n";
      }
    }
  };

  // Fmt methods inherited from ILogger - no need to redefine

  void setLevel (dotnamecpp::logging::Level level) override {
    std::lock_guard<std::mutex> lock (logMutex_);
    currentLevel_ = level;
  };
  [[nodiscard]] dotnamecpp::logging::Level getLevel () const override {
    return currentLevel_;
  };

  bool enableFileLogging (const std::string& filename) override {
    std::lock_guard<std::mutex> lock (logMutex_);
    try {
      logFile_.open (filename, std::ios::out | std::ios::app);
      return logFile_.is_open ();
    } catch (const std::ios_base::failure& e) {
      std::cerr << "Failed to open log file: " << filename << " - " << e.what () << "\n";
      return false;
    } catch (const std::exception& e) {
      std::cerr << "Failed to open log file: " << filename << " - " << e.what () << "\n";
      return false;
    } catch (...) {
      std::cerr << "Failed to open log file: " << filename << "\n";
      return false;
    }
  };

  void disableFileLogging () override {
    std::lock_guard<std::mutex> lock (logMutex_);
    if (logFile_.is_open ()) {
      logFile_.close ();
    }
  };

  static std::string levelToString (dotnamecpp::logging::Level level) {
    switch (level) {
    case dotnamecpp::logging::Level::LOG_DEBUG:
      return "DBG";
    case dotnamecpp::logging::Level::LOG_INFO:
      return "INF";
    case dotnamecpp::logging::Level::LOG_WARNING:
      return "WRN";
    case dotnamecpp::logging::Level::LOG_ERROR:
      return "ERR";
    case dotnamecpp::logging::Level::LOG_CRITICAL:
      return "CRI";
    default:
      return "INF";
    }
  }

  static void resetConsoleColor () {
#ifdef _WIN32
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#elif defined(__EMSCRIPTEN__)
// no colors, no reset
#else
    std::cout << "\033[0m";
#endif
  }

#ifdef _WIN32
  static void setConsoleColorWindows (dotnamecpp::logging::Level level) {
    const std::map<dotnamecpp::logging::Level, WORD> colorMap
        = { { dotnamecpp::logging::Level::LOG_DEBUG,
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY },
            { dotnamecpp::logging::Level::LOG_INFO, FOREGROUND_GREEN | FOREGROUND_INTENSITY },
            { dotnamecpp::logging::Level::LOG_WARNING,
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY },
            { dotnamecpp::logging::Level::LOG_ERROR, FOREGROUND_RED | FOREGROUND_INTENSITY },
            { dotnamecpp::logging::Level::LOG_CRITICAL,
              FOREGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_BLUE } };
    auto it = colorMap.find (level);
    if (it != colorMap.end ()) {
      SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), it->second);
    } else {
      resetConsoleColor ();
    }
  }
#else
  static void setConsoleColorUnix (dotnamecpp::logging::Level level) {
    static const std::map<dotnamecpp::logging::Level, const char*> colorMap
        = { { dotnamecpp::logging::Level::LOG_DEBUG, "\033[34m" },
            { dotnamecpp::logging::Level::LOG_INFO, "\033[32m" },
            { dotnamecpp::logging::Level::LOG_WARNING, "\033[33m" },
            { dotnamecpp::logging::Level::LOG_ERROR, "\033[31m" },
            { dotnamecpp::logging::Level::LOG_CRITICAL, "\033[95m" } };
    auto it = colorMap.find (level);
    if (it != colorMap.end ()) {
      std::cout << it->second;
    } else {
      resetConsoleColor ();
    }
  }
#endif

  static void setConsoleColor (dotnamecpp::logging::Level level) {
#ifdef _WIN32
    setConsoleColorWindows (level);
#elif EMSCRIPTEN
      // no colors
#else
    setConsoleColorUnix (level);
#endif
  }

private:
  std::string headerName_ = "DotNameLib";
  bool includeName_ = true;
  bool includeTime_ = true;
  bool includeCaller_ = true;
  bool includeLevel_ = true;

  void logToStream (std::ostream& stream, dotnamecpp::logging::Level level,
      const std::string& message, const std::string& caller, const std::tm& now_tm) {
    setConsoleColor (level);
    stream << buildHeader (now_tm, caller, level) << message;
    resetConsoleColor ();
    if (addNewLine_) {
      stream << "\n";
    }
    // Explicitly flush the stream to ensure immediate output
    stream.flush ();
  }

  std::string buildHeader (const std::tm& now_tm, const std::string& caller,
      dotnamecpp::logging::Level level) const {
    std::ostringstream header;
    if (includeName_) {
      header << "[" << headerName_ << "] ";
    }
    if (includeTime_) {
      header << "[" << std::put_time (&now_tm, "%d-%m-%Y %H:%M:%S") << "] ";
    }
    if (includeCaller_ && !caller.empty ()) {
      header << "[" << caller << "] ";
    }
    if (includeLevel_) {
      header << "[" << levelToString (level) << "] ";
    }
    return header.str ();
  }

public:
  void setHeaderName (const std::string& headerName) {
    std::lock_guard<std::mutex> lock (logMutex_);
    headerName_ = headerName;
  }
  void showHeaderName (bool includeName) {
    std::lock_guard<std::mutex> lock (logMutex_);
    includeName_ = includeName;
  }
  void showHeaderTime (bool includeTime) {
    std::lock_guard<std::mutex> lock (logMutex_);
    includeTime_ = includeTime;
  }
  void showHeaderCaller (bool includeCaller) {
    std::lock_guard<std::mutex> lock (logMutex_);
    includeCaller_ = includeCaller;
  }
  void showHeaderLevel (bool includeLevel) {
    std::lock_guard<std::mutex> lock (logMutex_);
    includeLevel_ = includeLevel;
  }
  void noHeader (bool noHeader) {
    if (noHeader) {
      showHeaderName (false);
      showHeaderTime (false);
      showHeaderCaller (false);
      showHeaderLevel (false);
    } else {
      showHeaderName (true);
      showHeaderTime (true);
      showHeaderCaller (true);
      showHeaderLevel (true);
    }
  }
  void visibleHeaders (bool incName, bool incTime, bool incCaller, bool incLevel) {
    showHeaderName (incName);
    showHeaderTime (incTime);
    showHeaderCaller (incCaller);
    showHeaderLevel (incLevel);
  }

  // Stream and fmt methods inherited from ILogger
  // No need to redefine - they're already available!

}; // class ConsoleLogger

// MIT License Copyright (c) 2024-2025 Tomáš Mark

#endif