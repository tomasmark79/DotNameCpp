#pragma once

#include <Utils/UtilsError.hpp>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace dotname {

  // Import error types
  using dotnamecpp::utils::FileError;
  using dotnamecpp::utils::Result;

  /**
 * @brief Interface for writing file content
 * 
 * Provides abstraction for file writing operations with explicit error handling.
 * All methods return Result<void, FileError> for safe error handling.
 */
  class IFileWriter {
  public:
    virtual ~IFileWriter () = default;

    /**
     * @brief Write string content to file
     * @param filePath Path to the file to write
     * @param content String content to write
     * @param append If true, append to existing file; if false, overwrite
     * @return Result with success or FileError
     */
    [[nodiscard]] virtual Result<void, FileError> write (const std::filesystem::path& filePath,
        const std::string& content, bool append = false) const
        = 0;

    /**
     * @brief Write binary data to file
     * @param filePath Path to the file to write
     * @param data Byte vector to write
     * @param append If true, append to existing file; if false, overwrite
     * @return Result with success or FileError
     */
    [[nodiscard]] virtual Result<void, FileError> writeBytes (const std::filesystem::path& filePath,
        const std::vector<uint8_t>& data, bool append = false) const
        = 0;

    /**
     * @brief Write lines to file (each string becomes one line)
     * @param filePath Path to the file to write
     * @param lines Vector of lines to write
     * @param append If true, append to existing file; if false, overwrite
     * @return Result with success or FileError
     */
    [[nodiscard]] virtual Result<void, FileError> writeLines (const std::filesystem::path& filePath,
        const std::vector<std::string>& lines, bool append = false) const
        = 0;

    /**
     * @brief Create empty file or update timestamp of existing file
     * @param filePath Path to the file to touch
     * @return Result with success or FileError
     */
    [[nodiscard]] virtual Result<void, FileError> touch (
        const std::filesystem::path& filePath) const
        = 0;
  };

} // namespace dotname
