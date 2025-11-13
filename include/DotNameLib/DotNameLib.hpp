#pragma once

#include <DotNameLib/version.h> // first configuration will create this file
#include <Logger/ILogger.hpp>
#include <Assets/AssetContext.hpp>
#include <Utils/Utils.hpp>
#include <filesystem>
#include <memory>
#include <string>

namespace dotnamecpp::v1 {
  class DotNameLib {

  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::filesystem::path assetsPath_;
    bool isInitialized_ = false;

  public:
    explicit DotNameLib (std::shared_ptr<dotnamecpp::logging::ILogger> logger,
        const std::filesystem::path& assetsPath)
        : logger_ (std::move (logger)), assetsPath_ (assetsPath) {

      if (assetsPath.empty ()) {
        logger_->warning ("Empty assets path provided");
        return;
      }
      try {
        assetsPath_ = assetsPath;
        AssetContext::setAssetsPath (assetsPath);
        logger_->infoStream () << libName_ << " initialized with assets path: "
                               << AssetContext::getAssetsPath ();
        logger_->debugStream () << "Assets: " << AssetContext::getAssetsPath ();
        logger_->debugStream () << DotNameUtils::json::getCustomStringSign ();

        // Check if logo file exists before trying to open it
        const auto logoPath = AssetContext::getAssetsPath () / "DotNameCppLogo.svg";
        logger_->debugStream () << "Logo path: " << logoPath;

        if (std::filesystem::exists (logoPath)) {
          std::ifstream logoFile (logoPath);
          if (logoFile.is_open ()) {
            logger_->debugStream () << "Logo file successfully opened";
            isInitialized_ = true;
          } else {
            logger_->warningStream () << "Could not open logo file: " << logoPath;
          }
        } else {
          logger_->warningStream () << "Logo file does not exist: " << logoPath;
        }
      } catch (const std::exception& e) {
        logger_->errorStream () << "Error initializing DotNameLib: " << e.what ();
        isInitialized_ = false;
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
