#pragma once

#include <Utils/Platform/IPlatformInfo.hpp>

namespace dotnamecpp::utils {

  /**
   * @brief Unix-like platform information implementation (Linux/macOS)
   * 
   */
  class UnixPlatformInfo final : public IPlatformInfo {
  public:
    UnixPlatformInfo () = default;

    UnixPlatformInfo (const UnixPlatformInfo&) = delete;
    UnixPlatformInfo& operator= (const UnixPlatformInfo&) = delete;
    UnixPlatformInfo (UnixPlatformInfo&&) = delete;
    UnixPlatformInfo& operator= (UnixPlatformInfo&&) = delete;
    ~UnixPlatformInfo () override = default;

    [[nodiscard]]
    Platform getPlatform () const override;
    [[nodiscard]]
    std::string getPlatformName () const override;
    [[nodiscard]]
    Result<std::filesystem::path, FileError> getExecutablePath () const override;
    [[nodiscard]]
    Result<std::filesystem::path, FileError> getExecutableDirectory () const override;
    [[nodiscard]]
    bool isWindows () const override;
    [[nodiscard]]
    bool isLinux () const override;
    [[nodiscard]]
    bool isMacOS () const override;
    [[nodiscard]]
    bool isEmscripten () const override;
  };

} // namespace dotnamecpp::utils