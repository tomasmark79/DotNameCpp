#pragma once

#include <DotNameLib/version.h> // first configuration will create this file
#include <Logger/ILogger.hpp>
#include <Logger/NullLogger.hpp>
#include <Assets/AssetManagerFactory.hpp>
#include <Utils/Utils.hpp>
#include <filesystem>
#include <memory>
#include <string>

namespace dotnamecpp::v1 {
  class DotNameLib {

  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<IAssetManager> assetManager_;
    std::filesystem::path assetsPath_;
    bool isInitialized_ = false;

  public:
    // Nový konstruktor s asset managerem
    DotNameLib (std::shared_ptr<logging::ILogger> logger,
        std::shared_ptr<IAssetManager> assetManager)
        : logger_ (logger ? std::move (logger) : std::make_shared<logging::NullLogger> ()),
          assetManager_ (std::move (assetManager)) {
      if (assetManager_ && assetManager_->validate ()) {
        logger_->infoStream () << "DotNameLib initialized";
        logger_->infoStream () << "Assets: " << assetManager_->getAssetsPath ();
        logger_->debugStream () << "Assets: " << assetManager_->getAssetsPath ();

        const auto logoPath = assetManager_->resolveAsset ("DotNameCppLogo.svg");
        if (assetManager_->assetExists ("DotNameCppLogo.svg")) {
          logger_->debugStream () << "Logo: " << logoPath;
        } else {
          logger_->warningStream () << "Logo not found: " << logoPath;
        }
      } else {
        logger_->errorStream () << "Invalid or missing asset manager";
      }
    }

    // Nová metoda pro získání asset manageru
    [[nodiscard]]
    std::shared_ptr<IAssetManager> getAssetManager () const noexcept {
      return assetManager_;
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
