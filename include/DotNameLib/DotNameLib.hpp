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
     * Automatically stops any running worker threads
     */
    ~DotNameLib();

    /**
     * @brief Copy construction is deleted
     */
    DotNameLib(const DotNameLib &other) = delete;

    /**
     * @brief Copy assignment is deleted
     */
    DotNameLib &operator=(const DotNameLib &other) = delete;

    /**
     * @brief Move constructor
     *
     * @param other Object to move from
     */
    DotNameLib(DotNameLib &&other) noexcept;

    /**
     * @brief Move assignment operator
     *
     * @param other Object to move from
     * @return Reference to this object
     */
    DotNameLib &operator=(DotNameLib &&other) noexcept;

    // ============================================================================
    // Main API
    // ============================================================================

    /**
     * @brief Run your business logic
     *
     * This is the main entry point for your library's functionality.
     * Starts a worker thread and runs for the specified duration.
     *
     * @param durationSeconds Duration to run in seconds (0 = run indefinitely)
     * @return true if successful
     * @return false if an error occurred or not initialized
     */
    bool run(int durationSeconds = 0);

    /**
     * @brief Stop all running processes
     *
     * Signals the worker thread to stop. Thread will be joined by run() or destructor.
     */
    void stop();

    // ============================================================================
    // Query Methods
    // ============================================================================

    /**
     * @brief Check if the library is properly initialized
     *
     * @return true if initialized and ready to use
     * @return false if initialization failed
     */
    [[nodiscard]]
    bool isInitialized() const noexcept;

    /**
     * @brief Get the assets directory path
     *
     * @return const std::filesystem::path& Path to assets
     */
    [[nodiscard]]
    const std::filesystem::path &getAssetsPath() const noexcept;

  private:
    /**
     * @brief Internal worker thread stop helper
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
