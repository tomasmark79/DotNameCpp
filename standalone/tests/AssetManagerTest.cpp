#include <gtest/gtest.h>
#include "../../src/Assets/AssetManager.hpp"
#include "../../src/Assets/AssetManagerFactory.hpp"
#include "MockAssetManager.hpp"
#include <filesystem>
#include <fstream>

using namespace dotnamecpp;

class AssetManagerTest : public ::testing::Test
{
  protected:
    void SetUp () override {
      // Create temporary test directory
      testAssetsPath_ = std::filesystem::temp_directory_path () / "test_assets_dotnamelib";
      std::filesystem::create_directories (testAssetsPath_);
    }

    void TearDown () override {
      // Cleanup
      if (std::filesystem::exists (testAssetsPath_)) {
        std::filesystem::remove_all (testAssetsPath_);
      }
    }

    std::filesystem::path testAssetsPath_;
};

// ============================================================================
// AssetManager Basic Tests
// ============================================================================

TEST_F (AssetManagerTest, ConstructorSetsPath) {
  AssetManager manager (testAssetsPath_);
  EXPECT_EQ (manager.getAssetsPath (), testAssetsPath_);
}

TEST_F (AssetManagerTest, ValidateReturnsTrueForExistingDirectory) {
  AssetManager manager (testAssetsPath_);
  EXPECT_TRUE (manager.validate ());
}

TEST_F (AssetManagerTest, ValidateReturnsFalseForNonexistentPath) {
  std::filesystem::path nonexistent = testAssetsPath_ / "nonexistent";
  AssetManager manager (nonexistent);
  EXPECT_FALSE (manager.validate ());
}

TEST_F (AssetManagerTest, ValidateReturnsFalseForEmptyPath) {
  AssetManager manager ("");
  EXPECT_FALSE (manager.validate ());
}

TEST_F (AssetManagerTest, ResolveAssetCombinesPaths) {
  AssetManager manager (testAssetsPath_);
  auto resolved = manager.resolveAsset ("test.txt");
  EXPECT_EQ (resolved, testAssetsPath_ / "test.txt");
}

TEST_F (AssetManagerTest, ResolveAssetWithSubdirectory) {
  AssetManager manager (testAssetsPath_);
  auto resolved = manager.resolveAsset ("subdir/test.json");
  EXPECT_EQ (resolved, testAssetsPath_ / "subdir" / "test.json");
}

TEST_F (AssetManagerTest, AssetExistsReturnsTrueForExistingFile) {
  // Create test file
  auto testFile = testAssetsPath_ / "test.txt";
  std::ofstream (testFile) << "test content";

  AssetManager manager (testAssetsPath_);
  EXPECT_TRUE (manager.assetExists ("test.txt"));
}

TEST_F (AssetManagerTest, AssetExistsReturnsFalseForNonexistentFile) {
  AssetManager manager (testAssetsPath_);
  EXPECT_FALSE (manager.assetExists ("nonexistent.txt"));
}

TEST_F (AssetManagerTest, AssetExistsWorksWithSubdirectories) {
  // Create subdirectory and file
  auto subdir = testAssetsPath_ / "data";
  std::filesystem::create_directories (subdir);
  auto testFile = subdir / "config.json";
  std::ofstream (testFile) << "{}";

  AssetManager manager (testAssetsPath_);
  EXPECT_TRUE (manager.assetExists ("data/config.json"));
  EXPECT_FALSE (manager.assetExists ("data/missing.json"));
}

// ============================================================================
// AssetManagerFactory Tests
// ============================================================================

TEST_F (AssetManagerTest, FactoryCreateReturnsValidManager) {
  auto manager = AssetManagerFactory::create (testAssetsPath_);
  ASSERT_NE (manager, nullptr);
  EXPECT_EQ (manager->getAssetsPath (), testAssetsPath_);
  EXPECT_TRUE (manager->validate ());
}

TEST_F (AssetManagerTest, FactoryCreateDefaultFindsAssetsPath) {
  // Create mock executable directory structure
  auto execDir = testAssetsPath_ / "bin";
  auto assetsDir = testAssetsPath_ / "assets"; // First candidate
  std::filesystem::create_directories (execDir);
  std::filesystem::create_directories (assetsDir);

  auto execPath = execDir / "app";
  auto manager = AssetManagerFactory::createDefault (execPath, "TestApp");

  ASSERT_NE (manager, nullptr);
  // Should find the assets directory relative to executable
  EXPECT_TRUE (manager->validate () || !manager->getAssetsPath ().empty ());
}

// ============================================================================
// MockAssetManager Tests
// ============================================================================

