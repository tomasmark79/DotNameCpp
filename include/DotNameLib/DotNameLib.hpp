#pragma once

#include <DotNameLib/version.h> // first configuration will create this file
#include <Utils/UtilsFactory.hpp>
#include <filesystem>
#include <memory>

#include <string>

namespace dotnamecpp::v1 {
  class DotNameLib {

  public:
    /**
     * @brief Construct a new object
     *
     * @param logger
     * @param assetManager
     */
    DotNameLib(std::shared_ptr<logging::ILogger> logger,
               std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager);

    /**
     * @brief Destroy the object
     *
     */
    ~DotNameLib();

    /**
     * @brief Don't allow copy construction
     *
     * @param other
     */
    DotNameLib(const DotNameLib &other) = delete;

    /**
     * @brief Don't allow copy assignment
     *
     * @param other
     * @return DotNameLib&
     */
    DotNameLib &operator=(const DotNameLib &other) = delete;

    /**
     * @brief Construct a new object by moving
     *
     * @param other
     */
    DotNameLib(DotNameLib &&other) noexcept;

    /**
     * @brief Move assignment operator - default
     *
     * @param other
     * @return DotNameLib&
     */
    DotNameLib &operator=(DotNameLib &&other) noexcept;

    /**
     * @brief Check if the object is initialized
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    bool isInitialized() const noexcept;

    /**
     * @brief Get the Assets Path object
     *
     * @return const std::filesystem::path&
     */
    [[nodiscard]]
    const std::filesystem::path &getAssetsPath() const noexcept;

  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
    std::filesystem::path assetsPath_;
    bool isInitialized_ = false;
  };

} // namespace dotnamecpp::v1
