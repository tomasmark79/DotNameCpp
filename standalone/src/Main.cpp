#include "Standalone.hpp"
#include "Utils/Utils.hpp"

// TODO: OOP fix
namespace AppContext {
  constexpr const char* standaloneName = "DotNameStandalone";

  std::filesystem::path getStandalonePath () {
    return DotNameUtils::fs::path::getStandalonePath ();
  }

  std::filesystem::path getAssetsPath () {
    return AssetContext::findAssetsPath (getStandalonePath (), standaloneName);
  }
}

int main (int argc, const char* argv[]) {
  using namespace dotnamecpp;

  logging::LoggerConfig logConfig{ .level = logging::Level::LOG_INFO,
    .enableFileLogging = false,
    .logFilePath = "",
    .colorOutput = true };

  app::StandaloneConfig config{ .appName = AppContext::standaloneName,
    .assetsPath = AppContext::getAssetsPath (),
    .loggerConfig = logConfig,
    .omitLibraryLoading = false };

  // Vytvoření a spuštění aplikace
  app::Standalone app (config);
  return app.run (argc, argv);
}