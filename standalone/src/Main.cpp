#include "Standalone.hpp"
#include <Logger/LoggerFactory.hpp>
#include <Utils/Utils.hpp>
#include <cxxopts.hpp>
#include <iostream>

std::filesystem::path getStandalonePath () {
  return DotNameUtils::fs::path::getStandalonePath ();
}

int main (int argc, char** argv) {
  const std::string standaloneName = "DotNameStandalone";

  try {
    // Parse command line
    cxxopts::Options options (standaloneName, "DotName C++ Application");
    options.add_options () ("h,help", "Print usage");
    auto result = options.parse (argc, argv);

    if (result.count ("help") > 0) {
      std::cout << options.help () << '\n';
      return 0;
    }

    // Create standalone instance
    dotnamecpp::app::Standalone app (standaloneName);

    // Initialize logger config
    dotnamecpp::logging::LoggerConfig loggerConfig {
        .level = dotnamecpp::logging::Level::LOG_INFO,
        .enableFileLogging = false,
        .logFilePath = "standalone.log",
        .colorOutput = true};

    // Initialize all components
    auto execPath = getStandalonePath ();
    if (!app.initialize (loggerConfig, execPath)) {
      std::cerr << "Failed to initialize application\n";
      return 1;
    }

    // Run application
    return app.run ();
  }
  catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what () << '\n';
    return 1;
  }
}

// MIT License Copyright (c) 2024-2025 Tomáš Mark