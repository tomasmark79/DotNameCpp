#pragma once

#include <Assets/IAssetManager.hpp>
#include <mutex>
#include <memory>

namespace dotnamecpp::assets {
  class AssetManager : public IAssetManager {
  public:
    /**
     * @brief Construct a new Asset Manager object
     * 
     * @param assetsPath 
     */
    explicit AssetManager (std::filesystem::path assetsPath);

    /**
     * @brief Create a new Asset Manager instance
     * 
     * @param executablePath  
     * @param appName 
     * @return std::unique_ptr<IAssetManager> 
     */
    static std::unique_ptr<IAssetManager> create (const std::filesystem::path& executablePath,
        const std::string& appName);

    /**
      * @brief Get the Assets Path object
      * 
      * @return const std::filesystem::path& 
      */
    [[nodiscard]]
    const std::filesystem::path& getAssetsPath () const override;

    /**
     * @brief Resolve the full path of an asset given its relative path
     * 
     * @param relativePath  
     * @return std::filesystem::path 
     */
    [[nodiscard]]
    std::filesystem::path resolveAsset (const std::filesystem::path& relativePath) const override;

    /**
      * @brief Check if an asset exists at the given relative path
      * 
      * @param relativePath 
      * @return true 
      * @return false 
      */
    [[nodiscard]]
    bool assetExists (const std::filesystem::path& relativePath) const override;

    /**
      * @brief Validate the Asset Manager configuration
      * 
      * @return true 
      * @return false 
      */
    [[nodiscard]]
    bool validate () const override;

  private:
    std::filesystem::path assetsPath_;
    mutable std::mutex mutex_;
    static std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
        const std::string& appName);
  };

} // namespace dotnamecpp::assets