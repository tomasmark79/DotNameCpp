#pragma once

#include <Utils/UtilsError.hpp>
#include <filesystem>
#include <vector>

namespace dotname {

  // Import error types
  using dotnamecpp::utils::FileError;
  using dotnamecpp::utils::Result;

  /**
 * @brief Interface for directory operations
 * 
 * Provides abstraction for directory creation, removal, and listing.
 */
  class IDirectoryManager {
  public:
    virtual ~IDirectoryManager () = default;

    /**
     * @brief Create directory (and all parent directories if needed)
     * @param dirPath Path to directory to create
     * @return Result with success or FileError
     */
    [[nodiscard]] virtual Result<void, FileError> createDirectory (
        const std::filesystem::path& dirPath) const
        = 0;

    /**
     * @brief Remove directory (must be empty)
     * @param dirPath Path to directory to remove
     * @return Result with success or FileError
     */
    [[nodiscard]] virtual Result<void, FileError> removeDirectory (
        const std::filesystem::path& dirPath) const
        = 0;

    /**
     * @brief Remove directory and all its contents recursively
     * @param dirPath Path to directory to remove
     * @return Result with number of removed items or FileError
     */
    [[nodiscard]] virtual Result<std::uintmax_t, FileError> removeDirectoryRecursive (
        const std::filesystem::path& dirPath) const
        = 0;

    /**
     * @brief Check if directory exists
     * @param dirPath Path to check
     * @return True if exists and is a directory, false otherwise
     */
    [[nodiscard]] virtual bool exists (const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Check if directory is empty
     * @param dirPath Path to check
     * @return Result with true if empty, false if not, or FileError
     */
    [[nodiscard]] virtual Result<bool, FileError> isEmpty (
        const std::filesystem::path& dirPath) const
        = 0;

    /**
     * @brief List all entries in directory (non-recursive)
     * @param dirPath Directory to list
     * @return Result with vector of paths or FileError
     */
    [[nodiscard]] virtual Result<std::vector<std::filesystem::path>, FileError> listEntries (
        const std::filesystem::path& dirPath) const
        = 0;

    /**
     * @brief List all entries in directory recursively
     * @param dirPath Directory to list
     * @return Result with vector of paths or FileError
     */
    [[nodiscard]] virtual Result<std::vector<std::filesystem::path>, FileError>
    listEntriesRecursive (const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Get current working directory
     * @return Result with current directory path or FileError
     */
    [[nodiscard]] virtual Result<std::filesystem::path, FileError> getCurrentDirectory () const = 0;

    /**
     * @brief Get temporary directory path
     * @return Result with temp directory path or FileError
     */
    [[nodiscard]] virtual Result<std::filesystem::path, FileError> getTempDirectory () const = 0;
  };

} // namespace dotname
