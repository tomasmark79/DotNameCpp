#pragma once

#include <DotNameLib/version.h> // cmake configuration will generate this file
#include <Utils/UtilsFactory.hpp>

#include <atomic>
#include <memory>
#include <string>

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
     * Automatically ensures graceful shutdown
     */
    ~DotNameLib();

    /**
     * @brief Copy and move operations are deleted
     *
     * This class manages unique resources
     * and should not be copied or moved.
     */
    DotNameLib(const DotNameLib &other) = delete;
    DotNameLib &operator=(const DotNameLib &other) = delete;
    DotNameLib(DotNameLib &&other) = delete;
    DotNameLib &operator=(DotNameLib &&other) = delete;

    // ============================================================================
    // Main API
    // ============================================================================

    /**
     * @brief Run your business logic
     *
     * This is the main entry point for your library's functionality.
     * Runs synchronously in the calling thread.
     *
     * @param durationSeconds Duration to run in seconds (0 = run indefinitely)
     * @return true if successful
     * @return false if an error occurred or not initialized
     */
    bool run(int durationSeconds = 0);

    /**
     * @brief Stop all running processes
     *
     * Signals to stop running processes. Safe to call from another thread.
     */
    void stop();

    // ============================================================================
    // Query Methods
    // ============================================================================

    /**
     * @brief Check if the library is initialized
     *
     * @return true
     * @return false
     */
    [[nodiscard]]
    bool isInitialized() const noexcept;

    /**
     * @brief Get the Asset Manager object
     *
     * @return const std::shared_ptr<dotnamecpp::assets::IAssetManager>&
     */
    [[nodiscard]]
    const std::shared_ptr<dotnamecpp::assets::IAssetManager> &getAssetManager() const noexcept;

  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
    std::shared_ptr<dotnamecpp::logging::ILogger> logger_;
    std::shared_ptr<dotnamecpp::assets::IAssetManager> assetManager_;
    bool isInitialized_ = false;
    std::atomic<bool> shouldStop_{false};
  };

} // namespace dotnamecpp::v1
