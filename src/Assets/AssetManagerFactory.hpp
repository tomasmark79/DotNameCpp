#pragma once

#include <Assets/IAssetManager.hpp>
#include <Assets/AssetManager.hpp>
#include <filesystem>
#include <memory>
#include <string>

namespace dotnamecpp::assets {
  class AssetManagerFactory {
  public:
    static std::shared_ptr<IAssetManager> createDefault (
        const std::filesystem::path& executablePath, const std::string& appName) {
      return AssetManager::create (executablePath, appName);
    }
    static std::shared_ptr<IAssetManager> create (std::filesystem::path assetsPath) {
      return std::make_shared<AssetManager> (std::move (assetsPath));
    }
  };
} // namespace dotnamecpp::assets