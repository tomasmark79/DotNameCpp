#pragma once
#include "DotNameLib/DotNameLib.hpp"
#include <Logger/ILogger.hpp>
#include <Logger/LoggerFactory.hpp>
#include <Assets/IAssetManager.hpp>
#include <Assets/AssetManagerFactory.hpp>
#include <memory>
#include <string>
#include <filesystem>

namespace dotnamecpp::app {
  class Standalone {

  private:
    bool initialized_ = false;
    std::string appName_ = "DotNameStandalone";
    std::shared_ptr<logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
    std::unique_ptr<dotnamecpp::v1::DotNameLib> library_;

    bool initializeLogger (const logging::LoggerConfig& loggerConfig) {
      logger_ = logging::LoggerFactory::create (logging::LoggerType::Console, loggerConfig);
      return logger_ != nullptr;
    }

    bool initializeAssets (const std::filesystem::path& executablePath) {
      try {
        assetManager_ =
            dotnamecpp::assets::AssetManagerFactory::createDefault (executablePath, appName_);
        if (!assetManager_->validate ()) {
          logger_->errorStream () << "Failed to validate assets path: "
                                  << assetManager_->getAssetsPath ();
          return initialized_ = false;
        }
        logger_->infoStream () << "Assets initialized: " << assetManager_->getAssetsPath ();
        return initialized_ = true;
      } catch (const std::exception& e) {
        logger_->errorStream () << "Asset initialization error: " << e.what ();
        return initialized_ = false;
      }
    }

    bool initializeLibrary () {
      try {
        library_ = std::make_unique<dotnamecpp::v1::DotNameLib> (logger_, assetManager_);
        logger_->infoStream () << "Library initialized successfully";
        return initialized_ = true;
      } catch (const std::exception& e) {
        logger_->errorStream () << "Failed to initialize library: " << e.what ();
        return initialized_ = false;
      }
    }

  public:
    /**
    * @brief Construct a new Standalone object 
    * 
    */
    explicit Standalone () = default;
    ~Standalone () {
      if (logger_) {
        logger_->infoStream () << appName_ << " shutting down";
      }
    }

    // Delete copy constructor and assignment operator
    Standalone (const Standalone&) = delete;
    Standalone& operator= (const Standalone&) = delete;

    logging::ILogger& getLogger () {
      return *logger_;
    }

    [[nodiscard]]
    const std::string& getAppName () const {
      return appName_;
    }

    /**
     * @brief Initializes all components: logger, assets, and library.
     * 
     * @param loggerConfig Configuration for the logger.
     * @param executablePath Path to the executable, used for asset initialization.
     * @return true if all components are initialized successfully.
     * @return false if any component fails to initialize.
     */
    bool initializeComponents (const logging::LoggerConfig& loggerConfig,
        const std::filesystem::path& executablePath) {
      if (!initializeLogger (loggerConfig)) {
        return initialized_ = false;
      }
      if (!initializeAssets (executablePath)) {
        return initialized_ = false;
      }
      if (!initializeLibrary ()) {
        return initialized_ = false;
      }
      return initialized_ = true;
    }

    /**
     * @brief Runs the application.
     * 
     * @return int EXIT_SUCCESS on success, EXIT_FAILURE on failure.
     */
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