#pragma once

#include <Utils/UtilsError.hpp>
#include <cstdint>
#include <filesystem>
#include <string>

namespace dotname {

  // Import error types
  using dotnamecpp::utils::FileError;
  using dotnamecpp::utils::Result;

  /**
 * @brief Platform information enumeration
 */
  enum class Platform : std::uint8_t { Windows, Linux, macOS, Emscripten, Unknown };

  /**
 * @brief Interface for platform-specific information
 * 
 * Provides abstraction for platform detection and executable path retrieval.
 */
  class IPlatformInfo {
  public:
    virtual ~IPlatformInfo () = default;

    /**
     * @brief Get current platform
     * @return Platform enum value
     */
    [[nodiscard]] virtual Platform getPlatform () const = 0;

    /**
     * @brief Get platform name as string
     * @return Platform name (e.g., "Windows", "Linux", "macOS", "Emscripten")
     */
    [[nodiscard]] virtual std::string getPlatformName () const = 0;

    /**
     * @brief Get full path to current executable
     * @return Result with executable path or FileError
     */
    [[nodiscard]] virtual Result<std::filesystem::path, FileError> getExecutablePath () const = 0;

    /**
     * @brief Get directory containing current executable
     * @return Result with executable directory or FileError
     */
    [[nodiscard]] virtual Result<std::filesystem::path, FileError> getExecutableDirectory () const
        = 0;

    /**
     * @brief Check if running on Windows
     * @return True if Windows platform
     */
    [[nodiscard]] virtual bool isWindows () const = 0;

    /**
     * @brief Check if running on Linux
     * @return True if Linux platform
     */
    [[nodiscard]] virtual bool isLinux () const = 0;

    /**
     * @brief Check if running on macOS
     * @return True if macOS platform
     */
    [[nodiscard]] virtual bool isMacOS () const = 0;

    /**
     * @brief Check if running on Emscripten (WebAssembly)
     * @return True if Emscripten platform
     */
    [[nodiscard]] virtual bool isEmscripten () const = 0;
  };

} // namespace dotname
