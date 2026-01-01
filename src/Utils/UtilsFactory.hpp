#pragma once

#include <DotNameLib/version.h>
#include <Utils/Assets/IAssetManager.hpp>
#include <Utils/Filesystem/IDirectoryManager.hpp>
#include <Utils/Filesystem/IFileReader.hpp>
#include <Utils/Filesystem/IFileWriter.hpp>
#include <Utils/Filesystem/IPathResolver.hpp>
#include <Utils/Json/ICustomStringsLoader.hpp>
#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Logger/ILogger.hpp>
#include <Utils/Logger/LoggerFactory.hpp>
#include <Utils/Logger/NullLogger.hpp>
#include <Utils/Platform/IPlatformInfo.hpp>
#include <Utils/String/IStringFormatter.hpp>
#include <filesystem>
#include <memory>

namespace dotnamecpp::utils {

  // Import types from other namespaces
  using dotnamecpp::assets::IAssetManager;
  using dotnamecpp::logging::ILogger;
  using dotnamecpp::logging::LoggerConfig;
  using dotnamecpp::logging::LoggerType;

  class UtilsFactory {
  public:
    [[nodiscard]]
    static std::shared_ptr<IFileReader> createFileReader();
    [[nodiscard]]
    static std::shared_ptr<IFileWriter> createFileWriter();
    [[nodiscard]]
    static std::shared_ptr<IPathResolver> createPathResolver();
    [[nodiscard]]
    static std::shared_ptr<IDirectoryManager> createDirectoryManager();
    [[nodiscard]]
    static std::unique_ptr<IPlatformInfo> createPlatformInfo();
    [[nodiscard]]
    static std::unique_ptr<IPlatformInfo> createPlatformInfo(Platform platform);
    [[nodiscard]]
    static std::shared_ptr<IAssetManager>
        createAssetManager(const std::filesystem::path &executablePath, const std::string &appName);
    [[nodiscard]]
    static std::shared_ptr<IJsonSerializer> createJsonSerializer();
    [[nodiscard]]
    static std::shared_ptr<ICustomStringsLoader>
        createCustomStringsLoader(const std::filesystem::path &executablePath,
                                  const std::string &appName);
    [[nodiscard]]
    static std::shared_ptr<IStringFormatter> createStringFormatter();
    [[nodiscard]]
    static std::shared_ptr<ILogger> createLogger(LoggerType type, const LoggerConfig &config);
    [[nodiscard]]
    static std::shared_ptr<ILogger> createDefaultLogger();

    // Unified application context
    struct ApplicationContext {

      // System components
      std::shared_ptr<ILogger> logger;
      std::unique_ptr<IPlatformInfo> platformInfo;

      // File system utilities
      std::shared_ptr<IFileReader> fileReader;
      std::shared_ptr<IFileWriter> fileWriter;
      std::shared_ptr<IPathResolver> pathResolver;
      std::shared_ptr<IDirectoryManager> directoryManager;

      // Serialization & formatting
      std::shared_ptr<IJsonSerializer> jsonSerializer;
      std::shared_ptr<IStringFormatter> stringFormatter;

      // Application-specific (nullable for flexibility)
      std::shared_ptr<IAssetManager> assetManager;
      std::shared_ptr<ICustomStringsLoader> customStringsLoader;
    };

    /**
     * @brief Create full application context with all components
     * @param appName Application name for asset resolution
     * @param loggerConfig Logger configuration
     * @return Complete ApplicationContext with all utilities initialized
     */
    [[nodiscard]]
    static ApplicationContext createFullContext(const std::string &appName,
                                                const LoggerConfig &loggerConfig);

    /**
     * @brief Create minimal context without app-specific components
     * @param loggerConfig Logger configuration
     * @return ApplicationContext with core utilities only (no assetManager/customStringsLoader)
     */
    [[nodiscard]]
    static ApplicationContext createCoreContext(const LoggerConfig &loggerConfig);

    /**
     * @brief Create basic utilities context with default logger
     * @return ApplicationContext with basic utilities and default logger
     */
    [[nodiscard]]
    static ApplicationContext createBasicContext();
  };

} // namespace dotnamecpp::utils
