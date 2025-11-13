#include "FileReader.hpp"
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <system_error>

namespace dotnamecpp {
  namespace utils {

    Result<std::string, FileError> FileReader::read (const std::filesystem::path& filePath) const {
      if (auto error = validatePath (filePath)) {
        return *error;
      }

      std::ifstream file (filePath, std::ios::in);
      if (!file.is_open ()) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "Failed to open file for reading",
          .path = filePath.string (),
        };
      }

      std::ostringstream buffer;
      buffer << file.rdbuf ();

      if (file.bad ()) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "I/O error while reading file",
          .path = filePath.string (),
        };
      }

      return buffer.str ();
    }

    Result<std::vector<uint8_t>, FileError> FileReader::readBytes (
        const std::filesystem::path& filePath) const {
      if (auto error = validatePath (filePath)) {
        return *error;
      }

      std::ifstream file (filePath, std::ios::binary);
      if (!file.is_open ()) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "Failed to open file for reading",
          .path = filePath.string (),
        };
      }

      // Get file size for pre-allocation
      auto sizeResult = getSize (filePath);
      if (!sizeResult) {
        return sizeResult.error ();
      }

      std::vector<uint8_t> buffer;
      buffer.reserve (static_cast<size_t> (sizeResult.value ()));

      buffer.assign (std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());

      if (file.bad ()) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "I/O error while reading file",
          .path = filePath.string (),
        };
      }

      return buffer;
    }

    Result<std::vector<std::string>, FileError> FileReader::readLines (
        const std::filesystem::path& filePath) const {
      if (auto error = validatePath (filePath)) {
        return *error;
      }

      std::ifstream file (filePath, std::ios::in);
      if (!file.is_open ()) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "Failed to open file for reading",
          .path = filePath.string (),
        };
      }

      std::vector<std::string> lines;
      std::string line;

      while (std::getline (file, line)) {
        lines.push_back (std::move (line));
      }

      if (file.bad ()) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = "I/O error while reading file",
          .path = filePath.string (),
        };
      }

      return lines;
    }

    bool FileReader::exists (const std::filesystem::path& filePath) const {
      std::error_code ec;
      return std::filesystem::is_regular_file (filePath, ec) && !ec;
    }

    Result<std::uintmax_t, FileError> FileReader::getSize (
        const std::filesystem::path& filePath) const {
      if (auto error = validatePath (filePath)) {
        return *error;
      }

      std::error_code ec;
      auto size = std::filesystem::file_size (filePath, ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = fmt::format ("Failed to get file size: {}", ec.message ()),
          .path = filePath.string (),
        };
      }

      return size;
    }

    std::optional<FileError> FileReader::validatePath (const std::filesystem::path& filePath) {
      if (filePath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty file path",
          .path = "",
        };
      }

      std::error_code ec;

      if (!std::filesystem::exists (filePath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "File does not exist",
          .path = filePath.string (),
        };
      }

      if (std::filesystem::is_directory (filePath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::IsDirectory,
          .message = "Path is a directory, not a file",
          .path = filePath.string (),
        };
      }

      // Note: More comprehensive access check would require platform-specific code
      // For now, we rely on open() to report AccessDenied errors

      return std::nullopt;
    }

  } // namespace utils
} // namespace dotnamecpp
