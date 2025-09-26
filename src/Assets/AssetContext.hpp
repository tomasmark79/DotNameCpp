#ifndef __ASSETCONTEXT_HEADER_GUARD__
#define __ASSETCONTEXT_HEADER_GUARD__

// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include <filesystem>

namespace AssetContext {
  void clearAssetsPath ();
  void setAssetsPath (const std::filesystem::path& path);
  const std::filesystem::path& getAssetsPath ();
  std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
                                        const std::string& appName);
}

#endif // __ASSETCONTEXT_HEADER_GUARD__