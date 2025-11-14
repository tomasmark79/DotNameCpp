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
     * @brief Create a File Reader object
     * 
     * @return std::shared_ptr<IFileReader> 
     */
    [[nodiscard]]
    static std::shared_ptr<IFileReader> createFileReader ();

    /**
     * @brief Create a File Writer object
     * 
     * @return std::shared_ptr<IFileWriter> 
     */
    [[nodiscard]]
    static std::shared_ptr<IFileWriter> createFileWriter ();

    /**
     * @brief Create a Path Resolver object
     * 
     * @return std::shared_ptr<IPathResolver> 
     */
    [[nodiscard]]
    static std::shared_ptr<IPathResolver> createPathResolver ();

    /**
     * @brief Create a Directory Manager object
     * 
     * @return std::shared_ptr<IDirectoryManager> 
     */
    [[nodiscard]]
    static std::shared_ptr<IDirectoryManager> createDirectoryManager ();

    // ========================================================================
    // Platform factories
    // ========================================================================

    /**
     * @brief Create a Platform Info object
     * 
     * @return std::unique_ptr<IPlatformInfo> 
     */
    [[nodiscard]]
    static std::unique_ptr<IPlatformInfo> createPlatformInfo ();
    
    /**
     * @brief Create a Platform Info object
     * 
     * @param platform 
     * @return std::unique_ptr<IPlatformInfo> 
     */
    [[nodiscard]]
    static std::unique_ptr<IPlatformInfo> createPlatformInfo (Platform platform);

    // ========================================================================
    // JSON factories
    // ========================================================================

    /**
     * @brief Create a Json Serializer object
     * 
     * @return std::shared_ptr<IJsonSerializer> 
     */
    [[nodiscard]]
    static std::shared_ptr<IJsonSerializer> createJsonSerializer ();

    /**
     * @brief Create a Custom Strings Loader object
     * 
     * @param assetManager 
     * @param filename 
     * @return std::shared_ptr<ICustomStringsLoader> 
     */
    [[nodiscard]]
    static std::shared_ptr<ICustomStringsLoader> createCustomStringsLoader (
        std::shared_ptr<IAssetManager> assetManager,
        const std::string& filename = "customstrings.json");

    // ========================================================================
    // String factories
    // ========================================================================

    /**
     * @brief Create a String Formatter object
     * 
     * @return std::shared_ptr<IStringFormatter> 
     */
    [[nodiscard]]
    static std::shared_ptr<IStringFormatter> createStringFormatter ();

    // ========================================================================
    // Convenience: Create complete utility set
    // ========================================================================

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
     * @brief Create a Bundle object
     * 
     * @return UtilsBundle 
     */
    [[nodiscard]]
    static UtilsBundle createBundle ();
  };

} // namespace dotnamecpp::utils
