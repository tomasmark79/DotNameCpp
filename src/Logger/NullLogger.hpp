#pragma once
#include "ILogger.hpp"

namespace dotnamecpp::logging {

  class NullLogger : public ILogger {
  public:
    void debug (const std::string&, const std::string& = "") override {
    }
    void info (const std::string&, const std::string& = "") override {
    }
    void warning (const std::string&, const std::string& = "") override {
    }
    void error (const std::string&, const std::string& = "") override {
    }
    void critical (const std::string&, const std::string& = "") override {
    }

    void setLevel (Level) override {
    }
    Level getLevel () const override {
      return Level::LOG_INFO;
    }

    bool enableFileLogging (const std::string&) override {
      return true;
    }
    void disableFileLogging () override {
    }
  };

} // namespace dotnamecpp::logging