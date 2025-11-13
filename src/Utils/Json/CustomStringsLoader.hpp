#pragma once

#include <Utils/Json/ICustomStringsLoader.hpp>
#include <Utils/Json/IJsonSerializer.hpp>
#include <Assets/IAssetManager.hpp>
#include <memory>
#include <mutex>

namespace dotnamecpp {
  namespace utils {

    // Import asset types
    using dotnamecpp::assets::IAssetManager;

    /**
 * @brief Implementation of ICustomStringsLoader with dependency injection
 * 
 * Loads customstrings.json from assets using IAssetManager and IJsonSerializer.
 * Caches loaded data for performance.
 */
    class CustomStringsLoader final : public ICustomStringsLoader {
    public:
      /**
     * @brief Construct with dependencies
     * @param assetManager Asset manager for resolving asset paths
     * @param jsonSerializer JSON serializer for parsing files
     * @param filename Custom strings filename (default: "customstrings.json")
     */
      CustomStringsLoader (std::shared_ptr<IAssetManager> assetManager,
          std::shared_ptr<IJsonSerializer> jsonSerializer,
          std::string filename = "customstrings.json");

      ~CustomStringsLoader () override = default;

      CustomStringsLoader (const CustomStringsLoader&) = delete;
      CustomStringsLoader& operator= (const CustomStringsLoader&) = delete;
      CustomStringsLoader (CustomStringsLoader&&) = delete;
      CustomStringsLoader& operator= (CustomStringsLoader&&) = delete;

      [[nodiscard]] Result<nlohmann::json, JsonError> load () const override;

      [[nodiscard]] std::optional<std::string> getLocalizedString (const std::string& id,
          const std::string& locale = "en") const override;

      [[nodiscard]] std::optional<std::string> getEmail (const std::string& id) const override;

      [[nodiscard]] std::optional<std::string> getUrl (const std::string& id) const override;

      [[nodiscard]] std::optional<std::string> getTel (const std::string& id) const override;

      [[nodiscard]] std::string getCustomStringSign () const override;

    private:
      std::shared_ptr<IAssetManager> assetManager_;
      std::shared_ptr<IJsonSerializer> jsonSerializer_;
      std::string filename_;

      // Cached data
      mutable std::mutex cacheMutex_;
      mutable std::optional<nlohmann::json> cachedData_;

      /**
     * @brief Ensure data is loaded and cached
     * @return Result with JSON or error
     */
      [[nodiscard]] Result<nlohmann::json, JsonError> ensureLoaded () const;

      /**
     * @brief Find item by ID in strings array
     * @param id Item identifier
     * @return JSON object if found, nullopt otherwise
     */
      [[nodiscard]] std::optional<nlohmann::json> findById (const std::string& id) const;
    };

  } // namespace utils
} // namespace dotnamecpp
