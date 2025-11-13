#include "CustomStringsLoader.hpp"
#include <fmt/core.h>

namespace dotname {

  // Import error types
  using dotnamecpp::utils::JsonErrorCode;

  CustomStringsLoader::CustomStringsLoader (std::shared_ptr<IAssetManager> assetManager,
      std::shared_ptr<IJsonSerializer> jsonSerializer, std::string filename)
      : assetManager_ (std::move (assetManager)), jsonSerializer_ (std::move (jsonSerializer)),
        filename_ (std::move (filename)) {
    if (!assetManager_ || !jsonSerializer_) {
      throw std::invalid_argument (
          "CustomStringsLoader requires valid asset manager and JSON serializer");
    }
  }

  Result<nlohmann::json, JsonError> CustomStringsLoader::load () const {
    // Check if asset exists
    if (!assetManager_->assetExists (filename_)) {
      return JsonError{
        .code = JsonErrorCode::FileNotFound,
        .message = "Custom strings file not found in assets",
        .details = filename_,
      };
    }

    // Resolve asset path
    auto assetPath = assetManager_->resolveAsset (filename_);

    // Load and parse JSON
    return jsonSerializer_->loadFromFile (assetPath);
  }

  Result<nlohmann::json, JsonError> CustomStringsLoader::ensureLoaded () const {
    std::lock_guard<std::mutex> lock (cacheMutex_);

    if (cachedData_) {
      return *cachedData_;
    }

    auto loadResult = load ();
    if (loadResult) {
      cachedData_ = loadResult.value ();
    }

    return loadResult;
  }

  std::optional<nlohmann::json> CustomStringsLoader::findById (const std::string& id) const {
    auto dataResult = ensureLoaded ();
    if (!dataResult) {
      return std::nullopt;
    }

    const auto& data = dataResult.value ();

    try {
      if (data.contains ("strings") && data["strings"].is_array ()) {
        for (const auto& item : data["strings"]) {
          if (item.contains ("id") && item["id"] == id) {
            return item;
          }
        }
      }
    } catch (const std::exception&) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::optional<std::string> CustomStringsLoader::getLocalizedString (const std::string& id,
      const std::string& locale) const {
    auto item = findById (id);
    if (!item) {
      return std::nullopt;
    }

    try {
      if (item->contains ("data")) {
        const auto& itemData = (*item)["data"];

        // Try requested locale
        if (itemData.contains (locale)) {
          return itemData[locale].get<std::string> ();
        }

        // Fallback to English
        if (locale != "en" && itemData.contains ("en")) {
          return itemData["en"].get<std::string> ();
        }
      }
    } catch (const std::exception&) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::optional<std::string> CustomStringsLoader::getEmail (const std::string& id) const {
    auto item = findById (id);
    if (!item) {
      return std::nullopt;
    }

    try {
      if (item->contains ("data") && (*item)["data"].contains ("email")) {
        return (*item)["data"]["email"].get<std::string> ();
      }
    } catch (const std::exception&) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::optional<std::string> CustomStringsLoader::getUrl (const std::string& id) const {
    auto item = findById (id);
    if (!item) {
      return std::nullopt;
    }

    try {
      if (item->contains ("data") && (*item)["data"].contains ("url")) {
        return (*item)["data"]["url"].get<std::string> ();
      }
    } catch (const std::exception&) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::optional<std::string> CustomStringsLoader::getTel (const std::string& id) const {
    auto item = findById (id);
    if (!item) {
      return std::nullopt;
    }

    try {
      if (item->contains ("data") && (*item)["data"].contains ("tel")) {
        return (*item)["data"]["tel"].get<std::string> ();
      }
    } catch (const std::exception&) {
      return std::nullopt;
    }

    return std::nullopt;
  }

  std::string CustomStringsLoader::getCustomStringSign () const {
    std::string result;

    // Get author info
    if (auto author = getLocalizedString ("author")) {
      result += "Author: " + *author + "\n";
    }

    // Get email
    if (auto email = getEmail ("author")) {
      result += "Email: " + *email + "\n";
    }

    // Get website
    if (auto website = getUrl ("website")) {
      result += "Website: " + *website + "\n";
    }

    // Get GitHub
    if (auto github = getUrl ("github")) {
      result += "GitHub: " + *github + "\n";
    }

    // Get phone
    if (auto phone = getTel ("contact")) {
      result += "Phone: " + *phone + "\n";
    }

    return result.empty () ? "No custom strings available" : result;
  }

} // namespace dotname
