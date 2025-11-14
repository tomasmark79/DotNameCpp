#pragma once

#include <Utils/Filesystem/IFileReader.hpp>

namespace dotnamecpp::utils {

  class FileReader final : public IFileReader {
  public:
    FileReader () = default;
    FileReader (const FileReader&) = delete;
    FileReader& operator= (const FileReader&) = delete;
    FileReader (FileReader&&) = delete;
    FileReader& operator= (FileReader&&) = delete;
    ~FileReader () override = default;

    [[nodiscard]]
    Result<std::string, FileError> read (const std::filesystem::path& filePath) const override;

    [[nodiscard]]
    Result<std::vector<uint8_t>, FileError>
    readBytes (const std::filesystem::path& filePath) const override;

    [[nodiscard]]
    Result<std::vector<std::string>, FileError>
    readLines (const std::filesystem::path& filePath) const override;

    [[nodiscard]]
    bool exists (const std::filesystem::path& filePath) const override;

    [[nodiscard]]
    Result<std::uintmax_t, FileError>
    getSize (const std::filesystem::path& filePath) const override;

  private:
    [[nodiscard]]
    static std::optional<FileError> validatePath (const std::filesystem::path& filePath);
  };

} // namespace dotnamecpp::utils