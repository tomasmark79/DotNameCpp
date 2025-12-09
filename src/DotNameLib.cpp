#include <DotNameLib/DotNameLib.hpp>
#include <thread>

namespace dotnamecpp::v1 {

  DotNameLib::DotNameLib(std::shared_ptr<logging::ILogger> logger,
                         std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager)
      : logger_(logger ? std::move(logger) : std::make_shared<dotnamecpp::logging::NullLogger>()),
        assetManager_(std::move(assetManager)) {

    if (!assetManager_ || !assetManager_->validate()) {
      logger_->errorStream() << "Invalid or missing asset manager";
      isInitialized_ = false;
      return;
    }

    // just an example of asset usage
    const auto logoPath = assetManager_->resolveAsset("DotNameCppLogo.svg");
    if (!assetManager_->assetExists("DotNameCppLogo.svg")) {
      logger_->warningStream() << "Logo not found: " << logoPath;
    }

    // ┌──────────────────────────────────────────────────────────────────┐
    // │ PLACE YOUR INITIALIZATION LOGIC HERE                             │
    // ├──────────────────────────────────────────────────────────────────┤
    // │ Example: Load configurations, initialize resources, etc.         │
    // └──────────────────────────────────────────────────────────────────┘

    isInitialized_ = true;
    logger_->infoStream() << libName_ << " initialized ...";
  }

  DotNameLib::~DotNameLib() {
    if (isInitialized_) {
      stop();
      logger_->infoStream() << libName_ << " ... destructed";
    } else {
      logger_->infoStream() << libName_ << " ... (not initialized) destructed";
    }
  }

  bool DotNameLib::run(int durationSeconds) {
    if (!isInitialized_) {
      logger_->errorStream() << "Cannot run: " << libName_ << " is not initialized";
      return false;
    }

    // Reset stop flag
    shouldStop_.store(false);

    try {
      // ┌──────────────────────────────────────────────────────────────────┐
      // │ PLACE YOUR BUSINESS LOGIC HERE                                   │
      // ├──────────────────────────────────────────────────────────────────┤
      // │ Example: Start services, process data, etc.                      │
      // └──────────────────────────────────────────────────────────────────┘

      // Run for specified duration
      if (durationSeconds > 0) {
        logger_->infoStream() << "MOCK BUSINESS LOGIC running for " << durationSeconds << " seconds ...";
        for (int i = 0; i < durationSeconds && !shouldStop_.load(); ++i) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        if (shouldStop_.load()) {
          logger_->infoStream() << libName_ << " stopped by user request";
        } else {
          logger_->infoStream() << libName_ << " finished after " << durationSeconds << " seconds";
        }
      } else {
        logger_->infoStream() << "Running indefinitely. Call stop() to terminate.";
        constexpr int pollIntervalMs = 100;
        while (!shouldStop_.load()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(pollIntervalMs));
        }
        logger_->infoStream() << libName_ << " stopped";
      }

      return true;
    } catch (const std::exception &e) {
      logger_->errorStream() << "Error running " << libName_ << ": " << e.what();
      return false;
    }
  }

  void DotNameLib::stop() {
    if (!isInitialized_) {
      logger_->warningStream() << "Cannot stop: " << libName_ << " is not initialized";
      return;
    }

    logger_->infoStream() << "Stopping " << libName_ << " ...";
    shouldStop_.store(true);

    // ┌──────────────────────────────────────────────────────────────────┐
    // │ PLACE YOUR CLEANUP LOGIC HERE                                    │
    // ├──────────────────────────────────────────────────────────────────┤
    // │ Example: Shutdown services, release resources, etc.              │
    // └──────────────────────────────────────────────────────────────────┘
  }

  bool DotNameLib::isInitialized() const noexcept { return isInitialized_; }
  const std::shared_ptr<dotnamecpp::assets::IAssetManager> &
      DotNameLib::getAssetManager() const noexcept {
    return assetManager_;
  }

} // namespace dotnamecpp::v1
