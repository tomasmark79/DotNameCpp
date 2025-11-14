#pragma once

#include <Utils/Filesystem/IDirectoryManager.hpp>
#include <Utils/Filesystem/IFileReader.hpp>
#include <Utils/Filesystem/IFileWriter.hpp>
#include <Utils/Filesystem/IPathResolver.hpp>
#include <Utils/Json/ICustomStringsLoader.hpp>
#include <Utils/Json/IJsonSerializer.hpp>
#include <Utils/Logger/ILogger.hpp>
#include <Utils/Logger/LoggerFactory.hpp>
#include <Utils/Platform/IPlatformInfo.hpp>
#include <Utils/String/IStringFormatter.hpp>
#include <Utils/Assets/IAssetManager.hpp>
#include <memory>

namespace dotnamecpp::utils {

  // Import types from other namespaces
  using dotnamecpp::assets::IAssetManager;
  using dotnamecpp::logging::ILogger;
  using dotnamecpp::logging::LoggerConfig;
  using dotnamecpp::logging::LoggerType;

  class UtilsFactory {
  public:
    // Filesystem factories
    [[nodiscard]] static std::shared_ptr<IFileReader> createFileReader ();
    [[nodiscard]] static std::shared_ptr<IFileWriter> createFileWriter ();
    [[nodiscard]] static std::shared_ptr<IPathResolver> createPathResolver ();
    [[nodiscard]] static std::shared_ptr<IDirectoryManager> createDirectoryManager ();

    // Platform factories
    [[nodiscard]] static std::unique_ptr<IPlatformInfo> createPlatformInfo ();
    [[nodiscard]] static std::unique_ptr<IPlatformInfo> createPlatformInfo (Platform platform);

    // Assets factories
    [[nodiscard]] static std::shared_ptr<IAssetManager> createAssetManager (
        const std::filesystem::path& executablePath, const std::string& appName);

    // JSON factories
    [[nodiscard]] static std::shared_ptr<IJsonSerializer> createJsonSerializer ();
    [[nodiscard]] static std::shared_ptr<ICustomStringsLoader> createCustomStringsLoader (
        std::shared_ptr<IAssetManager> assetManager,
        const std::string& filename = "customstrings.json");

    // String factories
    [[nodiscard]] static std::shared_ptr<IStringFormatter> createStringFormatter ();

    // Logger factories
    [[nodiscard]] static std::shared_ptr<ILogger> createLogger (LoggerType type, const LoggerConfig& config);
    [[nodiscard]] static std::shared_ptr<ILogger> createDefaultLogger ();

    // Convenience: Create complete utility set
    struct UtilsBundle {
      std::shared_ptr<IFileReader> fileReader;
      std::shared_ptr<IFileWriter> fileWriter;
      std::shared_ptr<IPathResolver> pathResolver;
      std::shared_ptr<IDirectoryManager> directoryManager;
      std::unique_ptr<IPlatformInfo> platformInfo;
      std::shared_ptr<IJsonSerializer> jsonSerializer;
      std::shared_ptr<IStringFormatter> stringFormatter;
      std::shared_ptr<ILogger> logger;
    };

    [[nodiscard]] static UtilsBundle createBundle ();
  };

} // namespace dotnamecpp::utils
