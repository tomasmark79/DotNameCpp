#pragma once

#include <Utils/Json/ICustomStringsLoader.hpp>
#include <Utils/Json/IJsonSerializer.hpp>
#include <Assets/IAssetManager.hpp>
#include <memory>
#include <mutex>

namespace dotnamecpp::utils {
  using dotnamecpp::assets::IAssetManager;

  /**
   * @brief Loader for custom strings JSON data
   * 
   */
  class CustomStringsLoader final : public ICustomStringsLoader {
  public:
    /**
    * @brief Construct a new Custom Strings Loader object
    * 
    * @param assetManager 
    * @param jsonSerializer 
    * @param filename 
    */
    CustomStringsLoader (std::shared_ptr<IAssetManager> assetManager,
        std::shared_ptr<IJsonSerializer> jsonSerializer,
        std::string filename = "customstrings.json");

    ~CustomStringsLoader () override = default;

    CustomStringsLoader (const CustomStringsLoader&) = delete;
    CustomStringsLoader& operator= (const CustomStringsLoader&) = delete;
    CustomStringsLoader (CustomStringsLoader&&) = delete;
    CustomStringsLoader& operator= (CustomStringsLoader&&) = delete;

    /**
     * @brief Load the custom strings JSON data
     * 
     * @return Result<nlohmann::json, JsonError> 
     */
    [[nodiscard]]
    Result<nlohmann::json, JsonError> load () const override;

    /**
     * @brief Get the Localized String object
     * 
     * @param id 
     * @param locale 
     * @return std::optional<std::string> 
     */
    [[nodiscard]]
    std::optional<std::string> getLocalizedString (const std::string& id,
        const std::string& locale = "en") const override;

    /**
     * @brief Get the Email object
     * 
     * @param id 
     * @return std::optional<std::string> 
     */
    [[nodiscard]]
    std::optional<std::string> getEmail (const std::string& id) const override;

    /**
     * @brief Get the Url object
     * 
     * @param id 
     * @return std::optional<std::string> 
     */
    [[nodiscard]]
    std::optional<std::string> getUrl (const std::string& id) const override;

    /**
     * @brief Get the Tel object
     * 
     * @param id 
     * @return std::optional<std::string> 
     */
    [[nodiscard]]
    std::optional<std::string> getTel (const std::string& id) const override;

    /**
     * @brief Get the Custom String Sign object
     * 
     * @return std::string 
     */
    [[nodiscard]]
    std::string getCustomStringSign () const override;

  private:
    std::shared_ptr<IAssetManager> assetManager_;
    std::shared_ptr<IJsonSerializer> jsonSerializer_;
    std::string filename_;

    // Cached data
    mutable std::mutex cacheMutex_;
    mutable std::optional<nlohmann::json> cachedData_;

    /**
      * @brief Ensure the custom strings JSON is loaded and cached 
      * 
      * @return Result<nlohmann::json, JsonError> 
      */
    [[nodiscard]]
    Result<nlohmann::json, JsonError> ensureLoaded () const;

    /**
     * @brief Find a string item by its ID 
     * 
     * @param id 
     * @return std::optional<nlohmann::json> 
     */
    [[nodiscard]]
    std::optional<nlohmann::json> findById (const std::string& id) const;
  };

} // namespace dotnamecpp::utils