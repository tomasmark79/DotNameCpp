#include "DirectoryManager.hpp"
#include <fmt/core.h>
#include <system_error>

namespace dotnamecpp {
  namespace utils {

    Result<void, FileError> DirectoryManager::createDirectory (
        const std::filesystem::path& dirPath) const {
      if (dirPath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty directory path",
          .path = "",
        };
      }

      std::error_code ec;
      std::filesystem::create_directories (dirPath, ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::WriteError,
          .message = fmt::format ("Failed to create directory: {}", ec.message ()),
          .path = dirPath.string (),
        };
      }

      return {};
    }

    Result<void, FileError> DirectoryManager::removeDirectory (
        const std::filesystem::path& dirPath) const {
      if (dirPath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty directory path",
          .path = "",
        };
      }

      std::error_code ec;
      if (!std::filesystem::exists (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "Directory does not exist",
          .path = dirPath.string (),
        };
      }

      if (!std::filesystem::is_directory (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotDirectory,
          .message = "Path is not a directory",
          .path = dirPath.string (),
        };
      }

      std::filesystem::remove (dirPath, ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::WriteError,
          .message = fmt::format ("Failed to remove directory: {}", ec.message ()),
          .path = dirPath.string (),
        };
      }

      return {};
    }

    Result<std::uintmax_t, FileError> DirectoryManager::removeDirectoryRecursive (
        const std::filesystem::path& dirPath) const {
      if (dirPath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty directory path",
          .path = "",
        };
      }

      std::error_code ec;
      if (!std::filesystem::exists (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "Directory does not exist",
          .path = dirPath.string (),
        };
      }

      auto removed = std::filesystem::remove_all (dirPath, ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::WriteError,
          .message = fmt::format ("Failed to remove directory recursively: {}", ec.message ()),
          .path = dirPath.string (),
        };
      }

      return removed;
    }

    bool DirectoryManager::exists (const std::filesystem::path& dirPath) const {
      std::error_code ec;
      return std::filesystem::is_directory (dirPath, ec) && !ec;
    }

    Result<bool, FileError> DirectoryManager::isEmpty (const std::filesystem::path& dirPath) const {
      if (dirPath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty directory path",
          .path = "",
        };
      }

      std::error_code ec;
      if (!std::filesystem::exists (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "Directory does not exist",
          .path = dirPath.string (),
        };
      }

      if (!std::filesystem::is_directory (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotDirectory,
          .message = "Path is not a directory",
          .path = dirPath.string (),
        };
      }

      return std::filesystem::is_empty (dirPath, ec);
    }

    Result<std::vector<std::filesystem::path>, FileError> DirectoryManager::listEntries (
        const std::filesystem::path& dirPath) const {
      if (dirPath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty directory path",
          .path = "",
        };
      }

      std::error_code ec;
      if (!std::filesystem::exists (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "Directory does not exist",
          .path = dirPath.string (),
        };
      }

      if (!std::filesystem::is_directory (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotDirectory,
          .message = "Path is not a directory",
          .path = dirPath.string (),
        };
      }

      std::vector<std::filesystem::path> entries;

      for (const auto& entry : std::filesystem::directory_iterator (dirPath, ec)) {
        if (ec) {
          return FileError{
            .code = FileErrorCode::ReadError,
            .message = fmt::format ("Error reading directory: {}", ec.message ()),
            .path = dirPath.string (),
          };
        }
        entries.push_back (entry.path ());
      }

      return entries;
    }

    Result<std::vector<std::filesystem::path>, FileError> DirectoryManager::listEntriesRecursive (
        const std::filesystem::path& dirPath) const {
      if (dirPath.empty ()) {
        return FileError{
          .code = FileErrorCode::InvalidPath,
          .message = "Empty directory path",
          .path = "",
        };
      }

      std::error_code ec;
      if (!std::filesystem::exists (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotFound,
          .message = "Directory does not exist",
          .path = dirPath.string (),
        };
      }

      if (!std::filesystem::is_directory (dirPath, ec) || ec) {
        return FileError{
          .code = FileErrorCode::NotDirectory,
          .message = "Path is not a directory",
          .path = dirPath.string (),
        };
      }

      std::vector<std::filesystem::path> entries;

      for (const auto& entry : std::filesystem::recursive_directory_iterator (dirPath, ec)) {
        if (ec) {
          return FileError{
            .code = FileErrorCode::ReadError,
            .message = fmt::format ("Error reading directory recursively: {}", ec.message ()),
            .path = dirPath.string (),
          };
        }
        entries.push_back (entry.path ());
      }

      return entries;
    }

    Result<std::filesystem::path, FileError> DirectoryManager::getCurrentDirectory () const {
      std::error_code ec;
      auto current = std::filesystem::current_path (ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = fmt::format ("Failed to get current directory: {}", ec.message ()),
          .path = "",
        };
      }

      return current;
    }

    Result<std::filesystem::path, FileError> DirectoryManager::getTempDirectory () const {
      std::error_code ec;
      auto temp = std::filesystem::temp_directory_path (ec);

      if (ec) {
        return FileError{
          .code = FileErrorCode::ReadError,
          .message = fmt::format ("Failed to get temp directory: {}", ec.message ()),
          .path = "",
        };
      }

      return temp;
    }

  } // namespace utils
} // namespace dotnamecpp
