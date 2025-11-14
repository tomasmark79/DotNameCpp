#pragma once

#include <Utils/UtilsError.hpp>
#include <filesystem>
#include <vector>

namespace dotnamecpp::utils {

  /**
    * @brief Interface for directory operations
    * 
    * Provides abstraction for directory creation, removal, and listing.
    */
  class IDirectoryManager {
  public:
    virtual ~IDirectoryManager () = default;

    /**
     * @brief Create a Directory object
     * 
     * @param dirPath 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    virtual Result<void, FileError> createDirectory (
        const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Remove a Directory object
     * 
     * @param dirPath 
     * @return Result<void, FileError> 
     */
    [[nodiscard]]
    virtual Result<void, FileError> removeDirectory (
        const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Remove a Directory object recursively
     * 
     * @param dirPath 
     * @return Result<std::uintmax_t, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::uintmax_t, FileError> removeDirectoryRecursive (
        const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Check if a directory exists
     * 
     * @param dirPath 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    virtual bool exists (const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Check if a directory is empty
     * 
     * @param dirPath 
     * @return Result<bool, FileError> 
     */
    [[nodiscard]]
    virtual Result<bool, FileError> isEmpty (const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief List entries in a directory
     * 
     * @param dirPath 
     * @return Result<std::vector<std::filesystem::path>, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::vector<std::filesystem::path>, FileError> listEntries (
        const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief List entries in a directory recursively
     * 
     * @param dirPath 
     * @return Result<std::vector<std::filesystem::path>, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::vector<std::filesystem::path>, FileError> listEntriesRecursive (
        const std::filesystem::path& dirPath) const = 0;

    /**
     * @brief Get the Current Directory object  
     * 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError> getCurrentDirectory () const = 0;

    /**
     * @brief Get the Temp Directory object 
     * 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError> getTempDirectory () const = 0;
  };

} // namespace dotnamecpp::utils