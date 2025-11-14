#include "PathResolver.hpp"
#include <fmt/core.h>
#include <system_error>

namespace dotnamecpp::utils {

  Result<std::filesystem::path, FileError> PathResolver::getAbsolutePath (
      const std::filesystem::path& path) const {
    if (path.empty ()) {
      return FileError{
        .code = FileErrorCode::InvalidPath,
        .message = "Empty path",
        .path = "",
      };
    }

    std::error_code ec;
    auto absolute = std::filesystem::absolute (path, ec);

    if (ec) {
      return FileError{
        .code = FileErrorCode::InvalidPath,
        .message = fmt::format ("Failed to get absolute path: {}", ec.message ()),
        .path = path.string (),
      };
    }

    return absolute;
  }

  Result<std::filesystem::path, FileError> PathResolver::getCanonicalPath (
      const std::filesystem::path& path) const {
    if (path.empty ()) {
      return FileError{
        .code = FileErrorCode::InvalidPath,
        .message = "Empty path",
        .path = "",
      };
    }

    std::error_code ec;
    auto canonical = std::filesystem::canonical (path, ec);

    if (ec) {
      return FileError{
        .code = FileErrorCode::NotFound,
        .message = fmt::format ("Failed to get canonical path: {}", ec.message ()),
        .path = path.string (),
      };
    }

    return canonical;
  }

  Result<std::filesystem::path, FileError> PathResolver::getRelativePath (
      const std::filesystem::path& target, const std::filesystem::path& base) const {
    if (target.empty ()) {
      return FileError{
        .code = FileErrorCode::InvalidPath,
        .message = "Empty target path",
        .path = "",
      };
    }

    std::error_code ec;
    auto relative = std::filesystem::relative (target, base, ec);

    if (ec) {
      return FileError{
        .code = FileErrorCode::InvalidPath,
        .message = fmt::format ("Failed to get relative path: {}", ec.message ()),
        .path = target.string (),
      };
    }

    return relative;
  }

  bool PathResolver::isAbsolute (const std::filesystem::path& path) const {
    return path.is_absolute ();
  }

  bool PathResolver::isRelative (const std::filesystem::path& path) const {
    return path.is_relative ();
  }

  std::filesystem::path PathResolver::getParent (const std::filesystem::path& path) const {
    return path.parent_path ();
  }

  std::string PathResolver::getFilename (const std::filesystem::path& path) const {
    return path.filename ().string ();
  }

  std::string PathResolver::getExtension (const std::filesystem::path& path) const {
    return path.extension ().string ();
  }

  std::string PathResolver::getStem (const std::filesystem::path& path) const {
    return path.stem ().string ();
  }

  std::filesystem::path PathResolver::join (const std::vector<std::string>& parts) const {
    std::filesystem::path result;
    for (const auto& part : parts) {
      result /= part;
    }
    return result;
  }

} // namespace dotnamecpp::utils