#pragma once
#include "Logger/ILogger.hpp"
#include "Logger/LoggerFactory.hpp"
#include "DotNameLib/DotNameLib.hpp"
#include <memory>
#include <string>

namespace dotnamecpp::app {

  struct StandaloneConfig {
    std::string appName;
    std::filesystem::path assetsPath;
    logging::LoggerConfig loggerConfig;
    bool omitLibraryLoading = false;
  };

  class Standalone {
  private:
    std::shared_ptr<logging::ILogger> logger_;
    std::unique_ptr<v1::DotNameLib> library_;
    StandaloneConfig config_;
    bool initialized_ = false;

    void initializeLogger ();
    void initializeLibrary ();

  public:
    explicit Standalone (const StandaloneConfig& config);
    ~Standalone ();

    // Non-copyable
    Standalone (const Standalone&) = delete;
    Standalone& operator= (const Standalone&) = delete;

    int run (int argc, const char* argv[]);

    // Getters pro testování
    logging::ILogger& getLogger () {
      return *logger_;
    }
    const StandaloneConfig& getConfig () const {
      return config_;
    }
  };

} // namespace dotnamecpp::app