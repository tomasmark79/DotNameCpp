#pragma once

#include <Utils/Platform/IPlatformInfo.hpp>
#include <memory>

namespace dotnamecpp {
  namespace utils {

    /**
 * @brief Factory for creating platform-specific IPlatformInfo instances
 */
    class PlatformInfoFactory {
    public:
      /**
     * @brief Create platform info for current platform
     * @return Unique pointer to platform-specific implementation
     */
      [[nodiscard]] static std::unique_ptr<IPlatformInfo> createForCurrentPlatform ();

      /**
     * @brief Create platform info for specific platform
     * @param platform Target platform
     * @return Unique pointer to platform-specific implementation
     */
      [[nodiscard]] static std::unique_ptr<IPlatformInfo> create (Platform platform);
    };

  } // namespace utils
} // namespace dotnamecpp
