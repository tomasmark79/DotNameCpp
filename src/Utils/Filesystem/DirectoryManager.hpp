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
    ~DirectoryManager () override = default;

    // Delete copy/move - stateless class
    DirectoryManager (const DirectoryManager&) = delete;
    DirectoryManager& operator= (const DirectoryManager&) = delete;
    DirectoryManager (DirectoryManager&&) = delete;
    DirectoryManager& operator= (DirectoryManager&&) = delete;

    /**
     * @brief Create a Directory object
     * 
     * @param dirPath 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    Result<void, FileError> createDirectory (const std::filesystem::path& dirPath) const override;

    /**
     * @brief Remove a Directory object
     * 
     * @param dirPath 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    Result<void, FileError> removeDirectory (const std::filesystem::path& dirPath) const override;

    /**
     * @brief Remove a Directory object recursively
     * 
     * @param dirPath 
     * @return Result<std::uintmax_t, FileError> 
     */
    [[nodiscard]]
    Result<std::uintmax_t, FileError> removeDirectoryRecursive (
        const std::filesystem::path& dirPath) const override;

    /**
     * @brief Check if a directory exists
     * 
     * @param dirPath 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    bool exists (const std::filesystem::path& dirPath) const override;

    /**
     * @brief Check if a directory is empty
     * 
     * @param dirPath 
     * @return Result<bool, FileError> 
     */
    [[nodiscard]]
    Result<bool, FileError> isEmpty (const std::filesystem::path& dirPath) const override;

    /**
     * @brief List entries in a directory
     * 
     * @param dirPath 
     * @return Result<std::vector<std::filesystem::path>, FileError> 
     */
    [[nodiscard]]
    Result<std::vector<std::filesystem::path>, FileError> listEntries (
        const std::filesystem::path& dirPath) const override;

    /**
     * @brief List entries in a directory recursively
     * 
     * @param dirPath 
     * @return Result<std::vector<std::filesystem::path>, FileError> 
     */
    [[nodiscard]]
    Result<std::vector<std::filesystem::path>, FileError> listEntriesRecursive (
        const std::filesystem::path& dirPath) const override;

    /**
     * @brief Get the Current Directory object  
     * 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    Result<std::filesystem::path, FileError> getCurrentDirectory () const override;

    /**
     * @brief Get the Temp Directory object 
     * 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    Result<std::filesystem::path, FileError> getTempDirectory () const override;
  };

} // namespace dotnamecpp::utils