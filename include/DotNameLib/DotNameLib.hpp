#pragma once

#include <DotNameLib/version.h> // cmake configuration will generate this file
#include <Utils/UtilsFactory.hpp>

#include <memory>

namespace dotnamecpp::v1 {
  class DotNameLib {

  public:
    DotNameLib(std::shared_ptr<logging::ILogger> logger,
               std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager);

    ~DotNameLib();

    DotNameLib(const DotNameLib &other) = delete;
    DotNameLib &operator=(const DotNameLib &other) = delete;
    DotNameLib(DotNameLib &&other) = delete;
    DotNameLib &operator=(DotNameLib &&other) = delete;

    [[nodiscard]] bool isInitialized() const noexcept;
    [[nodiscard]]
    const std::shared_ptr<dotnamecpp::assets::IAssetManager> &getAssetManager() const noexcept;

  private:
    bool isInitialized_{false};
    static constexpr const char *libName_ = "DotNameLib v" DOTNAMELIB_VERSION;

    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
  };

} // namespace dotnamecpp::v1
