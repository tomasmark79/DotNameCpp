#pragma once

#include <Utils/UtilsError.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace dotname {

  // Import error types
  using dotnamecpp::utils::JsonError;
  using dotnamecpp::utils::Result;

  /**
 * @brief Interface for loading custom application strings
 * 
 * Provides abstraction for loading localized strings, emails, URLs, etc.
 * from customstrings.json format.
 */
  class ICustomStringsLoader {
  public:
    virtual ~ICustomStringsLoader () = default;

    /**
     * @brief Load custom strings JSON from assets
     * @return Result with JSON object or JsonError
     */
    [[nodiscard]] virtual Result<nlohmann::json, JsonError> load () const = 0;

    /**
     * @brief Get localized string by ID
     * @param id String identifier
     * @param locale Locale code (e.g., "en", "cs"), defaults to "en"
     * @return Localized string if found, nullopt otherwise
     */
    [[nodiscard]] virtual std::optional<std::string> getLocalizedString (const std::string& id,
        const std::string& locale = "en") const
        = 0;

    /**
     * @brief Get email by ID
     * @param id String identifier
     * @return Email string if found, nullopt otherwise
     */
    [[nodiscard]] virtual std::optional<std::string> getEmail (const std::string& id) const = 0;

    /**
     * @brief Get URL by ID
     * @param id String identifier
     * @return URL string if found, nullopt otherwise
     */
    [[nodiscard]] virtual std::optional<std::string> getUrl (const std::string& id) const = 0;

    /**
     * @brief Get telephone by ID
     * @param id String identifier
     * @return Telephone string if found, nullopt otherwise
     */
    [[nodiscard]] virtual std::optional<std::string> getTel (const std::string& id) const = 0;

    /**
     * @brief Get all custom string data as formatted signature
     * @return Formatted string with author, contact info, etc.
     */
    [[nodiscard]] virtual std::string getCustomStringSign () const = 0;
  };

} // namespace dotname
