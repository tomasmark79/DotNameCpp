#pragma once

#include <Utils/Filesystem/IFileReader.hpp>

namespace dotnamecpp::utils {

  /**
   * @brief Standard implementation of IFileReader interface
   * 
   * Provides file reading operations using std::filesystem and standard C++ streams.
   * Thread-safe for concurrent reads from different instances.
   */
  class FileReader final : public IFileReader {
  public:
    FileReader () = default;
    ~FileReader () override = default;

    // Delete copy/move - stateless class, just use multiple instances
    FileReader (const FileReader&) = delete;
    FileReader& operator= (const FileReader&) = delete;
    FileReader (FileReader&&) = delete;
    FileReader& operator= (FileReader&&) = delete;

    /**
     * @brief Read the entire contents of a file as a string
     * 
     * @param filePath 
     * @return Result<std::string, FileError> 
     */
    [[nodiscard]]
    Result<std::string, FileError> read (const std::filesystem::path& filePath) const override;

    /**
     * @brief Read the entire contents of a file as a byte vector
     * 
     * @param filePath 
     * @return Result<std::vector<uint8_t>, FileError> 
     */
    [[nodiscard]]
    Result<std::vector<uint8_t>, FileError> readBytes (
        const std::filesystem::path& filePath) const override;

    /**
     * @brief Read the entire contents of a file as a vector of lines
     * 
     * @param filePath 
     * @return Result<std::vector<std::string>, FileError> 
     */
    [[nodiscard]]
    Result<std::vector<std::string>, FileError> readLines (
        const std::filesystem::path& filePath) const override;

    /**
     * @brief Check if a file exists
     * 
     * @param filePath 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    bool exists (const std::filesystem::path& filePath) const override;

    /**
     * @brief Get the Size object 
     * 
     * @param filePath 
     * @return Result<std::uintmax_t, FileError> 
     */
    [[nodiscard]]
    Result<std::uintmax_t, FileError> getSize (
        const std::filesystem::path& filePath) const override;

  private:
    /**
     * @brief Validate path and check file accessibility
     * 
     * @param filePath 
     * @return std::optional<FileError> 
     */
    [[nodiscard]]
    static std::optional<FileError> validatePath (const std::filesystem::path& filePath);
  };

} // namespace dotnamecpp::utils