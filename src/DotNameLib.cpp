#include <DotNameLib/DotNameLib.hpp>
#include <Assets/AssetContext.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>
#include <fstream>

#if defined(__EMSCRIPTEN__)
  #include <emscripten/emscripten.h>
#endif

namespace dotname {
  inline namespace v1 {

    DotNameLib::DotNameLib () : isInitialized_ (false) {
      LOG_D_STREAM << libName_ << " constructed ..." << "\n";
    }

    DotNameLib::DotNameLib (const std::filesystem::path& assetsPath) : DotNameLib () {
      if (assetsPath.empty ()) {
        LOG_W_STREAM << "Empty assets path provided" << "\n";
        return;
      }

      try {
        assetsPath_ = assetsPath;
        AssetContext::setAssetsPath (assetsPath);
        LOG_D_STREAM << "Assets: " << AssetContext::getAssetsPath () << "\n";
        LOG_I_STREAM << DotNameUtils::json::getCustomStringSign () << "\n";

        // Check if logo file exists before trying to open it
        const auto logoPath = AssetContext::getAssetsPath () / "DotNameCppLogo.svg";
        LOG_D_STREAM << "Logo path: " << logoPath << "\n";

        if (std::filesystem::exists (logoPath)) {
          std::ifstream logoFile (logoPath);
          if (logoFile.is_open ()) {
            LOG_D_STREAM << "Logo file successfully opened" << "\n";
            isInitialized_ = true;
          } else {
            LOG_W_STREAM << "Could not open logo file: " << logoPath << "\n";
          }
        } else {
          LOG_W_STREAM << "Logo file does not exist: " << logoPath << "\n";
        }
      } catch (const std::exception& e) {
        LOG_E_STREAM << "Error initializing DotNameLib: " << e.what () << "\n";
        isInitialized_ = false;
      }
    }

    // Move constructor
    DotNameLib::DotNameLib (DotNameLib&& other) noexcept
        : assetsPath_ (std::move (other.assetsPath_)),
          isInitialized_ (other.isInitialized_) {
      other.isInitialized_ = false;
      LOG_D_STREAM << libName_ << " move constructed" << "\n";
    }

    // Move assignment
    DotNameLib& DotNameLib::operator= (DotNameLib&& other) noexcept {
      if (this != &other) {
        assetsPath_ = std::move (other.assetsPath_);
        isInitialized_ = other.isInitialized_;
        other.isInitialized_ = false;
        LOG_D_STREAM << libName_ << " move assigned" << "\n";
      }
      return *this;
    }

    DotNameLib::~DotNameLib () {
      if (isInitialized_) {
        LOG_D_STREAM << libName_ << " (initialized) ... destructed" << "\n";
      } else {
        LOG_D_STREAM << libName_ << " (uninitialized) ... destructed" << "\n";
      }
    }

  } // namespace v1
} // namespace dotname

// MIT License Copyright (c) 2024-2025 Tomáš Mark
