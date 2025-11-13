#pragma once

#include <DotNameLib/version.h> // first configuration will create this file
#include <Logger/ILogger.hpp>
#include <Logger/NullLogger.hpp>
#include <Assets/IAssetManager.hpp>
#include <Assets/AssetManagerFactory.hpp>
#include <filesystem>
#include <string>

namespace dotnamecpp::v1 {
  class DotNameLib {

  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
    std::filesystem::path assetsPath_;
    bool isInitialized_ = false;

    [[nodiscard]]
    std::shared_ptr<dotnamecpp::assets::IAssetManager> getAssetManager () const noexcept {
      return assetManager_;
    }

  public:
    DotNameLib (std::shared_ptr<logging::ILogger> logger,
        std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager)
        : logger_ (logger ? std::move (logger) : std::make_shared<logging::NullLogger> ()),
          assetManager_ (std::move (assetManager)) {
      if (assetManager_ && assetManager_->validate ()) {

        logger_->infoStream () << "DotNameLib initialized";

        const auto logoPath = assetManager_->resolveAsset ("DotNameCppLogo.svg");
        if (assetManager_->assetExists ("DotNameCppLogo.svg")) {
          logger_->debugStream () << "Logo: " << logoPath << " found";
        } else {
          logger_->warningStream () << "Logo not found: " << logoPath;
        }
      } else {
        logger_->errorStream () << "Invalid or missing asset manager";
      }
    }

    // Non-copyable
    DotNameLib (const DotNameLib& other) = delete;
    DotNameLib& operator= (const DotNameLib& other) = delete;

    // Moveable
    DotNameLib (DotNameLib&& other) noexcept : logger_ (std::move (other.logger_)),
                                               assetsPath_ (std::move (other.assetsPath_)),
                                               isInitialized_ (other.isInitialized_) {
      other.isInitialized_ = false;
      if (logger_) {
        logger_->infoStream () << libName_ << " move constructed";
      }
    }
    DotNameLib& operator= (DotNameLib&& other) noexcept {
      if (this != &other) {
        logger_ = std::move (other.logger_);
        assetsPath_ = std::move (other.assetsPath_);
        isInitialized_ = other.isInitialized_;
        other.isInitialized_ = false;
        if (logger_) {
          logger_->infoStream () << libName_ << " move assigned";
        }
      }
      return *this;
    }

    ~DotNameLib () {
      if (isInitialized_) {
        logger_->infoStream () << libName_ << " destructed";
      } else {
        logger_->infoStream () << libName_ << " (not initialized) destructed";
      }
    }

    // Public interface
    [[nodiscard]] bool isInitialized () const noexcept {
      return isInitialized_;
    }

    [[nodiscard]] const std::filesystem::path& getAssetsPath () const noexcept {
      return assetsPath_;
    }
  };
} // namespace dotnamecpp::v1
