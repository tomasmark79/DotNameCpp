#pragma once

#include <Utils/Filesystem/IFileReader.hpp>

namespace dotnamecpp {
  namespace utils {

    /**
 * @brief Standard implementation of IFileReader interface
 * 
 * Provides file reading operations using std::filesystem and standard C++ streams.
 * Thread-safe for concurrent reads from different instances.
 */
    class FileReader final : public IFileReader {
    public:
      FileReader () = default;
      ~FileReader () override = default;

      // Delete copy/move - stateless class, just use multiple instances
      FileReader (const FileReader&) = delete;
      FileReader& operator= (const FileReader&) = delete;
      FileReader (FileReader&&) = delete;
      FileReader& operator= (FileReader&&) = delete;

      [[nodiscard]] Result<std::string, FileError> read (
          const std::filesystem::path& filePath) const override;

      [[nodiscard]] Result<std::vector<uint8_t>, FileError> readBytes (
          const std::filesystem::path& filePath) const override;

      [[nodiscard]] Result<std::vector<std::string>, FileError> readLines (
          const std::filesystem::path& filePath) const override;

      [[nodiscard]] bool exists (const std::filesystem::path& filePath) const override;

      [[nodiscard]] Result<std::uintmax_t, FileError> getSize (
          const std::filesystem::path& filePath) const override;

    private:
      /**
     * @brief Validate path and check file accessibility
     * @param filePath Path to validate
     * @return FileError if path is invalid/inaccessible, std::nullopt if OK
     */
      [[nodiscard]] static std::optional<FileError> validatePath (
          const std::filesystem::path& filePath);
    };

  } // namespace utils
} // namespace dotnamecpp
