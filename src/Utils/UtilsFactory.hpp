#pragma once

#include <Utils/Filesystem/IDirectoryManager.hpp>
#include <Utils/Filesystem/IFileReader.hpp>
#include <Utils/Filesystem/IFileWriter.hpp>
#include <Utils/Filesystem/IPathResolver.hpp>
#include <Utils/Json/ICustomStringsLoader.hpp>
#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Platform/IPlatformInfo.hpp>
#include <Utils/String/IStringFormatter.hpp>
#include <Assets/IAssetManager.hpp>
#include <memory>

namespace dotnamecpp::utils {

  // Import asset types
  using dotnamecpp::assets::IAssetManager;

  /**
  * @brief Factory for creating all utility component instances
  * 
  * Provides centralized creation and dependency injection for all Utils subsystems.
  * Creates properly initialized instances with all required dependencies.
  */
  class UtilsFactory {
  public:
    // ========================================================================
    // Filesystem factories
    // ========================================================================

    /**
     * @brief Create file reader instance
     * @return Shared pointer to IFileReader implementation
     */
    [[nodiscard]]
    static std::shared_ptr<IFileReader> createFileReader ();

    /**
     * @brief Create file writer instance
     * @return Shared pointer to IFileWriter implementation
     */
    [[nodiscard]]
    static std::shared_ptr<IFileWriter> createFileWriter ();

    /**
     * @brief Create path resolver instance
     * @return Shared pointer to IPathResolver implementation
     */
    [[nodiscard]]
    static std::shared_ptr<IPathResolver> createPathResolver ();

    /**
     * @brief Create directory manager instance
     * @return Shared pointer to IDirectoryManager implementation
     */
    [[nodiscard]]
    static std::shared_ptr<IDirectoryManager> createDirectoryManager ();

    // ========================================================================
    // Platform factories
    // ========================================================================

    /**
     * @brief Create platform info for current platform
     * @return Unique pointer to IPlatformInfo implementation
     */
    [[nodiscard]]
    static std::unique_ptr<IPlatformInfo> createPlatformInfo ();

    /**
     * @brief Create platform info for specific platform
     * @param platform Target platform
     * @return Unique pointer to IPlatformInfo implementation
     */
    [[nodiscard]]
    static std::unique_ptr<IPlatformInfo> createPlatformInfo (Platform platform);

    // ========================================================================
    // JSON factories
    // ========================================================================

    /**
     * @brief Create JSON serializer with file I/O dependencies
     * @return Shared pointer to IJsonSerializer implementation
     */
    [[nodiscard]]
    static std::shared_ptr<IJsonSerializer> createJsonSerializer ();

    /**
     * @brief Create custom strings loader with all dependencies
     * @param assetManager Asset manager for resolving asset paths
     * @param filename Custom strings filename (default: "customstrings.json")
     * @return Shared pointer to ICustomStringsLoader implementation
     */
    [[nodiscard]]
    static std::shared_ptr<ICustomStringsLoader> createCustomStringsLoader (
        std::shared_ptr<IAssetManager> assetManager,
        const std::string& filename = "customstrings.json");

    // ========================================================================
    // String factories
    // ========================================================================

    /**
     * @brief Create string formatter instance
     * @return Shared pointer to IStringFormatter implementation
     */
    [[nodiscard]]
    static std::shared_ptr<IStringFormatter> createStringFormatter ();

    // ========================================================================
    // Convenience: Create complete utility set
    // ========================================================================

    /**
     * @brief Utility bundle containing all commonly used components
     */
    struct UtilsBundle {
      std::shared_ptr<IFileReader> fileReader;
      std::shared_ptr<IFileWriter> fileWriter;
      std::shared_ptr<IPathResolver> pathResolver;
      std::shared_ptr<IDirectoryManager> directoryManager;
      std::unique_ptr<IPlatformInfo> platformInfo;
      std::shared_ptr<IJsonSerializer> jsonSerializer;
      std::shared_ptr<IStringFormatter> stringFormatter;
    };

    /**
     * @brief Create complete utility bundle with all components
     * @return UtilsBundle with all utility instances
     */
    [[nodiscard]]
    static UtilsBundle createBundle ();
  };

} // namespace dotnamecpp::utils