TEST (MockAssetManagerTest, BasicFunctionality) {
  MockAssetManager mock ("/mock/assets");
  
  EXPECT_EQ (mock.getAssetsPath (), "/mock/assets");
  EXPECT_TRUE (mock.validate ());
  EXPECT_TRUE (mock.assetExists ("any.txt"));
}

TEST (MockAssetManagerTest, ResolveAssetCombinesPaths) {
  MockAssetManager mock ("/mock/assets");
  
  auto resolved = mock.resolveAsset ("test.svg");
  EXPECT_EQ (resolved, "/mock/assets/test.svg");
}

TEST (MockAssetManagerTest, CanControlValidation) {
  MockAssetManager mock ("/mock/assets");
  
  EXPECT_TRUE (mock.validate ());
  
  mock.setMockValid (false);
  EXPECT_FALSE (mock.validate ());
  
  mock.setMockValid (true);
  EXPECT_TRUE (mock.validate ());
}

TEST (MockAssetManagerTest, CanControlAssetExistence) {
  MockAssetManager mock ("/mock/assets");
  
  EXPECT_TRUE (mock.assetExists ("file.txt"));
  
  mock.setMockExists (false);
  EXPECT_FALSE (mock.assetExists ("file.txt"));
  EXPECT_FALSE (mock.assetExists ("other.txt"));
  
  mock.setMockExists (true);
  EXPECT_TRUE (mock.assetExists ("file.txt"));
}

TEST (MockAssetManagerTest, UsefulForTestingDependentCode) {
  // Example: Testing code that depends on IAssetManager
  auto mockAssets = std::make_shared<MockAssetManager> ("/test/assets");
  
  // Simulate missing assets scenario
  mockAssets->setMockExists (false);
  mockAssets->setMockValid (true);
  
  EXPECT_TRUE (mockAssets->validate ());
  EXPECT_FALSE (mockAssets->assetExists ("logo.svg"));
  
  // Verify path resolution still works
  auto logoPath = mockAssets->resolveAsset ("logo.svg");
  EXPECT_EQ (logoPath, "/test/assets/logo.svg");
}

// ============================================================================
// Edge Cases and Error Handling
// ============================================================================

TEST_F (AssetManagerTest, HandlesPathsWithSpaces) {
  auto pathWithSpaces = testAssetsPath_ / "dir with spaces";
  std::filesystem::create_directories (pathWithSpaces);
  
  AssetManager manager (pathWithSpaces);
  EXPECT_TRUE (manager.validate ());
  EXPECT_EQ (manager.getAssetsPath (), pathWithSpaces);
}

TEST_F (AssetManagerTest, HandlesUnicodeInPaths) {
  auto unicodePath = testAssetsPath_ / "资源"; // Chinese characters
  std::filesystem::create_directories (unicodePath);
  
  AssetManager manager (unicodePath);
  EXPECT_TRUE (manager.validate ());
}

TEST_F (AssetManagerTest, ResolveAssetWithEmptyRelativePath) {
  AssetManager manager (testAssetsPath_);
  auto resolved = manager.resolveAsset ("");
  // Empty path resolves to base path (may have trailing slash)
  EXPECT_TRUE (resolved == testAssetsPath_ 
               || resolved == testAssetsPath_.string () + "/");
}

TEST_F (AssetManagerTest, MultipleManagersWithDifferentPaths) {
  auto path1 = testAssetsPath_ / "assets1";
  auto path2 = testAssetsPath_ / "assets2";
  std::filesystem::create_directories (path1);
  std::filesystem::create_directories (path2);
  
  AssetManager manager1 (path1);
  AssetManager manager2 (path2);
  
  EXPECT_NE (manager1.getAssetsPath (), manager2.getAssetsPath ());
  EXPECT_TRUE (manager1.validate ());
  EXPECT_TRUE (manager2.validate ());
}

// ============================================================================
// Interface Polymorphism Tests
// ============================================================================

TEST_F (AssetManagerTest, WorksThroughInterfacePointer) {
  std::unique_ptr<IAssetManager> manager = std::make_unique<AssetManager> (testAssetsPath_);
  
  EXPECT_EQ (manager->getAssetsPath (), testAssetsPath_);
  EXPECT_TRUE (manager->validate ());
  
  auto resolved = manager->resolveAsset ("test.txt");
  EXPECT_EQ (resolved, testAssetsPath_ / "test.txt");
}

TEST_F (AssetManagerTest, SharedPointerLifetimeManagement) {
  std::shared_ptr<IAssetManager> manager1 = AssetManagerFactory::create (testAssetsPath_);
  std::shared_ptr<IAssetManager> manager2 = manager1;
  
  EXPECT_EQ (manager1.use_count (), 2);
  EXPECT_EQ (manager1->getAssetsPath (), manager2->getAssetsPath ());
}

// MIT License Copyright (c) 2024-2025 Tomáš Mark
