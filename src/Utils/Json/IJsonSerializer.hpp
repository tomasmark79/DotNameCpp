#pragma once

#include <Utils/UtilsError.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace dotnamecpp::utils {
  /**
   * @brief Interface for JSON serialization and parsing
   * 
   * Provides safe JSON operations with Result<T,E> error handling.
   */
  class IJsonSerializer {
  public:
    virtual ~IJsonSerializer () = default;

    /**
     * @brief Load JSON from file
     * 
     * @param filePath 
     * @return Result<nlohmann::json, JsonError> 
     */
    [[nodiscard]]
    virtual Result<nlohmann::json, JsonError>
    loadFromFile (const std::filesystem::path& filePath) const = 0;

    /**
     * @brief Save JSON to file
     * 
     * @param filePath 
     * @param jsonData 
     * @param indent Indentation level (default: 2)
     * @return Result<void, JsonError> 
     */
    [[nodiscard]]
    virtual Result<void, JsonError> saveToFile (const std::filesystem::path& filePath,
                                                const nlohmann::json& jsonData,
                                                int indent = 2) const = 0;

    /**
     * @brief Parse JSON from string
     * 
     * @param jsonString 
     * @return Result<nlohmann::json, JsonError> 
     */
    [[nodiscard]]
    virtual Result<nlohmann::json, JsonError> parse (const std::string& jsonString) const = 0;

    /**
     * @brief Convert JSON to string
     * 
     * @param jsonData 
     * @param indent Indentation level (default: 2)
     * @return Result<std::string, JsonError> 
     */
    [[nodiscard]]
    virtual Result<std::string, JsonError> toString (const nlohmann::json& jsonData,
                                                     int indent = 2) const = 0;
  };

} // namespace dotnamecpp::utils