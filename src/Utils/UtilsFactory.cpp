#include "UtilsFactory.hpp"
#include "Filesystem/DirectoryManager.hpp"
#include "Filesystem/FileReader.hpp"
#include "Filesystem/FileWriter.hpp"
#include "Filesystem/PathResolver.hpp"
#include "Json/CustomStringsLoader.hpp"
#include "Json/JsonSerializer.hpp"
#include "Platform/PlatformInfoFactory.hpp"
#include "String/StringFormatter.hpp"

namespace dotnamecpp {
  namespace utils {

    // ========================================================================
    // Filesystem factories
    // ========================================================================

    std::shared_ptr<IFileReader> UtilsFactory::createFileReader () {
      return std::make_shared<FileReader> ();
    }

    std::shared_ptr<IFileWriter> UtilsFactory::createFileWriter () {
      return std::make_shared<FileWriter> ();
    }

    std::shared_ptr<IPathResolver> UtilsFactory::createPathResolver () {
      return std::make_shared<PathResolver> ();
    }

    std::shared_ptr<IDirectoryManager> UtilsFactory::createDirectoryManager () {
      return std::make_shared<DirectoryManager> ();
    }

    // ========================================================================
    // Platform factories
    // ========================================================================

    std::unique_ptr<IPlatformInfo> UtilsFactory::createPlatformInfo () {
      return PlatformInfoFactory::createForCurrentPlatform ();
    }

    std::unique_ptr<IPlatformInfo> UtilsFactory::createPlatformInfo (Platform platform) {
      return PlatformInfoFactory::create (platform);
    }

    // ========================================================================
    // JSON factories
    // ========================================================================

    std::shared_ptr<IJsonSerializer> UtilsFactory::createJsonSerializer () {
      auto fileReader = createFileReader ();
      auto fileWriter = createFileWriter ();
      return std::make_shared<JsonSerializer> (fileReader, fileWriter);
    }

    std::shared_ptr<ICustomStringsLoader> UtilsFactory::createCustomStringsLoader (
        std::shared_ptr<IAssetManager> assetManager, const std::string& filename) {
      if (!assetManager) {
        throw std::invalid_argument (
            "UtilsFactory::createCustomStringsLoader requires valid asset manager");
      }

      auto jsonSerializer = createJsonSerializer ();
      return std::make_shared<CustomStringsLoader> (assetManager, jsonSerializer, filename);
    }

    // ========================================================================
    // String factories
    // ========================================================================

    std::shared_ptr<IStringFormatter> UtilsFactory::createStringFormatter () {
      return std::make_shared<StringFormatter> ();
    }

    // ========================================================================
    // Convenience: Create complete utility set
    // ========================================================================

    UtilsFactory::UtilsBundle UtilsFactory::createBundle () {
      return UtilsBundle{
        .fileReader = createFileReader (),
        .fileWriter = createFileWriter (),
        .pathResolver = createPathResolver (),
        .directoryManager = createDirectoryManager (),
        .platformInfo = createPlatformInfo (),
        .jsonSerializer = createJsonSerializer (),
        .stringFormatter = createStringFormatter (),
      };
    }

  } // namespace utils
} // namespace dotnamecpp
