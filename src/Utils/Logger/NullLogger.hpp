#pragma once
#include "ILogger.hpp"

namespace dotnamecpp::logging {

  class NullLogger : public ILogger {
  public:
    void debug (const std::string& /*message*/, const std::string& /*caller*/ = "") override {
    }

    void info (const std::string& /*message*/, const std::string& /*caller*/ = "") override {
    }

    void warning (const std::string& /*message*/, const std::string& /*caller*/ = "") override {
    }

    void error (const std::string& /*message*/, const std::string& /*caller*/ = "") override {
    }

    void critical (const std::string& /*message*/, const std::string& /*caller*/ = "") override {
    }

    void setLevel (Level /*level*/) override {
    }
    Level getLevel () const override {
      return Level::LOG_INFO;
    }
    void setAppPrefix (const std::string& /*prefix*/) override {
    }

    std::string getAppPrefix () const override {
      return "";
    }

    bool enableFileLogging (const std::string& /*filename*/) override {
      return true;
    }

    void disableFileLogging () override {
    }
  };

} // namespace dotnamecpp::logging