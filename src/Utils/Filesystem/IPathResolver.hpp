#pragma once

#include <Utils/UtilsError.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace dotnamecpp::utils {

  /**
   * @brief Interface for path resolution and manipulation
   * 
   * Provides abstraction for filesystem path operations.
   */
  class IPathResolver {
  public:
    virtual ~IPathResolver () = default;

    /**
     * @brief Get the Absolute Path object
     * 
     * @param path 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError>
    getAbsolutePath (const std::filesystem::path& path) const = 0;

    /**
     * @brief Get the Canonical Path object
     * 
     * @param path 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError>
    getCanonicalPath (const std::filesystem::path& path) const = 0;

    /**
     * @brief Get the Relative Path object
     * 
     * @param target 
     * @param base 
     * @return Result<std::filesystem::path, FileError> 
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError> getRelativePath (
      const std::filesystem::path& target,
      const std::filesystem::path& base = std::filesystem::current_path ()) const = 0;

    /**
     * @brief Check if path is absolute
     * 
     * @param path 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    virtual bool isAbsolute (const std::filesystem::path& path) const = 0;

    /**
     * @brief Check if path is relative
     * 
     * @param path 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    virtual bool isRelative (const std::filesystem::path& path) const = 0;

    /**
     * @brief Get the Parent object
     * 
     * @param path 
     * @return std::filesystem::path 
     */
    [[nodiscard]]
    virtual std::filesystem::path getParent (const std::filesystem::path& path) const = 0;

    /**
     * @brief Get the Filename object
     * 
     * @param path 
     * @return std::string 
     */
    [[nodiscard]]
    virtual std::string getFilename (const std::filesystem::path& path) const = 0;

    /**
     * @brief Get the Extension object
     * 
     * @param path 
     * @return std::string 
     */
    [[nodiscard]]
    virtual std::string getExtension (const std::filesystem::path& path) const = 0;

    /**
     * @brief Get the Stem object
     * 
     * @param path 
     * @return std::string 
     */
    [[nodiscard]]
    virtual std::string getStem (const std::filesystem::path& path) const = 0;

    /**
    * @brief Join multiple path components
    * 
    * @param parts 
    * @return std::filesystem::path 
    */
    [[nodiscard]]
    virtual std::filesystem::path join (const std::vector<std::string>& parts) const = 0;
  };

} // namespace dotnamecpp::utils