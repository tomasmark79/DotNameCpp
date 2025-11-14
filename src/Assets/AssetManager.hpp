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

    [[nodiscard]]
    const std::filesystem::path& getAssetsPath () const override;

    [[nodiscard]]
    std::filesystem::path resolveAsset (const std::filesystem::path& relativePath) const override;

    [[nodiscard]]
    bool assetExists (const std::filesystem::path& relativePath) const override;

    [[nodiscard]]
    bool validate () const override;

  private:
    std::filesystem::path assetsPath_;
    mutable std::mutex mutex_;
    static std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
        const std::string& appName);
  };

} // namespace dotnamecpp::assets