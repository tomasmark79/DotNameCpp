#pragma once

#include <filesystem>

namespace dotnamecpp::assets {
  class IAssetManager {
  public:
    virtual ~IAssetManager () = default;
    /**
     * @brief Get the Assets Path object
     * 
     * @return const std::filesystem::path& 
     */
    [[nodiscard]] virtual const std::filesystem::path& getAssetsPath () const = 0;
    [[nodiscard]] virtual std::filesystem::path resolveAsset (
        const std::filesystem::path& relativePath) const
        = 0;
    [[nodiscard]] virtual bool assetExists (const std::filesystem::path& relativePath) const = 0;
    [[nodiscard]] virtual bool validate () const = 0;
  };
} // namespace dotnamecpp::assets