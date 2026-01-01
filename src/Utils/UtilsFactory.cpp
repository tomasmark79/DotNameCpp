#include "UtilsFactory.hpp"
#include "Assets/AssetManagerFactory.hpp"
#include "Filesystem/DirectoryManager.hpp"
#include "Filesystem/FileReader.hpp"
#include "Filesystem/FileWriter.hpp"
#include "Filesystem/PathResolver.hpp"
#include "Logger/LoggerFactory.hpp"
#include "Platform/PlatformInfoFactory.hpp"
#include "String/StringFormatter.hpp"
#include "Utils/Platform/IPlatformInfo.hpp"
#include "Json/CustomStringsLoader.hpp"
#include "Json/JsonSerializer.hpp"

namespace dotnamecpp::utils {

  // Filesystem factories
  std::shared_ptr<IFileReader> UtilsFactory::createFileReader() {
    return std::make_shared<FileReader>();
  }

  std::shared_ptr<IFileWriter> UtilsFactory::createFileWriter() {
    return std::make_shared<FileWriter>();
  }

  std::shared_ptr<IPathResolver> UtilsFactory::createPathResolver() {
    return std::make_shared<PathResolver>();
  }

  std::shared_ptr<IDirectoryManager> UtilsFactory::createDirectoryManager() {
    return std::make_shared<DirectoryManager>();
  }

  // Platform factories
  std::unique_ptr<IPlatformInfo> UtilsFactory::createPlatformInfo() {
    return PlatformInfoFactory::createForCurrentPlatform();
  }

  std::unique_ptr<IPlatformInfo> UtilsFactory::createPlatformInfo(Platform platform) {
    return PlatformInfoFactory::create(platform);
  }

  // Assets factories
  std::shared_ptr<IAssetManager>
      UtilsFactory::createAssetManager(const std::filesystem::path &executablePath,
                                       const std::string &appName) {
    return dotnamecpp::assets::AssetManagerFactory::createDefault(executablePath, appName);
  }

  // Json factories
  std::shared_ptr<IJsonSerializer> UtilsFactory::createJsonSerializer() {
    auto fileReader = createFileReader();
    auto fileWriter = createFileWriter();
    return std::make_shared<JsonSerializer>(fileReader, fileWriter);
  }

  // Custom strings loader factory
  std::shared_ptr<ICustomStringsLoader>
      UtilsFactory::createCustomStringsLoader(const std::filesystem::path &executablePath,
                                              const std::string &appName) {
    auto assetManager = createAssetManager(executablePath, appName);
    const std::string filename = "customstrings.json";
    auto jsonSerializer = createJsonSerializer();
    return std::make_shared<CustomStringsLoader>(assetManager, jsonSerializer, filename);
  }

  // String factories
  std::shared_ptr<IStringFormatter> UtilsFactory::createStringFormatter() {
    return std::make_shared<StringFormatter>();
  }

  // Logger factories
  std::shared_ptr<ILogger> UtilsFactory::createLogger(LoggerType type, const LoggerConfig &config) {
    return dotnamecpp::logging::LoggerFactory::create(type, config);
  }

  std::shared_ptr<ILogger> UtilsFactory::createDefaultLogger() {
    LoggerConfig config{.level = dotnamecpp::logging::Level::LOG_INFO,
                        .enableFileLogging = false,
                        .logFilePath = "",
                        .colorOutput = true,
                        .appPrefix = ""};
    return createLogger(LoggerType::Console, config);
  }

  // Create complete application components
  UtilsFactory::AppComponents UtilsFactory::createAppComponents(const std::string &appName,
                                                                const LoggerConfig &loggerConfig) {
    // Create platform info to get executable path
    auto platformInfo = createPlatformInfo();
    auto execPathResult = platformInfo->getExecutablePath();
    if (!execPathResult.hasValue()) {
      throw std::runtime_error("Failed to get executable path: " +
                               execPathResult.error().toString());
    }

    auto logger = createLogger(LoggerType::Console, loggerConfig);
    auto assetManager = createAssetManager(execPathResult.value(), appName);
    auto customStringsLoader = createCustomStringsLoader(execPathResult.value(), appName);
    return AppComponents{.logger = std::move(logger),
                         .assetManager = std::move(assetManager),
                         .platformInfo = std::move(platformInfo),
                         .customStringsLoader = std::move(customStringsLoader)};
  }
  // Convenience: create a bundle of common utils
  UtilsFactory::UtilsBundle UtilsFactory::createBundle() {
    return UtilsBundle{.fileReader = createFileReader(),
                       .fileWriter = createFileWriter(),
                       .pathResolver = createPathResolver(),
                       .directoryManager = createDirectoryManager(),
                       .platformInfo = createPlatformInfo(),
                       .jsonSerializer = createJsonSerializer(),
                       .stringFormatter = createStringFormatter(),
                       .logger = createDefaultLogger()};
  }

} // namespace dotnamecpp::utils
