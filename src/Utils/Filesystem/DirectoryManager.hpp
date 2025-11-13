#pragma once

#include <Utils/Filesystem/IDirectoryManager.hpp>

namespace dotname {

  /**
 * @brief Standard implementation of IDirectoryManager interface
 * 
 * Provides directory operations using std::filesystem.
 * Thread-safe for concurrent operations from different instances.
 */
  class DirectoryManager final : public IDirectoryManager {
  public:
    DirectoryManager () = default;
    ~DirectoryManager () override = default;

    // Delete copy/move - stateless class
    DirectoryManager (const DirectoryManager&) = delete;
    DirectoryManager& operator= (const DirectoryManager&) = delete;
    DirectoryManager (DirectoryManager&&) = delete;
    DirectoryManager& operator= (DirectoryManager&&) = delete;

    [[nodiscard]] Result<void, FileError> createDirectory (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]] Result<void, FileError> removeDirectory (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]] Result<std::uintmax_t, FileError> removeDirectoryRecursive (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]] bool exists (const std::filesystem::path& dirPath) const override;

    [[nodiscard]] Result<bool, FileError> isEmpty (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]] Result<std::vector<std::filesystem::path>, FileError> listEntries (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]] Result<std::vector<std::filesystem::path>, FileError> listEntriesRecursive (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]] Result<std::filesystem::path, FileError> getCurrentDirectory () const override;

    [[nodiscard]] Result<std::filesystem::path, FileError> getTempDirectory () const override;
  };

} // namespace dotname
