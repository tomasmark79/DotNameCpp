// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include <DotNameLib/DotNameLib.hpp>
#include <Assets/AssetContext.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#if defined(__EMSCRIPTEN__)
  #include <emscripten/emscripten.h>
#endif

namespace dotname {

  DotNameLib::DotNameLib () {
    LOG_D_STREAM << libName_ << " constructed ..." << "\n";
  }

  DotNameLib::DotNameLib (const std::filesystem::path& assetsPath) : DotNameLib () {
    if (!assetsPath.empty ()) {
      AssetContext::setAssetsPath (assetsPath);
      LOG_D_STREAM << "Assets: " << AssetContext::getAssetsPath () << "\n";
      LOG_I_STREAM << DotNameUtils::JsonUtils::getCustomStringSign () << "\n";
      auto logo = std::ifstream (AssetContext::getAssetsPath () / "DotNameLogoV2.svg");
      LOG_D_STREAM << "path: " << (AssetContext::getAssetsPath () / "DotNameLogoV2.svg") << "\n";
    }
  }

  DotNameLib::~DotNameLib () {
    LOG_D_STREAM << libName_ << " ... destructed" << "\n";
  }

} // namespace dotname