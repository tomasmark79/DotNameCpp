#pragma once

#include <Assets/IAssetManager.hpp>
#include <mutex>
#include <memory>

namespace dotnamecpp::assets {
  class AssetManager : public IAssetManager {
  public:
    /**
     * @brief Construct a new Asset Manager object
     * @param assetsPath 
     */
    explicit AssetManager (std::filesystem::path assetsPath);

    /**
     * @brief Create a new Asset Manager instance
     * @param executablePath Path to the executable
     * @param appName Name of the application
     * @return std::unique_ptr<IAssetManager> 
     */
    static std::unique_ptr<IAssetManager> create (const std::filesystem::path& executablePath,
        const std::string& appName);

    /**
     * @brief Get the Assets Path object
     * @return const std::filesystem::path& 
     */
    [[nodiscard]] const std::filesystem::path& getAssetsPath () const override;

    /**
     * @brief Resolve the full path of an asset given its relative path
     * @param relativePath Relative path to the asset
     * @return std::filesystem::path Full resolved path to the asset
     */
    [[nodiscard]] std::filesystem::path resolveAsset (
        const std::filesystem::path& relativePath) const override;

    /**
     * @brief Check if an asset exists at the given relative path
     * @param relativePath Relative path to the asset
     * @return true If the asset exists
     * @return false If the asset does not exist
     */
    [[nodiscard]] bool assetExists (const std::filesystem::path& relativePath) const override;

    /**
     * @brief Validate the asset manager state
     * @return true If the asset manager is in a valid state
     * @return false If the asset manager is not in a valid state
     */
    [[nodiscard]] bool validate () const override;

  private:
    std::filesystem::path assetsPath_;
    mutable std::mutex mutex_;
    static std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
        const std::string& appName);
  };

} // namespace dotnamecpp::assets