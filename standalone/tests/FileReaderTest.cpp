#include <Utils/Filesystem/FileReader.hpp>
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

using namespace dotnamecpp::utils;
namespace fs = std::filesystem;

class FileReaderTest : public ::testing::Test {
protected:
  void SetUp () override {
    // Create temporary directory for tests
    testDir_ = fs::temp_directory_path () / "FileReaderTest";
    fs::create_directories (testDir_);

    // Create test files
    simpleFile_ = testDir_ / "simple.txt";
    emptyFile_ = testDir_ / "empty.txt";
    multiLineFile_ = testDir_ / "multiline.txt";
    binaryFile_ = testDir_ / "binary.bin";
    unicodeFile_ = testDir_ / "unicode_文件.txt";

    // Write simple file
    std::ofstream (simpleFile_) << "Hello, World!";

    // Write empty file
    {
      std::ofstream emptyFileStream (emptyFile_);
    }

    // Write multi-line file
    std::ofstream (multiLineFile_) << "Line 1\nLine 2\nLine 3\n";

    // Write binary file
    std::ofstream binFile (binaryFile_, std::ios::binary);
    std::vector<uint8_t> bytes = { 0x00, 0xFF, 0x42, 0xAB, 0xCD }; // NOLINT
    binFile.write (reinterpret_cast<const char*> (bytes.data ()), bytes.size ());

    // Write unicode file
    std::ofstream (unicodeFile_) << "Unicode: 你好世界 🌍";
  }

  void TearDown () override {
    // Clean up test directory
    std::error_code ec;
    fs::remove_all (testDir_, ec);
  }

  fs::path testDir_;
  fs::path simpleFile_;
  fs::path emptyFile_;
  fs::path multiLineFile_;
  fs::path binaryFile_;
  fs::path unicodeFile_;
};

// ============================================================================
// exists() tests
// ============================================================================

TEST_F (FileReaderTest, ExistsReturnsTrueForExistingFile) {
  FileReader reader;
  EXPECT_TRUE (reader.exists (simpleFile_));
}

TEST_F (FileReaderTest, ExistsReturnsFalseForNonexistentFile) {
  FileReader reader;
  EXPECT_FALSE (reader.exists (testDir_ / "nonexistent.txt"));
}

TEST_F (FileReaderTest, ExistsReturnsFalseForDirectory) {
  FileReader reader;
  EXPECT_FALSE (reader.exists (testDir_));
}

TEST_F (FileReaderTest, ExistsHandlesUnicodePaths) {
  FileReader reader;
  EXPECT_TRUE (reader.exists (unicodeFile_));
}

// ============================================================================
// getSize() tests
// ============================================================================

TEST_F (FileReaderTest, GetSizeReturnsCorrectSize) {
  FileReader reader;
  auto result = reader.getSize (simpleFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (result.value (), 13); // "Hello, World!" = 13 bytes
}

TEST_F (FileReaderTest, GetSizeHandlesEmptyFile) {
  FileReader reader;
  auto result = reader.getSize (emptyFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (result.value (), 0);
}

TEST_F (FileReaderTest, GetSizeFailsForNonexistentFile) {
  FileReader reader;
  auto result = reader.getSize (testDir_ / "nonexistent.txt");

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::NotFound);
}

TEST_F (FileReaderTest, GetSizeFailsForDirectory) {
  FileReader reader;
  auto result = reader.getSize (testDir_);

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::IsDirectory);
}

// ============================================================================
// read() tests
// ============================================================================

TEST_F (FileReaderTest, ReadReturnsFileContent) {
  FileReader reader;
  auto result = reader.read (simpleFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (result.value (), "Hello, World!");
}

TEST_F (FileReaderTest, ReadHandlesEmptyFile) {
  FileReader reader;
  auto result = reader.read (emptyFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (result.value (), "");
}

TEST_F (FileReaderTest, ReadHandlesUnicodeContent) {
  FileReader reader;
  auto result = reader.read (unicodeFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (result.value (), "Unicode: 你好世界 🌍");
}

TEST_F (FileReaderTest, ReadFailsForNonexistentFile) {
  FileReader reader;
  auto result = reader.read (testDir_ / "nonexistent.txt");

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::NotFound);
}

TEST_F (FileReaderTest, ReadFailsForDirectory) {
  FileReader reader;
  auto result = reader.read (testDir_);

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::IsDirectory);
}

TEST_F (FileReaderTest, ReadFailsForEmptyPath) {
  FileReader reader;
  auto result = reader.read (fs::path{});

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::InvalidPath);
}

// ============================================================================
// readBytes() tests
// ============================================================================

