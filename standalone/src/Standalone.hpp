#pragma once
#include "Logger/ILogger.hpp"
#include "Logger/LoggerFactory.hpp"
#include "DotNameLib/DotNameLib.hpp"
#include "Assets/IAssetManager.hpp"
#include <memory>
#include <string>

namespace dotnamecpp::app {

  class Standalone {
  private:
    std::shared_ptr<logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::IAssetManager> assetManager_;
    std::unique_ptr<v1::DotNameLib> library_;
    std::string appName_;
    bool initialized_ = false;

    bool initializeAssets (const std::filesystem::path& executablePath, const std::string& appName);
    void initializeLogger (const logging::LoggerConfig& loggerConfig);
    bool initializeLibrary ();

  public:
    explicit Standalone (const std::string& appName);
    ~Standalone ();

    // Non-copyable
    Standalone (const Standalone&) = delete;
    Standalone& operator= (const Standalone&) = delete;

    // Public initialization methods
    bool initialize (const logging::LoggerConfig& loggerConfig,
        const std::filesystem::path& executablePath);

    int run ();

    logging::ILogger& getLogger () {
      return *logger_;
    }
    const std::string& getAppName () const {
      return appName_;
    }
  };

} // namespace dotnamecpp::app