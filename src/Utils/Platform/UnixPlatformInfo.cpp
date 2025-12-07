#include "UnixPlatformInfo.hpp"
#include <fmt/core.h>

#if defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace dotnamecpp::utils {

  Platform UnixPlatformInfo::getPlatform() const {
#if defined(__APPLE__)
    return Platform::macOS;
#elif defined(__linux__)
    return Platform::Linux;
#else
    return Platform::Unknown;
#endif
  }

  std::string UnixPlatformInfo::getPlatformName() const {
#if defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif
  }

  Result<std::filesystem::path, FileError> UnixPlatformInfo::getExecutablePath() const {
#if defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t bufferSize = PATH_MAX;

    if (_NSGetExecutablePath(buffer, &bufferSize) != 0) {
      return FileError{
          .code = FileErrorCode::ReadError,
          .message = "Failed to get executable path on macOS",
          .path = "",
      };
    }

    return std::filesystem::path(buffer);

#elif defined(__linux__)
    constexpr size_t LINUX_PATH_BUFFER_SIZE = 4096;
    char buffer[LINUX_PATH_BUFFER_SIZE];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if (len == -1) {
      return FileError{
          .code = FileErrorCode::ReadError,
          .message = "Failed to get executable path on Linux",
          .path = "",
      };
    }

    buffer[len] = '\0';
    return std::filesystem::path(buffer);

#else
    return FileError{
        .code = FileErrorCode::Unknown,
        .message = "UnixPlatformInfo used on unsupported platform",
        .path = "",
    };
#endif
  }

  Result<std::filesystem::path, FileError> UnixPlatformInfo::getExecutableDirectory() const {
    auto exePathResult = getExecutablePath();
    if (!exePathResult) {
      return exePathResult.error();
    }

    return exePathResult.value().parent_path();
  }

  bool UnixPlatformInfo::isWindows() const { return false; }

  bool UnixPlatformInfo::isLinux() const {
#ifdef __linux__
    return true;
#else
    return false;
#endif
  }

  bool UnixPlatformInfo::isMacOS() const {
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
  }

  bool UnixPlatformInfo::isEmscripten() const { return false; }

} // namespace dotnamecpp::utils