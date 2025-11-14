#pragma once

#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Filesystem/IFileReader.hpp>
#include <Utils/Filesystem/IFileWriter.hpp>
#include <memory>

namespace dotnamecpp {
  namespace utils {

    /**
 * @brief Standard implementation of IJsonSerializer interface
 * 
 * Uses IFileReader/IFileWriter for file I/O with dependency injection.
 */
    class JsonSerializer final : public IJsonSerializer {
    public:
      /**
     * @brief Construct with file I/O dependencies
     * @param fileReader File reader implementation
     * @param fileWriter File writer implementation
     */
      JsonSerializer (std::shared_ptr<IFileReader> fileReader,
          std::shared_ptr<IFileWriter> fileWriter);

      ~JsonSerializer () override = default;

      JsonSerializer (const JsonSerializer&) = delete;
      JsonSerializer& operator= (const JsonSerializer&) = delete;
      JsonSerializer (JsonSerializer&&) = delete;
      JsonSerializer& operator= (JsonSerializer&&) = delete;

      [[nodiscard]]
      Result<nlohmann::json, JsonError> loadFromFile (
          const std::filesystem::path& filePath) const override;

      [[nodiscard]]
      Result<void, JsonError> saveToFile (const std::filesystem::path& filePath,
          const nlohmann::json& jsonData, int indent = 2) const override;

      [[nodiscard]]
      Result<nlohmann::json, JsonError> parse (const std::string& jsonString) const override;

      [[nodiscard]]
      Result<std::string, JsonError> toString (const nlohmann::json& jsonData,
          int indent = 2) const override;

    private:
      std::shared_ptr<IFileReader> fileReader_;
      std::shared_ptr<IFileWriter> fileWriter_;
    };

  } // namespace utils
} // namespace dotnamecpp
