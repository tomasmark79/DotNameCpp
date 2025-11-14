#pragma once

#include <Utils/UtilsError.hpp>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace dotnamecpp::utils {

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
     * @brief Read the entire content of a file as a string
     * 
     * @param filePath 
     * @return Result<std::string, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::string, FileError> read (const std::filesystem::path& filePath) const = 0;

    /**
     * @brief Read the entire content of a file as a vector of bytes
     * 
     * @param filePath 
     * @return Result<std::vector<uint8_t>, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::vector<uint8_t>, FileError>
    readBytes (const std::filesystem::path& filePath) const = 0;

    /**
     * @brief Read the content of a file as a vector of lines
     * 
     * @param filePath 
     * @return Result<std::vector<std::string>, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::vector<std::string>, FileError>
    readLines (const std::filesystem::path& filePath) const = 0;

    /**
     * @brief Check if a file exists
     * 
     * @param filePath 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    virtual bool exists (const std::filesystem::path& filePath) const = 0;

    /**
     * @brief Get the Size object
     * 
     * @param filePath 
     * @return Result<std::uintmax_t, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::uintmax_t, FileError>
    getSize (const std::filesystem::path& filePath) const = 0;
  };

} // namespace dotnamecpp::utils