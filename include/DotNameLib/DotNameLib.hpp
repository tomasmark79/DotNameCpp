#pragma once

// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include <DotNameLib/version.h> // first configuration will create this file
#include <filesystem>
#include <string>

// Public API

namespace dotname {
  // Version information
  inline namespace v1 {

    class DotNameLib {
    private:
      const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;
      std::filesystem::path assetsPath_;
      bool isInitialized_ = false;

    public:
      DotNameLib ();
      explicit DotNameLib (const std::filesystem::path& assetsPath);
      ~DotNameLib ();

      // Rule of 5 for better resource management
      DotNameLib (const DotNameLib& other) = delete;
      DotNameLib& operator= (const DotNameLib& other) = delete;
      DotNameLib (DotNameLib&& other) noexcept;
      DotNameLib& operator= (DotNameLib&& other) noexcept;

      // Public interface
      [[nodiscard]] bool isInitialized () const noexcept {
        return isInitialized_;
      }
      [[nodiscard]] const std::filesystem::path& getAssetsPath () const noexcept {
        return assetsPath_;
      }
    };

  } // namespace v1
} // namespace dotname