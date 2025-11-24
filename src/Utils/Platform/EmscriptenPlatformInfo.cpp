#include "EmscriptenPlatformInfo.hpp"
#include <fmt/core.h>

namespace dotnamecpp::utils {

  Platform EmscriptenPlatformInfo::getPlatform() const {
    return Platform::Emscripten;
  }

  std::string EmscriptenPlatformInfo::getPlatformName() const {
    return "Emscripten";
  }

  Result<std::filesystem::path, FileError> EmscriptenPlatformInfo::getExecutablePath() const {
    // Emscripten doesn't have a traditional executable path
    // Return current working directory as fallback
    try {
      return std::filesystem::current_path();
    } catch (const std::exception &e) {
      return FileError{
          .code = FileErrorCode::ReadError,
          .message = fmt::format("Failed to get current path in Emscripten: {}", e.what()),
          .path = "",
      };
    }
  }

  Result<std::filesystem::path, FileError> EmscriptenPlatformInfo::getExecutableDirectory() const {
    return getExecutablePath(); // Same as executable path for Emscripten
  }

  bool EmscriptenPlatformInfo::isWindows() const {
    return false;
  }

  bool EmscriptenPlatformInfo::isLinux() const {
    return false;
  }

  bool EmscriptenPlatformInfo::isMacOS() const {
    return false;
  }

  bool EmscriptenPlatformInfo::isEmscripten() const {
    return true;
  }

} // namespace dotnamecpp::utils