TEST_F (FileReaderTest, ReadBytesReturnsCorrectData) {
  FileReader reader;
  auto result = reader.readBytes (binaryFile_);

  ASSERT_TRUE (result.hasValue ());
  std::vector<uint8_t> expected = { 0x00, 0xFF, 0x42, 0xAB, 0xCD }; // NOLINT
  EXPECT_EQ (result.value (), expected);
}

TEST_F (FileReaderTest, ReadBytesHandlesEmptyFile) {
  FileReader reader;
  auto result = reader.readBytes (emptyFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_TRUE (result.value ().empty ());
}

TEST_F (FileReaderTest, ReadBytesWorksWithTextFile) {
  FileReader reader;
  auto result = reader.readBytes (simpleFile_);

  ASSERT_TRUE (result.hasValue ());
  std::string content (result.value ().begin (), result.value ().end ());
  EXPECT_EQ (content, "Hello, World!");
}

TEST_F (FileReaderTest, ReadBytesFailsForNonexistentFile) {
  FileReader reader;
  auto result = reader.readBytes (testDir_ / "nonexistent.bin");

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::NotFound);
}

// ============================================================================
// readLines() tests
// ============================================================================

TEST_F (FileReaderTest, ReadLinesReturnsAllLines) {
  FileReader reader;
  auto result = reader.readLines (multiLineFile_);

  ASSERT_TRUE (result.hasValue ());
  std::vector<std::string> expected = { "Line 1", "Line 2", "Line 3" };
  EXPECT_EQ (result.value (), expected);
}

TEST_F (FileReaderTest, ReadLinesHandlesSingleLine) {
  FileReader reader;
  auto result = reader.readLines (simpleFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (result.value ().size (), 1);
  EXPECT_EQ (result.value ()[0], "Hello, World!");
}

TEST_F (FileReaderTest, ReadLinesHandlesEmptyFile) {
  FileReader reader;
  auto result = reader.readLines (emptyFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_TRUE (result.value ().empty ());
}

TEST_F (FileReaderTest, ReadLinesHandlesTrailingNewline) {
  // multiLineFile_ has trailing newline
  FileReader reader;
  auto result = reader.readLines (multiLineFile_);

  ASSERT_TRUE (result.hasValue ());
  // std::getline doesn't include the last empty line if file ends with \n
  EXPECT_EQ (result.value ().size (), 3);
}

TEST_F (FileReaderTest, ReadLinesFailsForNonexistentFile) {
  FileReader reader;
  auto result = reader.readLines (testDir_ / "nonexistent.txt");

  EXPECT_FALSE (result.hasValue ());
  EXPECT_EQ (result.error ().code, dotnamecpp::utils::FileErrorCode::NotFound);
}

// ============================================================================
// Error handling tests
// ============================================================================

TEST_F (FileReaderTest, ErrorContainsFilePath) {
  FileReader reader;
  auto nonexistent = testDir_ / "missing.txt";
  auto result = reader.read (nonexistent);

  EXPECT_FALSE (result.hasValue ());
  EXPECT_FALSE (result.error ().path.empty ());
  EXPECT_EQ (result.error ().path, nonexistent.string ());
}

TEST_F (FileReaderTest, ErrorContainsMessage) {
  FileReader reader;
  auto result = reader.read (testDir_ / "missing.txt");

  EXPECT_FALSE (result.hasValue ());
  EXPECT_FALSE (result.error ().message.empty ());
}

TEST_F (FileReaderTest, ErrorToStringProducesOutput) {
  FileReader reader;
  auto result = reader.read (testDir_ / "missing.txt");

  EXPECT_FALSE (result.hasValue ());
  std::string errorStr = result.error ().toString ();
  EXPECT_FALSE (errorStr.empty ());
  EXPECT_NE (errorStr.find ("NotFound"), std::string::npos);
}

// ============================================================================
// Result API tests
// ============================================================================

TEST_F (FileReaderTest, ResultBoolConversionWorks) {
  FileReader reader;
  auto success = reader.read (simpleFile_);
  auto failure = reader.read (testDir_ / "missing.txt");

  EXPECT_TRUE (static_cast<bool> (success));
  EXPECT_FALSE (static_cast<bool> (failure));
}

TEST_F (FileReaderTest, ResultDereferenceOperatorWorks) {
  FileReader reader;
  auto result = reader.read (simpleFile_);

  ASSERT_TRUE (result.hasValue ());
  EXPECT_EQ (*result, "Hello, World!");
}

TEST_F (FileReaderTest, ResultValueOrWorks) {
  FileReader reader;
  auto success = reader.read (simpleFile_);
  auto failure = reader.read (testDir_ / "missing.txt");

  EXPECT_EQ (success.valueOr ("default"), "Hello, World!");
  EXPECT_EQ (failure.valueOr ("default"), "default");
}
