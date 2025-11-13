#pragma once

#include <Assets/IAssetManager.hpp>
#include <Assets/AssetManager.hpp>
#include <filesystem>
#include <memory>
#include <string>

namespace dotnamecpp {
  namespace assets {

    /**
 * @brief Factory for creating IAssetManager instances
 */
    class AssetManagerFactory {
    public:
      /**
     * @brief Create asset manager with automatic path discovery
     * @param executablePath Path to the executable
     * @param appName Application name
     * @return Shared pointer to IAssetManager
     */
      static std::shared_ptr<IAssetManager> createDefault (
          const std::filesystem::path& executablePath, const std::string& appName) {
        return AssetManager::create (executablePath, appName);
      }

      /**
     * @brief Create asset manager with explicit path
     * @param assetsPath Path to assets directory
     * @return Shared pointer to IAssetManager
     */
      static std::shared_ptr<IAssetManager> create (std::filesystem::path assetsPath) {
        return std::make_shared<AssetManager> (std::move (assetsPath));
      }
    };

  } // namespace assets
} // namespace dotnamecpp