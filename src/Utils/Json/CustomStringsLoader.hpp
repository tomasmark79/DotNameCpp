#pragma once

#include <Utils/Json/ICustomStringsLoader.hpp>
#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Assets/IAssetManager.hpp>
#include <memory>
#include <mutex>

namespace dotnamecpp::utils {

  using dotnamecpp::assets::IAssetManager;

  class CustomStringsLoader final : public ICustomStringsLoader {
  public:
    CustomStringsLoader (std::shared_ptr<IAssetManager> assetManager,
                         std::shared_ptr<IJsonSerializer> jsonSerializer,
                         std::string filename = "customstrings.json");

    CustomStringsLoader (const CustomStringsLoader&) = delete;
    CustomStringsLoader& operator= (const CustomStringsLoader&) = delete;
    CustomStringsLoader (CustomStringsLoader&&) = delete;
    CustomStringsLoader& operator= (CustomStringsLoader&&) = delete;
    ~CustomStringsLoader () override = default;

    [[nodiscard]]
    Result<nlohmann::json, JsonError> load () const override;

    [[nodiscard]]
    std::optional<std::string> getLocalizedString (const std::string& id,
                                                   const std::string& locale = "en") const override;

    [[nodiscard]]
    std::optional<std::string> getEmail (const std::string& id) const override;

    [[nodiscard]]
    std::optional<std::string> getUrl (const std::string& id) const override;

    [[nodiscard]]
    std::optional<std::string> getTel (const std::string& id) const override;

    [[nodiscard]]
    std::string getCustomStringSign () const override;

  private:
    std::shared_ptr<IAssetManager> assetManager_;
    std::shared_ptr<IJsonSerializer> jsonSerializer_;
    std::string filename_;

    // Cached data
    mutable std::mutex cacheMutex_;
    mutable std::optional<nlohmann::json> cachedData_;

    [[nodiscard]]
    Result<nlohmann::json, JsonError> ensureLoaded () const;

    [[nodiscard]]
    std::optional<nlohmann::json> findById (const std::string& id) const;
  };

} // namespace dotnamecpp::utils