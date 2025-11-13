#pragma once

#include <filesystem>

namespace dotnamecpp {

  /**
 * @brief Interface for managing application assets
 * 
 * Provides abstraction for asset path resolution and validation.
 * Allows dependency injection and testing with mock implementations.
 */
  class IAssetManager {
  public:
    virtual ~IAssetManager () = default;

    /**
     * @brief Get the configured assets directory path
     * @return Const reference to assets path
     */
    [[nodiscard]] virtual const std::filesystem::path& getAssetsPath () const = 0;

    /**
     * @brief Resolve a relative asset path to absolute path
     * @param relativePath Asset path relative to assets directory
     * @return Absolute path to the asset
     */
    [[nodiscard]] virtual std::filesystem::path resolveAsset (
        const std::filesystem::path& relativePath) const
        = 0;

    /**
     * @brief Check if an asset file exists
     * @param relativePath Asset path relative to assets directory
     * @return True if asset exists, false otherwise
     */
    [[nodiscard]] virtual bool assetExists (const std::filesystem::path& relativePath) const = 0;

    /**
     * @brief Validate that assets path is accessible
     * @return True if path exists and is a directory, false otherwise
     */
    [[nodiscard]] virtual bool validate () const = 0;
  };

} // namespace dotname