#include "AssetContext.hpp"
#include <vector>

namespace {
  std::filesystem::path g_assetsPath;
}

namespace AssetContext {

  void clearAssetsPath () {
    g_assetsPath.clear ();
  }

  void setAssetsPath (const std::filesystem::path& path) {
    g_assetsPath = path;
  }

  const std::filesystem::path& getAssetsPath () {
    return g_assetsPath;
  }

  std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
                                        const std::string& appName) {
    std::filesystem::path execDir = executablePath.parent_path ();

    // Priority list of asset locations to try
    std::vector<std::filesystem::path> candidatePaths
        = { // 1. Development/debug - assets next to executable
            execDir / "assets",

            // 2. Standard Unix installation - share directory
            execDir / ".." / "share" / appName / "assets",

            // 3. Alternative Unix location
            execDir / ".." / "share" / "assets",

            // 4. Build directory structure
            execDir / ".." / "assets"
          };

    // Find first existing path
    for (const auto& candidate : candidatePaths) {
      if (std::filesystem::exists (candidate) && std::filesystem::is_directory (candidate)) {
        return std::filesystem::canonical (candidate);
      }
    }

    // Fallback to first candidate if none found
    return candidatePaths[0];
  }
}

// MIT License Copyright (c) 2024-2025 Tomáš Mark
