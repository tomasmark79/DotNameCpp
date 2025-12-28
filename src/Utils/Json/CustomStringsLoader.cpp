#include "CustomStringsLoader.hpp"
#include <fmt/core.h>

namespace dotnamecpp::utils {

  CustomStringsLoader::CustomStringsLoader(std::shared_ptr<IAssetManager> assetManager,
                                           std::shared_ptr<IJsonSerializer> jsonSerializer,
                                           std::string filename)
      : assetManager_(std::move(assetManager)), jsonSerializer_(std::move(jsonSerializer)),
        filename_(std::move(filename)) {
    if (!assetManager_ || !jsonSerializer_) {
      throw std::invalid_argument(
          "CustomStringsLoader requires valid asset manager and JSON serializer");
    }
  }

  Result<nlohmann::json, JsonError> CustomStringsLoader::load() const {
    if (!assetManager_->assetExists(filename_)) {
      return JsonError{
          .code = JsonErrorCode::FileNotFound,
          .message = "Custom strings file not found in assets",
          .details = filename_,
      };
    }

    auto assetPath = assetManager_->resolveAsset(filename_);

    return jsonSerializer_->loadFromFile(assetPath);
  }

  Result<nlohmann::json, JsonError> CustomStringsLoader::ensureLoaded() const {
    std::lock_guard<std::mutex> lock(cacheMutex_);

    if (cachedData_) {
      return *cachedData_;
    }

    auto loadResult = load();
    if (loadResult) {
      cachedData_ = loadResult.value();
    }

    return loadResult;
  }

  std::optional<nlohmann::json> CustomStringsLoader::findById(const std::string &id) const {
    auto dataResult = ensureLoaded();
    if (!dataResult) {
      return std::nullopt;
    }

    const auto &data = dataResult.value();

    try {
      if (data.contains("strings") && data["strings"].is_array()) {
        for (const auto &item : data["strings"]) {
          if (item.contains("id") && item["id"] == id) {
            return item;
          }
        }
      }
    } catch (const std::exception &) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::optional<std::string> CustomStringsLoader::getPath(const std::string &id) const {
    auto item = findById(id);
    if (!item) {
      return std::nullopt;
    }

    try {
      if (item->contains("data") && (*item)["data"].contains("path")) {
        return (*item)["data"]["path"].get<std::string>();
      }
    } catch (const std::exception &) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::optional<std::string> CustomStringsLoader::getCustomKey(const std::string &id,
                                                               const std::string &key) const {
    auto item = findById(id);
    if (!item) {
      return std::nullopt;
    }
    try {
      if (item->contains("data") && (*item)["data"].contains(key)) {
        return (*item)["data"][key].get<std::string>();
      }
    } catch (const std::exception &) {
      return std::nullopt;
    }
    return std::nullopt;
  }

  std::optional<std::string>
      CustomStringsLoader::getLocalizedString(const std::string &id,
                                              const std::string &locale) const {
    auto item = findById(id);
    if (!item) {
      return std::nullopt;
    }

    try {
      if (item->contains("data")) {
        const auto &itemData = (*item)["data"];

        // Try requested locale
        if (itemData.contains(locale)) {
          return itemData[locale].get<std::string>();
        }

        // Fallback to English
        if (locale != "en" && itemData.contains("en")) {
          return itemData["en"].get<std::string>();
        }
      }
    } catch (const std::exception &) {
      return std::nullopt;
    }

    return std::nullopt;
  }

} // namespace dotnamecpp::utils