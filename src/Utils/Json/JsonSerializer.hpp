#pragma once

#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Filesystem/IFileReader.hpp>
#include <Utils/Filesystem/IFileWriter.hpp>
#include <memory>

namespace dotnamecpp::utils {

  class JsonSerializer final : public IJsonSerializer {
  public:
    JsonSerializer (std::shared_ptr<IFileReader> fileReader,
        std::shared_ptr<IFileWriter> fileWriter);

    JsonSerializer (const JsonSerializer&) = delete;
    JsonSerializer& operator= (const JsonSerializer&) = delete;
    JsonSerializer (JsonSerializer&&) = delete;
    JsonSerializer& operator= (JsonSerializer&&) = delete;
    ~JsonSerializer () override = default;

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

} // namespace dotnamecpp::utils