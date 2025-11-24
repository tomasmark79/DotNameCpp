#include "JsonSerializer.hpp"
#include <fmt/core.h>

namespace dotnamecpp::utils {

  JsonSerializer::JsonSerializer(std::shared_ptr<IFileReader> fileReader,
                                 std::shared_ptr<IFileWriter> fileWriter)
      : fileReader_(std::move(fileReader)), fileWriter_(std::move(fileWriter)) {
    if (!fileReader_ || !fileWriter_) {
      throw std::invalid_argument("JsonSerializer requires valid file reader and writer");
    }
  }

  Result<nlohmann::json, JsonError>
      JsonSerializer::loadFromFile(const std::filesystem::path &filePath) const {
    // Read file content
    auto contentResult = fileReader_->read(filePath);
    if (!contentResult) {
      return JsonError{
          .code = JsonErrorCode::FileNotFound,
          .message = "Failed to read JSON file: " + contentResult.error().message,
          .details = filePath.string(),
      };
    }

    // Parse JSON
    return parse(contentResult.value());
  }

  Result<void, JsonError> JsonSerializer::saveToFile(const std::filesystem::path &filePath,
                                                     const nlohmann::json &jsonData,
                                                     int indent) const {
    // Convert to string
    auto stringResult = toString(jsonData, indent);
    if (!stringResult) {
      return stringResult.error();
    }

    // Write to file
    auto writeResult = fileWriter_->write(filePath, stringResult.value());
    if (!writeResult) {
      return JsonError{
          .code = JsonErrorCode::FileNotFound,
          .message = "Failed to write JSON file: " + writeResult.error().message,
          .details = filePath.string(),
      };
    }

    return {};
  }

  Result<nlohmann::json, JsonError> JsonSerializer::parse(const std::string &jsonString) const {
    try {
      return nlohmann::json::parse(jsonString);
    } catch (const nlohmann::json::parse_error &e) {
      return JsonError{
          .code = JsonErrorCode::ParseError,
          .message = "JSON parse error",
          .details = fmt::format("Position {}: {}", e.byte, e.what()),
      };
    } catch (const std::exception &e) {
      return JsonError{
          .code = JsonErrorCode::Unknown,
          .message = "Unknown JSON parsing error",
          .details = e.what(),
      };
    }
  }

  Result<std::string, JsonError> JsonSerializer::toString(const nlohmann::json &jsonData,
                                                          int indent) const {
    try {
      return jsonData.dump(indent);
    } catch (const std::exception &e) {
      return JsonError{
          .code = JsonErrorCode::Unknown,
          .message = "Failed to serialize JSON to string",
          .details = e.what(),
      };
    }
  }

} // namespace dotnamecpp::utils