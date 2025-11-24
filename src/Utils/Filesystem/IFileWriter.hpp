#pragma once

#include <Utils/UtilsError.hpp>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace dotnamecpp::utils {

  /**
   * @brief Interface for writing file content
   *
   * Provides abstraction for file writing operations with explicit error handling.
   * All methods return Result<void, FileError> for safe error handling.
   */
  class IFileWriter {
  public:
    virtual ~IFileWriter() = default;

    /**
     * @brief Write string content to file
     *
     * @param filePath
     * @param content
     * @param append
     * @return Result<void, FileError>
     */
    [[nodiscard]]
    virtual Result<void, FileError> write(const std::filesystem::path &filePath,
                                          const std::string &content,
                                          bool append = false) const = 0;

    /**
     * @brief Write binary data to file
     *
     * @param filePath
     * @param data
     * @param append
     * @return Result<void, FileError>
     */
    [[nodiscard]]
    virtual Result<void, FileError> writeBytes(const std::filesystem::path &filePath,
                                               const std::vector<uint8_t> &data,
                                               bool append = false) const = 0;

    /**
     * @brief Write lines to file (each string becomes one line)
     *
     * @param filePath
     * @param lines
     * @param append
     * @return Result<void, FileError>
     */
    [[nodiscard]]
    virtual Result<void, FileError> writeLines(const std::filesystem::path &filePath,
                                               const std::vector<std::string> &lines,
                                               bool append = false) const = 0;

    /**
     * @brief Create empty file or update timestamp of existing file
     *
     * @param filePath
     * @return Result<void, FileError>
     */
    [[nodiscard]]
    virtual Result<void, FileError> touch(const std::filesystem::path &filePath) const = 0;
  };

} // namespace dotnamecpp::utils