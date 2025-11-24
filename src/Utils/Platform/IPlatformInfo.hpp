#pragma once

#include <Utils/UtilsError.hpp>
#include <cstdint>
#include <filesystem>
#include <string>

namespace dotnamecpp::utils {

  enum class Platform : std::uint8_t { Windows, Linux, macOS, Emscripten, Unknown };

  class IPlatformInfo {
  public:
    virtual ~IPlatformInfo() = default;

    /**
     * @brief Get the Platform object
     *
     * @return Platform
     */
    [[nodiscard]]
    virtual Platform getPlatform() const = 0;

    /**
     * @brief Get the Platform Name object
     *
     * @return std::string
     */
    [[nodiscard]]
    virtual std::string getPlatformName() const = 0;

    /**
     * @brief Get the Executable Path object
     *
     * @return Result<std::filesystem::path, FileError>
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError> getExecutablePath() const = 0;

    /**
     * @brief Get the Executable Directory object
     *
     * @return Result<std::filesystem::path, FileError>
     */
    [[nodiscard]]
    virtual Result<std::filesystem::path, FileError> getExecutableDirectory() const = 0;

    /**
     * @brief Check if the platform is Windows
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    virtual bool isWindows() const = 0;

    /**
     * @brief Check if the platform is Linux
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    virtual bool isLinux() const = 0;

    /**
     * @brief Check if the platform is macOS
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    virtual bool isMacOS() const = 0;

    /**
     * @brief Check if the platform is Emscripten
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    virtual bool isEmscripten() const = 0;
  };

} // namespace dotnamecpp::utils