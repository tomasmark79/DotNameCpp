#pragma once

#include <Utils/Platform/IPlatformInfo.hpp>

namespace dotnamecpp {
  namespace utils {

    /**
 * @brief Emscripten/WebAssembly platform information implementation
 */
    class EmscriptenPlatformInfo final : public IPlatformInfo {
    public:
      EmscriptenPlatformInfo () = default;
      ~EmscriptenPlatformInfo () override = default;

      EmscriptenPlatformInfo (const EmscriptenPlatformInfo&) = delete;
      EmscriptenPlatformInfo& operator= (const EmscriptenPlatformInfo&) = delete;
      EmscriptenPlatformInfo (EmscriptenPlatformInfo&&) = delete;
      EmscriptenPlatformInfo& operator= (EmscriptenPlatformInfo&&) = delete;

      [[nodiscard]] Platform getPlatform () const override;
      [[nodiscard]] std::string getPlatformName () const override;
      [[nodiscard]] Result<std::filesystem::path, FileError> getExecutablePath () const override;
      [[nodiscard]] Result<std::filesystem::path, FileError>
      getExecutableDirectory () const override;
      [[nodiscard]] bool isWindows () const override;
      [[nodiscard]] bool isLinux () const override;
      [[nodiscard]] bool isMacOS () const override;
      [[nodiscard]] bool isEmscripten () const override;
    };

  } // namespace utils
} // namespace dotnamecpp
