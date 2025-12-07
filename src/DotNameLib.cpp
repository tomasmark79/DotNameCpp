#include <DotNameLib/DotNameLib.hpp>

namespace dotnamecpp::v1 {

  DotNameLib::DotNameLib(std::shared_ptr<logging::ILogger> logger,
                         std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager)
      : logger_(logger ? std::move(logger) : std::make_shared<dotnamecpp::logging::NullLogger>()),
        assetManager_(std::move(assetManager)) {

    if (assetManager_ && assetManager_->validate()) {
      logger_->infoStream() << libName_ << " initialized ...";
      const auto logoPath = assetManager_->resolveAsset("DotNameCppLogo.svg");
      if (assetManager_->assetExists("DotNameCppLogo.svg")) {
        logger_->debugStream() << "Logo: " << logoPath << " found";
      } else {
        logger_->warningStream() << "Logo not found: " << logoPath;
      }
    } else {
      logger_->errorStream() << "Invalid or missing asset manager";
    }
  }

  DotNameLib::~DotNameLib() {
    if (isInitialized_) {
      logger_->infoStream() << libName_ << " destructed";
    } else {
      logger_->infoStream() << libName_ << " (not initialized) destructed";
    }
  }

  DotNameLib::DotNameLib(DotNameLib &&other) noexcept
      : logger_(std::move(other.logger_)), assetManager_(std::move(other.assetManager_)),
        assetsPath_(std::move(other.assetsPath_)), isInitialized_(other.isInitialized_) {
    other.isInitialized_ = false;
    if (logger_) {
      logger_->infoStream() << libName_ << " move constructed";
    }
  }

  DotNameLib &DotNameLib::operator=(DotNameLib &&other) noexcept {
    if (this != &other) {
      logger_ = std::move(other.logger_);
      assetManager_ = std::move(other.assetManager_);
      assetsPath_ = std::move(other.assetsPath_);
      isInitialized_ = other.isInitialized_;
      other.isInitialized_ = false;
      if (logger_) {
        logger_->infoStream() << libName_ << " move assigned";
      }
    }
    return *this;
  }

  bool DotNameLib::isInitialized() const noexcept { return isInitialized_; }

  const std::filesystem::path &DotNameLib::getAssetsPath() const noexcept { return assetsPath_; }

} // namespace dotnamecpp::v1
