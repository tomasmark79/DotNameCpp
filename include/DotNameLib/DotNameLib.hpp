#pragma once

#include <DotNameLib/version.h> // first configuration will create this file
#include <Utils/UtilsFactory.hpp>
#include <atomic>
#include <filesystem>
#include <memory>
#include <string>
#include <thread>

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

    /**
     * @brief Run your business logic
     *
     * This is the main entry point for your library's functionality.
     *
     * @param durationSeconds Duration to run in seconds (0 = run indefinitely)
     * @return true if successful
     * @return false if an error occurred
     */
    bool run(int durationSeconds = 0);

    /**
     * @brief Stop all running processes
     */
    void stop();

  private:
    /**
     * @brief Internal worker thread stop helper (no logging, no checks)
     */
    void stopWorker();

    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
    std::filesystem::path assetsPath_;
    bool isInitialized_ = false;
    std::atomic<bool> shouldStop_{false};
    std::thread workerThread_;
  };

} // namespace dotnamecpp::v1
