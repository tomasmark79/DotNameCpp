#pragma once

#include <filesystem>

namespace dotnamecpp::assets {
  class IAssetManager {
  public:
    /**
     * @brief Destroy the IAsset Manager object
     */
    virtual ~IAssetManager () = default;

    /**
     * @brief Get the Assets Path object
     * @return const std::filesystem::path& 
     */
    [[nodiscard]] virtual const std::filesystem::path& getAssetsPath () const = 0;

    /**
     * @brief Resolve the full path of an asset given its relative path
     * @param relativePath Relative path to the asset
     * @return std::filesystem::path Full resolved path to the asset
     */
    [[nodiscard]] virtual std::filesystem::path resolveAsset (
        const std::filesystem::path& relativePath) const
        = 0;

    /**
     * @brief Check if an asset exists at the given relative path
     * @param relativePath Relative path to the asset
     * @return true If the asset exists
     * @return false If the asset does not exist
     */
    [[nodiscard]] virtual bool assetExists (const std::filesystem::path& relativePath) const = 0;

    /**
     * @brief Validate the asset manager state
     * @return true If the asset manager is in a valid state
     * @return false If the asset manager is not in a valid state
     */
    [[nodiscard]] virtual bool validate () const = 0;
  };

} // namespace dotnamecpp::assets