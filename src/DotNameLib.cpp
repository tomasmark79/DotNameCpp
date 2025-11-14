#include <DotNameLib/DotNameLib.hpp>
#include <Utils/Logger/NullLogger.hpp> // for fallback logger

namespace dotnamecpp::v1 {

  // Constructor
  DotNameLib::DotNameLib (std::shared_ptr<logging::ILogger> logger,
      std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager)
      : logger_ (
            logger ? std::move (logger) : std::make_shared<dotnamecpp::logging::NullLogger> ()),
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

  // Destructor
  DotNameLib::~DotNameLib () {
    if (isInitialized_) {
      logger_->infoStream () << libName_ << " destructed";
    } else {
      logger_->infoStream () << libName_ << " (not initialized) destructed";
    }
  }

  // Move constructor
  DotNameLib::DotNameLib (DotNameLib&& other) noexcept
      : logger_ (std::move (other.logger_)),
        assetManager_ (std::move (other.assetManager_)),
        assetsPath_ (std::move (other.assetsPath_)),
        isInitialized_ (other.isInitialized_) {
    other.isInitialized_ = false;
    if (logger_) {
      logger_->infoStream () << libName_ << " move constructed";
    }
  }

  // Move assignment operator
  DotNameLib& DotNameLib::operator= (DotNameLib&& other) noexcept {
    if (this != &other) {
      logger_ = std::move (other.logger_);
      assetManager_ = std::move (other.assetManager_);
      assetsPath_ = std::move (other.assetsPath_);
      isInitialized_ = other.isInitialized_;
      other.isInitialized_ = false;
      if (logger_) {
        logger_->infoStream () << libName_ << " move assigned";
      }
    }
    return *this;
  }

  // Public methods
  bool DotNameLib::isInitialized () const noexcept {
    return isInitialized_;
  }

  const std::filesystem::path& DotNameLib::getAssetsPath () const noexcept {
    return assetsPath_;
  }

  // Private methods
  std::shared_ptr<dotnamecpp::assets::IAssetManager> DotNameLib::getAssetManager () const noexcept {
    return assetManager_;
  }

} // namespace dotnamecpp::v1