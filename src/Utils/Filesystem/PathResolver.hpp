#pragma once

#include <Utils/Filesystem/IPathResolver.hpp>

namespace dotnamecpp::utils {

  class PathResolver final : public IPathResolver {
  public:
    PathResolver () = default;
    PathResolver (const PathResolver&) = delete;
    PathResolver& operator= (const PathResolver&) = delete;
    PathResolver (PathResolver&&) = delete;
    PathResolver& operator= (PathResolver&&) = delete;
    ~PathResolver () override = default;

    [[nodiscard]]
    Result<std::filesystem::path, FileError>
    getAbsolutePath (const std::filesystem::path& path) const override;

    [[nodiscard]]
    Result<std::filesystem::path, FileError>
    getCanonicalPath (const std::filesystem::path& path) const override;

    [[nodiscard]]
    Result<std::filesystem::path, FileError>
    getRelativePath (const std::filesystem::path& target,
                     const std::filesystem::path& base) const override;

    [[nodiscard]]
    bool isAbsolute (const std::filesystem::path& path) const override;

    [[nodiscard]]
    bool isRelative (const std::filesystem::path& path) const override;

    [[nodiscard]]
    std::filesystem::path getParent (const std::filesystem::path& path) const override;

    [[nodiscard]]
    std::string getFilename (const std::filesystem::path& path) const override;

    [[nodiscard]]
    std::string getExtension (const std::filesystem::path& path) const override;

    [[nodiscard]]
    std::string getStem (const std::filesystem::path& path) const override;

    [[nodiscard]]
    std::filesystem::path join (const std::vector<std::string>& parts) const override;
  };

} // namespace dotnamecpp::utils