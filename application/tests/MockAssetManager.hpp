#pragma once

#include <Utils/Assets/IAssetManager.hpp>
#include <filesystem>

/**
 * @brief Mock implementation of IAssetManager for testing
 * 
 * Allows controlling behavior for test scenarios without filesystem dependencies.
 */
class MockAssetManager : public dotnamecpp::assets::IAssetManager {
public:
  explicit MockAssetManager (std::filesystem::path mockPath)
    : mockPath_ (std::move (mockPath))
    , mockExists_ (true)
    , mockValid_ (true) {
  }

  [[nodiscard]]
  const std::filesystem::path& getAssetsPath () const override {
    return mockPath_;
  }

  [[nodiscard]]
  std::filesystem::path resolveAsset (const std::filesystem::path& relativePath) const override {
    return mockPath_ / relativePath;
  }

  [[nodiscard]]
  bool assetExists (const std::filesystem::path& /*relativePath*/) const override {
    return mockExists_;
  }

  [[nodiscard]]
  bool validate () const override {
    return mockValid_;
  }

  // Test control methods
  void setMockExists (bool exists) {
    mockExists_ = exists;
  }

  void setMockValid (bool valid) {
    mockValid_ = valid;
  }

private:
  std::filesystem::path mockPath_;
  bool mockExists_;
  bool mockValid_;
};
