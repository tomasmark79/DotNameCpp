#include "PlatformInfoFactory.hpp"
#include "EmscriptenPlatformInfo.hpp"
#include "UnixPlatformInfo.hpp"
#include "WindowsPlatformInfo.hpp"
#include <stdexcept>

namespace dotnamecpp {
  namespace utils {

    std::unique_ptr<IPlatformInfo> PlatformInfoFactory::createForCurrentPlatform () {
#ifdef _WIN32
    return std::make_unique<WindowsPlatformInfo> ();
#elif defined(__EMSCRIPTEN__)
    return std::make_unique<EmscriptenPlatformInfo> ();
#elif defined(__APPLE__) || defined(__linux__)
    return std::make_unique<UnixPlatformInfo> ();
#else
    throw std::runtime_error ("Unsupported platform");
#endif
  }

  std::unique_ptr<IPlatformInfo> PlatformInfoFactory::create (Platform platform) {
    switch (platform) {
    case Platform::Windows:
      return std::make_unique<WindowsPlatformInfo> ();
    case Platform::Linux:
    case Platform::macOS:
      return std::make_unique<UnixPlatformInfo> ();
    case Platform::Emscripten:
      return std::make_unique<EmscriptenPlatformInfo> ();
    case Platform::Unknown:
    default:
      throw std::invalid_argument ("Cannot create platform info for Unknown platform");
    }
  }

  } // namespace utils
} // namespace dotnamecpp
