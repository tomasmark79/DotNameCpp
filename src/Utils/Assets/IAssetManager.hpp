#pragma once

#include <filesystem>

namespace dotnamecpp::assets {
  class IAssetManager {
  public:
    virtual ~IAssetManager() = default;

    /**
     * @brief Get the Assets Path object
     *
     * @return const std::filesystem::path&
     */
    [[nodiscard]]
    virtual const std::filesystem::path &getAssetsPath() const = 0;

    /**
     * @brief Resolve the full path of an asset given its relative path
     *
     * @param relativePath
     * @return std::filesystem::path
     */
    [[nodiscard]]
    virtual std::filesystem::path resolveAsset(const std::filesystem::path &relativePath) const = 0;

    /**
     * @brief Check if an asset exists given its relative path
     *
     * @param relativePath
     * @return true
     * @return false
     */
    [[nodiscard]]
    virtual bool assetExists(const std::filesystem::path &relativePath) const = 0;

    /**
     * @brief Validate the asset manager configuration
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    virtual bool validate() const = 0;
  };

} // namespace dotnamecpp::assets