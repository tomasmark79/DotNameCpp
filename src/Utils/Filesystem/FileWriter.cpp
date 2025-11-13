#include "FileWriter.hpp"
#include <fmt/core.h>
#include <fstream>
#include <system_error>

namespace dotnamecpp {
  namespace utils {

    Result<void, FileError> FileWriter::write (const std::filesystem::path& filePath,
      const std::string& content, bool append) const {
    if (auto error = validatePath (filePath, false)) {
      return *error;
    }

    if (auto error = ensureParentExists (filePath)) {
      return *error;
    }

    auto mode = append ? (std::ios::out | std::ios::app) : std::ios::out;
    std::ofstream file (filePath, mode);

    if (!file.is_open ()) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = "Failed to open file for writing",
        .path = filePath.string (),
      };
    }

    file << content;

    if (file.bad ()) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = "I/O error while writing file",
        .path = filePath.string (),
      };
    }

    return {}; // Success - Result<void, E> default constructs to success
  }

  Result<void, FileError> FileWriter::writeBytes (const std::filesystem::path& filePath,
      const std::vector<uint8_t>& data, bool append) const {
    if (auto error = validatePath (filePath, false)) {
      return *error;
    }

    if (auto error = ensureParentExists (filePath)) {
      return *error;
    }

    auto mode = append ? (std::ios::binary | std::ios::app) : std::ios::binary;
    std::ofstream file (filePath, mode);

    if (!file.is_open ()) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = "Failed to open file for writing",
        .path = filePath.string (),
      };
    }

    file.write (reinterpret_cast<const char*> (data.data ()), data.size ());

    if (file.bad ()) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = "I/O error while writing file",
        .path = filePath.string (),
      };
    }

    return {};
  }

  Result<void, FileError> FileWriter::writeLines (const std::filesystem::path& filePath,
      const std::vector<std::string>& lines, bool append) const {
    if (auto error = validatePath (filePath, false)) {
      return *error;
    }

    if (auto error = ensureParentExists (filePath)) {
      return *error;
    }

    auto mode = append ? (std::ios::out | std::ios::app) : std::ios::out;
    std::ofstream file (filePath, mode);

    if (!file.is_open ()) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = "Failed to open file for writing",
        .path = filePath.string (),
      };
    }

    for (const auto& line : lines) {
      file << line << '\n';
    }

    if (file.bad ()) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = "I/O error while writing file",
        .path = filePath.string (),
      };
    }

    return {};
  }

  Result<void, FileError> FileWriter::touch (const std::filesystem::path& filePath) const {
    if (auto error = validatePath (filePath, false)) {
      return *error;
    }

    if (auto error = ensureParentExists (filePath)) {
      return *error;
    }

    std::error_code ec;

    // If file exists, update timestamp
    if (std::filesystem::exists (filePath, ec) && !ec) {
      auto now = std::filesystem::file_time_type::clock::now ();
      std::filesystem::last_write_time (filePath, now, ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::WriteError,
          .message = fmt::format ("Failed to update file timestamp: {}", ec.message ()),
          .path = filePath.string (),
        };
      }
    } else {
      // Create empty file
      std::ofstream file (filePath);
      if (!file.is_open ()) {
        return FileError{
          .code = FileErrorCode::WriteError,
          .message = "Failed to create file",
          .path = filePath.string (),
        };
      }
    }

    return {};
  }

  std::optional<FileError> FileWriter::validatePath (const std::filesystem::path& filePath,
      bool requireParent) const {
    if (filePath.empty ()) {
      return FileError{
        .code = FileErrorCode::InvalidPath,
        .message = "Empty file path",
        .path = "",
      };
    }

    std::error_code ec;

    // Check if path exists and is a directory
    if (std::filesystem::exists (filePath, ec) && !ec) {
      if (std::filesystem::is_directory (filePath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::IsDirectory,
          .message = "Path is a directory, not a file",
          .path = filePath.string (),
        };
      }
    }

    if (requireParent) {
      auto parent = filePath.parent_path ();
      if (!parent.empty () && !std::filesystem::exists (parent, ec)) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "Parent directory does not exist",
          .path = parent.string (),
        };
      }
    }

    return std::nullopt;
  }

  std::optional<FileError> FileWriter::ensureParentExists (
      const std::filesystem::path& filePath) const {
    auto parent = filePath.parent_path ();
    if (parent.empty ()) {
      return std::nullopt; // No parent to create
    }

    std::error_code ec;
    if (std::filesystem::exists (parent, ec) && !ec) {
      return std::nullopt; // Parent already exists
    }

    // Create parent directories
    std::filesystem::create_directories (parent, ec);
    if (ec) {
      return FileError{
        .code = FileErrorCode::WriteError,
        .message = fmt::format ("Failed to create parent directory: {}", ec.message ()),
        .path = parent.string (),
      };
    }

    return std::nullopt;
  }

  } // namespace utils
} // namespace dotnamecpp
