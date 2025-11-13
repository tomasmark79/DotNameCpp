#pragma once

#include <Assets/IAssetManager.hpp>
#include <mutex>
#include <memory>

namespace dotnamecpp {
namespace assets {

  /**
 * @brief Concrete implementation of IAssetManager
 * 
 * Manages asset paths with thread-safe operations.
 * Supports automatic asset path discovery based on executable location.
 */
  class AssetManager : public IAssetManager {
  public:
    /**
     * @brief Construct with explicit assets path
     * @param assetsPath Path to assets directory
     */
    explicit AssetManager (std::filesystem::path assetsPath);

    /**
     * @brief Factory method for automatic asset discovery
     * @param executablePath Path to the executable
     * @param appName Application name for path resolution
     * @return Unique pointer to IAssetManager instance
     */
    static std::unique_ptr<IAssetManager> create (const std::filesystem::path& executablePath,
        const std::string& appName);

    // IAssetManager interface implementation
    [[nodiscard]] const std::filesystem::path& getAssetsPath () const override;
    [[nodiscard]] std::filesystem::path resolveAsset (
        const std::filesystem::path& relativePath) const override;
    [[nodiscard]] bool assetExists (const std::filesystem::path& relativePath) const override;
    [[nodiscard]] bool validate () const override;

  private:
    std::filesystem::path assetsPath_;
    mutable std::mutex mutex_; // For thread-safe operations

    /**
     * @brief Find assets path by checking multiple standard locations
     * @param executablePath Path to the executable
     * @param appName Application name
     * @return Discovered assets path or fallback
     */
    static std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
        const std::string& appName);
  };

} // namespace assets
} // namespace dotnamecpp
