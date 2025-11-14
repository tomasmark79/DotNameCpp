#pragma once

#include <DotNameLib/version.h> // first configuration will create this file
#include <Utils/Logger/ILogger.hpp>
#include <Utils/Assets/IAssetManager.hpp>
#include <filesystem>
#include <string>
#include <memory>

namespace dotnamecpp::v1 {
  class DotNameLib {

  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
    std::filesystem::path assetsPath_;
    bool isInitialized_ = false;

    [[nodiscard]]
    std::shared_ptr<dotnamecpp::assets::IAssetManager> getAssetManager () const noexcept;

  public:
    // Constructor with dependency injection
    DotNameLib (std::shared_ptr<logging::ILogger> logger,
        std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager);

    // Destructor
    ~DotNameLib ();

    // Non-copyable
    DotNameLib (const DotNameLib& other) = delete;
    DotNameLib& operator= (const DotNameLib& other) = delete;

    // Moveable
    DotNameLib (DotNameLib&& other) noexcept;
    DotNameLib& operator= (DotNameLib&& other) noexcept;

    // Public interface
    [[nodiscard]]
    bool isInitialized () const noexcept;

    [[nodiscard]]
    const std::filesystem::path& getAssetsPath () const noexcept;
  };
} // namespace dotnamecpp::v1
