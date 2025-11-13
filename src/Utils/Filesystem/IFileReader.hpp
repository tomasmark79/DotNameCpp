#pragma once

#include <Utils/UtilsError.hpp>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace dotnamecpp {
  namespace utils {

    /**
 * @brief Interface for reading file content
 * 
 * Provides abstraction for file reading operations with explicit error handling.
 * All methods return Result<T, FileError> for safe error handling.
 */
  class IFileReader {
  public:
    virtual ~IFileReader () = default;

    /**
     * @brief Read entire file content as string
     * @param filePath Path to the file to read
     * @return Result with file content or FileError
     */
    [[nodiscard]] virtual Result<std::string, FileError> read (
        const std::filesystem::path& filePath) const
        = 0;

    /**
     * @brief Read file content as bytes
     * @param filePath Path to the file to read
     * @return Result with byte vector or FileError
     */
    [[nodiscard]] virtual Result<std::vector<uint8_t>, FileError> readBytes (
        const std::filesystem::path& filePath) const
        = 0;

    /**
     * @brief Read file content line by line
     * @param filePath Path to the file to read
     * @return Result with vector of lines or FileError
     */
    [[nodiscard]] virtual Result<std::vector<std::string>, FileError> readLines (
        const std::filesystem::path& filePath) const
        = 0;

    /**
     * @brief Check if file exists and is a regular file
     * @param filePath Path to check
     * @return True if file exists and is regular file, false otherwise
     */
    [[nodiscard]] virtual bool exists (const std::filesystem::path& filePath) const = 0;

    /**
     * @brief Get file size in bytes
     * @param filePath Path to the file
     * @return Result with file size or FileError
     */
    [[nodiscard]] virtual Result<std::uintmax_t, FileError> getSize (
        const std::filesystem::path& filePath) const
        = 0;
  };

  } // namespace utils
} // namespace dotnamecpp
