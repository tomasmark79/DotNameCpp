#include "AssetManager.hpp"
#include <vector>

namespace dotnamecpp::assets {

  AssetManager::AssetManager (std::filesystem::path assetsPath)
    : assetsPath_ (std::move (assetsPath)) {
  }

  std::unique_ptr<IAssetManager> AssetManager::create (const std::filesystem::path& executablePath,
                                                       const std::string& appName) {
    auto path = findAssetsPath (executablePath, appName);
    return std::make_unique<AssetManager> (std::move (path));
  }

  const std::filesystem::path& AssetManager::getAssetsPath () const {
    std::lock_guard<std::mutex> lock (mutex_);
    return assetsPath_;
  }

  std::filesystem::path
  AssetManager::resolveAsset (const std::filesystem::path& relativePath) const {
    std::lock_guard<std::mutex> lock (mutex_);
    return assetsPath_ / relativePath;
  }

  bool AssetManager::assetExists (const std::filesystem::path& relativePath) const {
    auto fullPath = resolveAsset (relativePath);
    return std::filesystem::exists (fullPath);
  }

  bool AssetManager::validate () const {
    std::lock_guard<std::mutex> lock (mutex_);
    return !assetsPath_.empty () && std::filesystem::exists (assetsPath_) &&
           std::filesystem::is_directory (assetsPath_);
  }

  std::filesystem::path AssetManager::findAssetsPath (const std::filesystem::path& executablePath,
                                                      const std::string& appName) {
    std::filesystem::path execDir = executablePath.parent_path ();

    // Priority list of asset locations to try
    std::vector<std::filesystem::path> candidatePaths = {
      // 1. Development/debug - assets next to executable
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
} // namespace dotnamecpp::assets