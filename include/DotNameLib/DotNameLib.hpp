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

  public:
    /**
     * @brief Construct a new Dot Name Lib object by injection
     * 
     * @param logger 
     * @param assetManager 
     */
    DotNameLib (std::shared_ptr<logging::ILogger> logger,
        std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager);

    ~DotNameLib ();
    DotNameLib (const DotNameLib& other) = delete;
    DotNameLib& operator= (const DotNameLib& other) = delete;

    /**
     * @brief Construct a new Dot Name Lib object by moving
     * 
     * @param other 
     */
    DotNameLib (DotNameLib&& other) noexcept;
    DotNameLib& operator= (DotNameLib&& other) noexcept;

    /**
     * @brief Check if the Dot Name Lib object is initialized
     * 
     * @return true 
     * @return false 
     */
    [[nodiscard]]
    bool isInitialized () const noexcept;

    /**
     * @brief Get the Assets Path object
     * 
     * @return const std::filesystem::path& 
     */
    [[nodiscard]]
    const std::filesystem::path& getAssetsPath () const noexcept;
  };
} // namespace dotnamecpp::v1
