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
      isInitialized_ = true;
    } else {
      logger_->errorStream() << "Invalid or missing asset manager";
      isInitialized_ = false;
    }
  }

  DotNameLib::~DotNameLib() {
    stopWorker();

    if (isInitialized_) {
      logger_->infoStream() << libName_ << " destructed";
    } else {
      logger_->infoStream() << libName_ << " (not initialized) destructed";
    }
  }

  DotNameLib::DotNameLib(DotNameLib &&other) noexcept
      : logger_(std::move(other.logger_)), assetManager_(std::move(other.assetManager_)),
        assetsPath_(std::move(other.assetsPath_)), isInitialized_(other.isInitialized_),
        shouldStop_(other.shouldStop_.load()), workerThread_(std::move(other.workerThread_)) {
    other.isInitialized_ = false;
    other.shouldStop_.store(false);
    if (logger_) {
      logger_->infoStream() << libName_ << " move constructed";
    }
  }

  DotNameLib &DotNameLib::operator=(DotNameLib &&other) noexcept {
    if (this != &other) {
      stopWorker();

      // Move all members
      logger_ = std::move(other.logger_);
      assetManager_ = std::move(other.assetManager_);
      assetsPath_ = std::move(other.assetsPath_);
      isInitialized_ = other.isInitialized_;
      shouldStop_.store(other.shouldStop_.load());
      workerThread_ = std::move(other.workerThread_);

      other.isInitialized_ = false;
      other.shouldStop_.store(false);
      if (logger_) {
        logger_->infoStream() << libName_ << " move assigned";
      }
    }
    return *this;
  }

  bool DotNameLib::isInitialized() const noexcept { return isInitialized_; }

  const std::filesystem::path &DotNameLib::getAssetsPath() const noexcept { return assetsPath_; }

  bool DotNameLib::run(int durationSeconds) {
    if (!isInitialized_) {
      logger_->errorStream() << "Cannot run: " << libName_ << " is not initialized";
      return false;
    }

    // Reset stop flag
    shouldStop_.store(false);

    try {
      logger_->infoStream() << libName_ << " started successfully";

      // ┌──────────────────────────────────────────────────────────────────┐
      // │ PLACE YOUR WORKER THREAD LOGIC HERE                              │
      // ├──────────────────────────────────────────────────────────────────┤
      // │ Example: Launch a worker thread that performs periodic tasks     │
      // └──────────────────────────────────────────────────────────────────┘

      workerThread_ = std::thread([this]() {
        while (!shouldStop_.load()) {

          // Simulate work
          logger_->debugStream() << "Worker thread doing work...";

          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      });

      // Run for specified duration
      if (durationSeconds > 0) {
        logger_->infoStream() << "Running for " << durationSeconds << " seconds...";
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

      stopWorker();
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

    logger_->infoStream() << "Stopping " << libName_ << "...";
    stopWorker();
  }

  void DotNameLib::stopWorker() {
    shouldStop_.store(true);
    if (workerThread_.joinable()) {
      workerThread_.join();
    }
  }

} // namespace dotnamecpp::v1
