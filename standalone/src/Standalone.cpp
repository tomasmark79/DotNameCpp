// Standalone.cpp
#include "Standalone.hpp"
#include "Logger/LoggerFactory.hpp"
#include <cxxopts.hpp>

namespace dotnamecpp::app {

  Standalone::Standalone (const StandaloneConfig& config) : config_ (config) {
    initializeLogger ();
    if (!config_.omitLibraryLoading) {
      initializeLibrary ();
    }
  }

  void Standalone::initializeLogger () {
    logger_ = logging::LoggerFactory::create (logging::LoggerType::Console, config_.loggerConfig);

    logger_->infoStream () << config_.appName << " logger initialized";
  }

  void Standalone::initializeLibrary () {
    try {
      library_ = std::make_unique<v1::DotNameLib> (logger_, config_.assetsPath);
      logger_->infoStream () << "Library initialized successfully";
      initialized_ = true;
    } catch (const std::exception& e) {
      logger_->errorStream () << "Failed to initialize library: " << e.what ();
      initialized_ = false;
    }
  }

  Standalone::~Standalone () {
    logger_->infoStream () << config_.appName << " shutting down";
  }

  int Standalone::run ([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
    if (!initialized_ && !config_.omitLibraryLoading) {
      logger_->errorStream () << "Application not initialized properly. Exiting.";
      return EXIT_FAILURE;
    }

    // Argument parsing logic zde
    logger_->infoStream () << config_.appName << " running...";

    // Vaše business logic

    return 0;
  }

} // namespace dotnamecpp::app