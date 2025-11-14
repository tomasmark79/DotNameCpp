#pragma once

#include <Utils/Filesystem/IDirectoryManager.hpp>

namespace dotnamecpp::utils {

  /**
   * @brief Standard implementation of IDirectoryManager interface
   * 
   */
  class DirectoryManager final : public IDirectoryManager {
  public:
    DirectoryManager () = default;
    DirectoryManager (const DirectoryManager&) = delete;
    DirectoryManager& operator= (const DirectoryManager&) = delete;
    DirectoryManager (DirectoryManager&&) = delete;
    DirectoryManager& operator= (DirectoryManager&&) = delete;
    ~DirectoryManager () override = default;

    [[nodiscard]]
    Result<void, FileError> createDirectory (const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    Result<void, FileError> removeDirectory (const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    Result<std::uintmax_t, FileError> removeDirectoryRecursive (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    bool exists (const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    Result<bool, FileError> isEmpty (const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    Result<std::vector<std::filesystem::path>, FileError> listEntries (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    Result<std::vector<std::filesystem::path>, FileError> listEntriesRecursive (
        const std::filesystem::path& dirPath) const override;

    [[nodiscard]]
    Result<std::filesystem::path, FileError> getCurrentDirectory () const override;

    [[nodiscard]]
    Result<std::filesystem::path, FileError> getTempDirectory () const override;
  };

} // namespace dotnamecpp::utils