#pragma once

#include <Utils/UtilsError.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace dotnamecpp {
  namespace utils {

    /**
 * @brief Interface for path resolution and manipulation
 * 
 * Provides abstraction for filesystem path operations.
 */
    class IPathResolver {
    public:
      virtual ~IPathResolver () = default;

      /**
     * @brief Get absolute path from potentially relative path
     * @param path Path to resolve
     * @return Result with absolute path or FileError
     */
      [[nodiscard]] virtual Result<std::filesystem::path, FileError> getAbsolutePath (
          const std::filesystem::path& path) const
          = 0;

      /**
     * @brief Get canonical path (resolving symlinks, . and ..)
     * @param path Path to canonicalize
     * @return Result with canonical path or FileError
     */
      [[nodiscard]] virtual Result<std::filesystem::path, FileError> getCanonicalPath (
          const std::filesystem::path& path) const
          = 0;

      /**
     * @brief Get relative path from base to target
     * @param target Target path
     * @param base Base path (default: current working directory)
     * @return Result with relative path or FileError
     */
      [[nodiscard]] virtual Result<std::filesystem::path, FileError> getRelativePath (
          const std::filesystem::path& target,
          const std::filesystem::path& base = std::filesystem::current_path ()) const
          = 0;

      /**
     * @brief Check if path is absolute
     * @param path Path to check
     * @return True if absolute, false otherwise
     */
      [[nodiscard]] virtual bool isAbsolute (const std::filesystem::path& path) const = 0;

      /**
     * @brief Check if path is relative
     * @param path Path to check
     * @return True if relative, false otherwise
     */
      [[nodiscard]] virtual bool isRelative (const std::filesystem::path& path) const = 0;

      /**
     * @brief Get parent directory path
     * @param path Path to get parent from
     * @return Parent directory path
     */
      [[nodiscard]] virtual std::filesystem::path getParent (
          const std::filesystem::path& path) const
          = 0;

      /**
     * @brief Get filename from path
     * @param path Path to extract filename from
     * @return Filename (including extension)
     */
      [[nodiscard]] virtual std::string getFilename (const std::filesystem::path& path) const = 0;

      /**
     * @brief Get file extension from path
     * @param path Path to extract extension from
     * @return File extension (including dot, e.g., ".txt")
     */
      [[nodiscard]] virtual std::string getExtension (const std::filesystem::path& path) const = 0;

      /**
     * @brief Get filename without extension
     * @param path Path to extract stem from
     * @return Filename without extension
     */
      [[nodiscard]] virtual std::string getStem (const std::filesystem::path& path) const = 0;

      /**
     * @brief Join multiple path components
     * @param parts Path components to join
     * @return Joined path
     */
      [[nodiscard]] virtual std::filesystem::path join (const std::vector<std::string>& parts) const
          = 0;
    };

  } // namespace utils
} // namespace dotnamecpp
