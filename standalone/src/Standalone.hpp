#pragma once
#include "Assets/IAssetManager.hpp"
#include "Logger/ILogger.hpp"
#include "Logger/LoggerFactory.hpp"
#include "DotNameLib/DotNameLib.hpp"
#include <memory>
#include <string>

namespace dotnamecpp::app {
  class Standalone {

  private:
    bool initialized_ = false;
    std::string appName_ = "DotNameStandalone";
    std::shared_ptr<logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::IAssetManager> assetManager_;
    std::unique_ptr<dotnamecpp::v1::DotNameLib> library_;

    bool initializeLogger (const logging::LoggerConfig& loggerConfig) {
      logger_ = logging::LoggerFactory::create (logging::LoggerType::Console, loggerConfig);
      return logger_ != nullptr;
    }

    bool initializeAssets (const std::filesystem::path& executablePath) {
      try {
        assetManager_ = dotnamecpp::AssetManagerFactory::createDefault (executablePath, appName_);
        if (!assetManager_->validate ()) {
          logger_->errorStream () << "Failed to validate assets path: "
                                  << assetManager_->getAssetsPath ();
          return false;
        }
        logger_->infoStream () << "Assets initialized: " << assetManager_->getAssetsPath ();
        return true;
      } catch (const std::exception& e) {
        logger_->errorStream () << "Asset initialization error: " << e.what ();
        return false;
      }
    }

    bool initializeLibrary () {
      try {
        if (!assetManager_) {
          logger_->errorStream () << "Asset manager not initialized";
          return false;
        }
        library_ = std::make_unique<dotnamecpp::v1::DotNameLib> (logger_, assetManager_);
        logger_->infoStream () << "Library initialized successfully";
        initialized_ = true;
        return true;
      } catch (const std::exception& e) {
        logger_->errorStream () << "Failed to initialize library: " << e.what ();
        initialized_ = false;
        return false;
      }
    }

  public:
    explicit Standalone () = default;
    ~Standalone () {
      if (logger_) {
        logger_->infoStream () << appName_ << " shutting down";
      }
    }

    Standalone (const Standalone&) = delete;
    Standalone& operator= (const Standalone&) = delete;

    logging::ILogger& getLogger () {
      return *logger_;
    }

    [[nodiscard]]
    const std::string& getAppName () const {
      return appName_;
    }

    // Initialize all components (1. logger, 2. assets, 3. library)
    bool initializeComponents (const logging::LoggerConfig& loggerConfig,
        const std::filesystem::path& executablePath) {
      if (!initializeLogger (loggerConfig)) {
        return false;
      }
      if (!initializeAssets (executablePath)) {
        return false;
      }
      if (!initializeLibrary ()) {
        return false;
      }
      initialized_ = true;
      return true;
    }

    int run () {
      if (!initialized_) {
        if (logger_) {
          logger_->errorStream () << "Application not initialized properly. Exiting.";
        }
        return EXIT_FAILURE;
      }

      if (!library_) {
        logger_->errorStream () << "Library not initialized";
        return EXIT_FAILURE;
      }

      logger_->warningStream () << "... warning example ...";
      logger_->errorStream () << "... error example ...";
      logger_->infoStream () << appName_ << " running...";
      logger_->infoStream () << ".";

      return EXIT_SUCCESS;
    }
  };

} // namespace dotnamecpp::app