#pragma once

#include <Utils/Filesystem/IFileWriter.hpp>

namespace dotnamecpp::utils {

  class FileWriter final : public IFileWriter {
  public:
    FileWriter() = default;
    FileWriter(const FileWriter &) = delete;
    FileWriter &operator=(const FileWriter &) = delete;
    FileWriter(FileWriter &&) = delete;
    FileWriter &operator=(FileWriter &&) = delete;
    ~FileWriter() override = default;

    [[nodiscard]]
    Result<void, FileError> write(const std::filesystem::path &filePath, const std::string &content,
                                  bool append = false) const override;

    [[nodiscard]]
    Result<void, FileError> writeBytes(const std::filesystem::path &filePath,
                                       const std::vector<uint8_t> &data,
                                       bool append = false) const override;

    [[nodiscard]]
    Result<void, FileError> writeLines(const std::filesystem::path &filePath,
                                       const std::vector<std::string> &lines,
                                       bool append = false) const override;

    [[nodiscard]]
    Result<void, FileError> touch(const std::filesystem::path &filePath) const override;

  private:
    [[nodiscard]]
    static std::optional<FileError> validatePath(const std::filesystem::path &filePath,
                                                 bool requireParent);

    [[nodiscard]]
    static std::optional<FileError> ensureParentExists(const std::filesystem::path &filePath);
  };

} // namespace dotnamecpp::utils