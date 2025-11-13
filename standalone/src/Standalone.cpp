// Standalone.cpp
#include "Standalone.hpp"
#include <Assets/AssetManagerFactory.hpp>
#include <Logger/LoggerFactory.hpp>
#include <cstdlib>

namespace dotnamecpp::app {

  Standalone::Standalone (const std::string& appName) : appName_ (appName) {
  }

  bool Standalone::initializeAssets (const std::filesystem::path& executablePath,
      const std::string& appName) {
    try {
      assetManager_ = dotnamecpp::AssetManagerFactory::createDefault (executablePath, appName);

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

  void Standalone::initializeLogger (const logging::LoggerConfig& loggerConfig) {
    logger_ = logging::LoggerFactory::create (logging::LoggerType::Console, loggerConfig);
    logger_->infoStream () << appName_ << " logger initialized";
  }

  bool Standalone::initializeLibrary () {
    try {
      if (!assetManager_) {
        logger_->errorStream () << "Asset manager not initialized";
        return false;
      }
      library_ = std::make_unique<v1::DotNameLib> (logger_, assetManager_);
      logger_->infoStream () << "Library initialized successfully";
      initialized_ = true;
      return true;
    } catch (const std::exception& e) {
      logger_->errorStream () << "Failed to initialize library: " << e.what ();
      initialized_ = false;
      return false;
    }
  }

  bool Standalone::initialize (const logging::LoggerConfig& loggerConfig,
      const std::filesystem::path& executablePath) {
    initializeLogger (loggerConfig);

    if (!initializeAssets (executablePath, appName_)) {
      return false;
    }

    if (!initializeLibrary ()) {
      return false;
    }

    initialized_ = true;
    return true;
  }

  Standalone::~Standalone () {
    if (logger_) {
      logger_->infoStream () << appName_ << " shutting down";
    }
  }

  int Standalone::run () {
    if (!initialized_) {
      if (logger_) {
        logger_->errorStream () << "Application not initialized properly. Exiting.";
      }
      return EXIT_FAILURE;
    }

    if (!library_) {
      logger_->errorStream () << "Library not initialized";
      return 1;
    }

    logger_->infoStream () << appName_ << " running...";

    return 0;
  }

} // namespace dotnamecpp::app