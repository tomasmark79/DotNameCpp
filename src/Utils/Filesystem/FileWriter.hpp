#pragma once

#include <Utils/Filesystem/IFileWriter.hpp>

namespace dotnamecpp::utils {

  /**
   * @brief Standard implementation of IFileWriter interface
   * 
   * Provides file writing operations using std::filesystem and standard C++ streams.
   * Thread-safe for concurrent writes from different instances.
   */
  class FileWriter final : public IFileWriter {
  public:
    FileWriter () = default;
    ~FileWriter () override = default;

    // Delete copy/move - stateless class, just use multiple instances
    FileWriter (const FileWriter&) = delete;
    FileWriter& operator= (const FileWriter&) = delete;
    FileWriter (FileWriter&&) = delete;
    FileWriter& operator= (FileWriter&&) = delete;

    /**
     * @brief Write a string to a file
     * 
     * @param filePath 
     * @param content 
     * @param append 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    Result<void, FileError> write (const std::filesystem::path& filePath,
        const std::string& content, bool append = false) const override;

    /**
     * @brief Write bytes to a file
     * 
     * @param filePath 
     * @param data 
     * @param append 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    Result<void, FileError> writeBytes (const std::filesystem::path& filePath,
        const std::vector<uint8_t>& data, bool append = false) const override;

    /**
     * @brief Write lines to a file
     * 
     * @param filePath 
     * @param lines 
     * @param append 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    Result<void, FileError> writeLines (const std::filesystem::path& filePath,
        const std::vector<std::string>& lines, bool append = false) const override;

    /**
     * @brief Update the timestamp of a file or create it if it doesn't exist
     * 
     * @param filePath 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    Result<void, FileError> touch (const std::filesystem::path& filePath) const override;

  private:
    /**
     * @brief Validate path and parent directory
     * 
     * @param filePath 
     * @param requireParent 
     * @return std::optional<FileError> 
     */
    [[nodiscard]]
    static std::optional<FileError> validatePath (const std::filesystem::path& filePath,
        bool requireParent);

    /**
     * @brief Ensure parent directory exists, create if needed
     * 
     * @param filePath 
     * @return std::optional<FileError> 
     */
    [[nodiscard]]
    static std::optional<FileError> ensureParentExists (const std::filesystem::path& filePath);
  };

} // namespace dotnamecpp::utils