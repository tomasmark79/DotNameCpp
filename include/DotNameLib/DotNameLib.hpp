#ifndef __DOTNAMELIB_HEADER_GUARD__
#define __DOTNAMELIB_HEADER_GUARD__

// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include <DotNameLib/version.h> // first configuration will create this file
#include <filesystem>
#include <string>

// Public API

namespace dotname {

  class DotNameLib {
  private:
    const std::string libName_ = "DotNameLib v." DOTNAMELIB_VERSION;

  public:
    DotNameLib ();
    DotNameLib (const std::filesystem::path& assetsPath);
    ~DotNameLib ();
  };

} // namespace dotname

#endif // __DOTNAMELIB_HEADER_GUARD__