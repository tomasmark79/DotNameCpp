// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include <DotNameLib/DotNameLib.hpp>
#include <Assets/AssetContext.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

namespace dotname {

  DotNameLib::DotNameLib () {
    LOG_D_STREAM << libName_ << " constructed ..." << std::endl;
  }

  DotNameLib::DotNameLib (const std::filesystem::path& assetsPath) : DotNameLib () {
    if (!assetsPath.empty ()) {
      AssetContext::setAssetsPath (assetsPath);
      LOG_D_STREAM << "Assets: " << AssetContext::getAssetsPath () << std::endl;
      LOG_I_STREAM << DotNameUtils::JsonUtils::getCustomStringSign () << std::endl;
      auto logo = std::ifstream (AssetContext::getAssetsPath () / "DotNameLogoV2.svg");
      LOG_D_STREAM << "path: " << (AssetContext::getAssetsPath () / "DotNameLogoV2.svg")
                   << std::endl;
    }
  }

  DotNameLib::~DotNameLib () {
    LOG_D_STREAM << libName_ << " ... destructed" << std::endl;
  }

} // namespace dotname