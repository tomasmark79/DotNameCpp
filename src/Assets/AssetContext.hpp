// MIT License Copyright (c) 2024-2025 Tomáš Mark

#ifndef __ASSETCONTEXT_H__
#define __ASSETCONTEXT_H__

#include <filesystem>

namespace AssetContext {
  void clearAssetsPath (void);
  void setAssetsPath (const std::filesystem::path& path);
  const std::filesystem::path& getAssetsPath ();
  std::filesystem::path findAssetsPath (const std::filesystem::path& executablePath,
                                        const std::string& appName);
}

#endif // __ASSETCONTEXT_H__