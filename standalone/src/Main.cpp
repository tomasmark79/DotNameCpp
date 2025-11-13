#include "Standalone.hpp"
#include <Logger/LoggerFactory.hpp>
#include <Utils/Utils.hpp>
#include <cxxopts.hpp>
#include <iostream>

// Flow
// Main.cpp ->
// Standalone.hpp -> [Standalone.cpp (All implementations are inline)]
// DotNameLib.hpp -> [DotNameLib.cpp (All implementations are inline)] 

// Namespace structure:
// dotnamecpp::
//   ├── v1::              // Main library
//   ├── app::             // Standalone application  
//   ├── logging::         // Logging subsystem
//   ├── assets::          // Asset management
//   └── utils::
//       ├── fs::          // Filesystem operations
//       ├── string::      // String utilities
//       └── json::        // JSON utilities

std::filesystem::path getStandalonePath () {
  return dotnamecpp::utils::fs::getExecutablePath ();
}

int main (int argc, char** argv) {
  using namespace dotnamecpp::app;
  using namespace dotnamecpp::logging;

  try {

    Standalone app;

    // Parse command-line options
    cxxopts::Options options (app.getAppName (), "DotName C++ Standalone Application");
    options.add_options () ("h,help", "Print usage");
    options.add_options () ("w,write2file", "Write output to file",
        cxxopts::value<bool> ()->default_value ("false"));
    auto result = options.parse (argc, argv);
    if (result.count ("help") > 0) {
      std::cout << options.help () << '\n';
      return EXIT_SUCCESS;
    }

    LoggerConfig loggerConfig{ .level = Level::LOG_INFO,
      .enableFileLogging = result["write2file"].as<bool> (),
      .logFilePath = "standalone.log",
      .colorOutput = true };

    if (!app.initializeComponents (loggerConfig, getStandalonePath ())) {
      std::cerr << "Failed to initialize application\n";
      return EXIT_FAILURE;
    }

    return app.run ();

  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what () << '\n';
    return EXIT_FAILURE;
  }

  //
}