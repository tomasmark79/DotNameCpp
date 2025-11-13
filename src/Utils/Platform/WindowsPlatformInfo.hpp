#pragma once

#include <Utils/Platform/IPlatformInfo.hpp>

namespace dotname {

  /**
 * @brief Windows-specific platform information implementation
 */
  class WindowsPlatformInfo final : public IPlatformInfo {
  public:
    WindowsPlatformInfo () = default;
    ~WindowsPlatformInfo () override = default;

    WindowsPlatformInfo (const WindowsPlatformInfo&) = delete;
    WindowsPlatformInfo& operator= (const WindowsPlatformInfo&) = delete;
    WindowsPlatformInfo (WindowsPlatformInfo&&) = delete;
    WindowsPlatformInfo& operator= (WindowsPlatformInfo&&) = delete;

    [[nodiscard]] Platform getPlatform () const override;
    [[nodiscard]] std::string getPlatformName () const override;
    [[nodiscard]] Result<std::filesystem::path, FileError> getExecutablePath () const override;
    [[nodiscard]] Result<std::filesystem::path, FileError> getExecutableDirectory () const override;
    [[nodiscard]] bool isWindows () const override;
    [[nodiscard]] bool isLinux () const override;
    [[nodiscard]] bool isMacOS () const override;
    [[nodiscard]] bool isEmscripten () const override;
  };

} // namespace dotname
