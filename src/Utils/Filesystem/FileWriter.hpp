#pragma once

#include <Utils/Filesystem/IFileWriter.hpp>

namespace dotname {

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

    [[nodiscard]] Result<void, FileError> write (const std::filesystem::path& filePath,
        const std::string& content, bool append = false) const override;

    [[nodiscard]] Result<void, FileError> writeBytes (const std::filesystem::path& filePath,
        const std::vector<uint8_t>& data, bool append = false) const override;

    [[nodiscard]] Result<void, FileError> writeLines (const std::filesystem::path& filePath,
        const std::vector<std::string>& lines, bool append = false) const override;

    [[nodiscard]] Result<void, FileError> touch (
        const std::filesystem::path& filePath) const override;

  private:
    /**
     * @brief Validate path and parent directory
     * @param filePath Path to validate
     * @param requireParent If true, parent directory must exist
     * @return FileError if path is invalid, std::nullopt if OK
     */
    [[nodiscard]] std::optional<FileError> validatePath (const std::filesystem::path& filePath,
        bool requireParent) const;

    /**
     * @brief Ensure parent directory exists, create if needed
     * @param filePath File path whose parent should be created
     * @return FileError if creation failed, std::nullopt if OK
     */
    [[nodiscard]] std::optional<FileError> ensureParentExists (
        const std::filesystem::path& filePath) const;
  };

} // namespace dotname
