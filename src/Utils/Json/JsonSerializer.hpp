#pragma once

#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Filesystem/IFileReader.hpp>
#include <Utils/Filesystem/IFileWriter.hpp>
#include <memory>

namespace dotnamecpp::utils {

  /**
    * @brief Standard implementation of IJsonSerializer interface
    *  
    * Uses IFileReader/IFileWriter for file I/O with dependency injection.
    */
  class JsonSerializer final : public IJsonSerializer {
  public:
    /**
     * @brief Construct a new Json Serializer object
     * 
     * @param fileReader 
     * @param fileWriter 
     */
    JsonSerializer (std::shared_ptr<IFileReader> fileReader,
        std::shared_ptr<IFileWriter> fileWriter);

    ~JsonSerializer () override = default;

    JsonSerializer (const JsonSerializer&) = delete;
    JsonSerializer& operator= (const JsonSerializer&) = delete;
    JsonSerializer (JsonSerializer&&) = delete;
    JsonSerializer& operator= (JsonSerializer&&) = delete;

    /**
     * @brief Load JSON from file
     * 
     * @param filePath 
     * @return Result<nlohmann::json, JsonError> 
     */
    [[nodiscard]]
    Result<nlohmann::json, JsonError> loadFromFile (
        const std::filesystem::path& filePath) const override;

    /**
     * @brief Save JSON to file
     * 
     * @param filePath 
     * @param jsonData 
     * @param indent Indentation level (default: 2)
     * @return Result<void, JsonError> 
     */
    [[nodiscard]]
    Result<void, JsonError> saveToFile (const std::filesystem::path& filePath,
        const nlohmann::json& jsonData, int indent = 2) const override;

    /**
     * @brief Parse JSON from string
     * 
     * @param jsonString 
     * @return Result<nlohmann::json, JsonError> 
     */
    [[nodiscard]]
    Result<nlohmann::json, JsonError> parse (const std::string& jsonString) const override;

    /**
     * @brief Convert JSON to string
     * 
     * @param jsonData 
     * @param indent Indentation level (default: 2)
     * @return Result<std::string, JsonError> 
     */
    [[nodiscard]]
    Result<std::string, JsonError> toString (const nlohmann::json& jsonData,
        int indent = 2) const override;

  private:
    std::shared_ptr<IFileReader> fileReader_;
    std::shared_ptr<IFileWriter> fileWriter_;
  };

} // namespace dotnamecpp::utils