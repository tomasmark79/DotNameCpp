#include <DotNameLib/DotNameLib.hpp>

namespace dotnamecpp::v1 {

  DotNameLib::DotNameLib(std::shared_ptr<logging::ILogger> logger,
                         std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager)
      : logger_(logger ? std::move(logger) : std::make_shared<dotnamecpp::logging::NullLogger>()),
        assetManager_(std::move(assetManager)) {

    if (!assetManager_ || !assetManager_->validate()) {
      logger_->errorStream() << "Invalid or missing asset manager";
      return;
    }
    logger_->infoStream() << libName_ << " initialized ...";
    isInitialized_ = true;
  }

  DotNameLib::~DotNameLib() {
    if (isInitialized_) {
      logger_->infoStream() << libName_ << " ... destructed";
    } else {
      logger_->infoStream() << libName_ << " ... (not initialized) destructed";
    }
  }

  bool DotNameLib::isInitialized() const noexcept { return isInitialized_; }

  const std::shared_ptr<dotnamecpp::assets::IAssetManager> &
      DotNameLib::getAssetManager() const noexcept {
    return assetManager_;
  }

} // namespace dotnamecpp::v1
