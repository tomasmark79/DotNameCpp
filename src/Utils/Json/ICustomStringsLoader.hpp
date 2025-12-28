#pragma once

#include <Utils/UtilsError.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace dotnamecpp::utils {

  /**
   * @brief Interface for loading custom strings from JSON
   *
   */
  class ICustomStringsLoader {
  public:
    virtual ~ICustomStringsLoader() = default;

    /**
     * @brief Load the custom strings JSON data
     *
     * @return Result<nlohmann::json, JsonError>
     */
    [[nodiscard]]
    virtual Result<nlohmann::json, JsonError> load() const = 0;

    /**
     * @brief Get the Path object
     *
     * @param id
     * @return std::optional<std::string>
     */
    [[nodiscard]]
    virtual std::optional<std::string> getPath(const std::string &id) const = 0;

    /**
     * @brief Get the Custom Key object
     *
     * @param id
     * @param key
     * @return std::optional<std::string>
     */
    [[nodiscard]]
    virtual std::optional<std::string> getCustomKey(const std::string &id,
                                                    const std::string &key) const = 0;
    /**
     * @brief Get the Localized String object
     *
     * @param id
     * @param locale
     * @return std::optional<std::string>
     */
    [[nodiscard]]
    virtual std::optional<std::string>
        getLocalizedString(const std::string &id, const std::string &locale = "en") const = 0;
  };
} // namespace dotnamecpp::utils