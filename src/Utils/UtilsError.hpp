#pragma once

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

namespace dotnamecpp::utils {

  // Error codes for file operations
  enum class FileErrorCode : std::uint8_t {
    NotFound,
    AccessDenied,
    AlreadyExists,
    ReadError,
    WriteError,
    InvalidPath,
    IsDirectory,
    NotDirectory,
    Unknown
  };

  // Error codes for JSON operations
  enum class JsonErrorCode : std::uint8_t {
    ParseError,
    InvalidStructure,
    FileNotFound,
    InvalidType,
    MissingKey,
    Unknown
  };

  // File operation error details
  struct FileError {
    FileErrorCode code;
    std::string message;
    std::string path;

    [[nodiscard]]
    std::string toString() const {
      std::string codeStr;
      switch (code) {
      case FileErrorCode::NotFound: codeStr = "NotFound"; break;
      case FileErrorCode::AccessDenied: codeStr = "AccessDenied"; break;
      case FileErrorCode::AlreadyExists: codeStr = "AlreadyExists"; break;
      case FileErrorCode::ReadError: codeStr = "ReadError"; break;
      case FileErrorCode::WriteError: codeStr = "WriteError"; break;
      case FileErrorCode::InvalidPath: codeStr = "InvalidPath"; break;
      case FileErrorCode::IsDirectory: codeStr = "IsDirectory"; break;
      case FileErrorCode::NotDirectory: codeStr = "NotDirectory"; break;
      case FileErrorCode::Unknown: codeStr = "Unknown"; break;
      }
      return codeStr + ": " + message + (path.empty() ? "" : " (path: " + path + ")");
    }
  };

  // JSON operation error details
  struct JsonError {
    JsonErrorCode code;
    std::string message;
    std::string details;

    [[nodiscard]]
    std::string toString() const {
      std::string codeStr;
      switch (code) {
      case JsonErrorCode::ParseError: codeStr = "ParseError"; break;
      case JsonErrorCode::InvalidStructure: codeStr = "InvalidStructure"; break;
      case JsonErrorCode::FileNotFound: codeStr = "FileNotFound"; break;
      case JsonErrorCode::InvalidType: codeStr = "InvalidType"; break;
      case JsonErrorCode::MissingKey: codeStr = "MissingKey"; break;
      case JsonErrorCode::Unknown: codeStr = "Unknown"; break;
      }
      return codeStr + ": " + message + (details.empty() ? "" : " (details: " + details + ")");
    }
  };

  // Helper functions to create errors
  inline FileError makeFileError(FileErrorCode code, const std::string &message,
                                 const std::string &path = "") {
    return FileError{code, message, path};
  }

  inline JsonError makeJsonError(JsonErrorCode code, const std::string &message,
                                 const std::string &details = "") {
    return JsonError{code, message, details};
  }

  // Custom Result<T, E> implementation (similar to std::expected from C++23)
  // This is a simplified version - for production, consider using std::expected or a library
  template <typename T, typename E>
  class Result {
  public:
    // Constructors for success case
    Result(const T &value) : data_(value), hasValue_(true) {}

    Result(T &&value) : data_(std::move(value)), hasValue_(true) {}

    // Constructor for error case
    Result(const E &error) : data_(error), hasValue_(false) {}

    Result(E &&error) : data_(std::move(error)), hasValue_(false) {}

    // Check if result contains value
    [[nodiscard]]
    bool hasValue() const noexcept {
      return hasValue_;
    }

    [[nodiscard]]
    explicit operator bool() const noexcept {
      return hasValue_;
    }

    // Get value (throws if error)
    [[nodiscard]]
    T &value() & {
      if (!hasValue_) {
        throw std::logic_error("Accessing value of Result with error");
      }
      return std::get<T>(data_);
    }

    [[nodiscard]]
    const T &value() const & {
      if (!hasValue_) {
        throw std::logic_error("Accessing value of Result with error");
      }
      return std::get<T>(data_);
    }

    [[nodiscard]]
    T &&value() && {
      if (!hasValue_) {
        throw std::logic_error("Accessing value of Result with error");
      }
      return std::get<T>(std::move(data_));
    }

    // Get error (throws if value)
    [[nodiscard]]
    E &error() & {
      if (hasValue_) {
        throw std::logic_error("Accessing error of Result with value");
      }
      return std::get<E>(data_);
    }

    [[nodiscard]]
    const E &error() const & {
      if (hasValue_) {
        throw std::logic_error("Accessing error of Result with value");
      }
      return std::get<E>(data_);
    }

    [[nodiscard]]
    E &&error() && {
      if (hasValue_) {
        throw std::logic_error("Accessing error of Result with value");
      }
      return std::get<E>(std::move(data_));
    }

    // Dereference operators (for value)
    [[nodiscard]]
    T *operator->() {
      return &value();
    }

    [[nodiscard]]
    const T *operator->() const {
      return &value();
    }

    [[nodiscard]]
    T &operator*() & {
      return value();
    }

    [[nodiscard]]
    const T &operator*() const & {
      return value();
    }

    [[nodiscard]]
    T &&operator*() && {
      return std::move(value());
    }

    // Get value or default
    template <typename U>
    [[nodiscard]]
    T valueOr(U &&defaultValue) const & {
      return hasValue_ ? std::get<T>(data_) : static_cast<T>(std::forward<U>(defaultValue));
    }

    template <typename U>
    [[nodiscard]]
    T valueOr(U &&defaultValue) && {
      return hasValue_ ? std::get<T>(std::move(data_))
                       : static_cast<T>(std::forward<U>(defaultValue));
    }

  private:
    std::variant<T, E> data_;
    bool hasValue_;
  };

  // Specialization for void return type
  template <typename E>
  class Result<void, E> {
  public:
    // Constructor for success case
    Result() : error_(std::nullopt), hasValue_(true) {}

    // Constructor for error case
    Result(const E &error) : error_(error), hasValue_(false) {}

    Result(E &&error) : error_(std::move(error)), hasValue_(false) {}

    [[nodiscard]]
    bool hasValue() const noexcept {
      return hasValue_;
    }

    [[nodiscard]]
    explicit operator bool() const noexcept {
      return hasValue_;
    }

    [[nodiscard]]
    E &error() & {
      if (hasValue_) {
        throw std::logic_error("Accessing error of successful Result");
      }
      return *error_;
    }

    [[nodiscard]]
    const E &error() const & {
      if (hasValue_) {
        throw std::logic_error("Accessing error of successful Result");
      }
      return *error_;
    }

  private:
    std::optional<E> error_;
    bool hasValue_;
  };

} // namespace dotnamecpp::utils
