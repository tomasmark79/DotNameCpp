#include "WindowsPlatformInfo.hpp"
#include <fmt/core.h>

#ifdef _WIN32
  #include <Utils/Platform/WindowsHeaders.hpp>
#endif

namespace dotnamecpp {
  namespace utils {

    Platform WindowsPlatformInfo::getPlatform () const {
      return Platform::Windows;
    }

    std::string WindowsPlatformInfo::getPlatformName () const {
      return "Windows";
    }`

    Result<std::filesystem::path, FileError> WindowsPlatformInfo::getExecutablePath () const {
#ifdef _WIN32
      char buffer[MAX_PATH];
      DWORD result = GetModuleFileNameA (NULL, buffer, MAX_PATH);

      if (result == 0 || result == MAX_PATH) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "Failed to get executable path on Windows",
          .path = "",
        };
      }

      return std::filesystem::path (buffer);
#else
      return FileError{
        .code = FileErrorCode::Unknown,
        .message = "WindowsPlatformInfo used on non-Windows platform",
        .path = "",
      };
#endif
    }

    Result<std::filesystem::path, FileError> WindowsPlatformInfo::getExecutableDirectory () const {
      auto exePathResult = getExecutablePath ();
      if (!exePathResult) {
        return exePathResult.error ();
      }

      return exePathResult.value ().parent_path ();
    }

    bool WindowsPlatformInfo::isWindows () const {
      return true;
    }

    bool WindowsPlatformInfo::isLinux () const {
      return false;
    }

    bool WindowsPlatformInfo::isMacOS () const {
      return false;
    }

    bool WindowsPlatformInfo::isEmscripten () const {
      return false;
    }

  } // namespace utils
} // namespace dotnamecpp
