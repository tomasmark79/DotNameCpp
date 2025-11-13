#pragma once

#define CUSTOM_STRINGS_FILE "customstrings.json"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <cctype>

// ----------------------------------------------------
// processed by ../cmake/tmplt-assets.cmake)
#ifndef UTILS_ASSET_PATH
  #define UTILS_ASSET_PATH ""
#endif
#ifndef UTILS_FIRST_ASSET_FILE
  #define UTILS_FIRST_ASSET_FILE ""
#endif
#ifndef UTILS_ASSET_FILES_DIVIDED_BY_COMMAS
  #define UTILS_ASSET_FILES_DIVIDED_BY_COMMAS ""
#endif
// -----------------------------------------------------

#ifdef _WIN32
  #include "../WindowsHeaders.hpp"
#endif

#if defined(__APPLE__)
  #include <limits.h>
  #include <mach-o/dyld.h>
#elif defined(__linux__)
  #include <unistd.h>
#endif

namespace dotnamecpp {
namespace utils {

  // Core filesystem operations
  namespace fs {
    inline std::string readFile (const std::filesystem::path& filePath) {
      std::ifstream file (filePath, std::ios::in);
      if (!file.is_open ()) {
        throw std::ios_base::failure ("Failed to open file: " + filePath.string ());
      }
      std::stringstream buffer;
      buffer << file.rdbuf ();
      return buffer.str ();
    }

    inline void writeFile (const std::filesystem::path& filePath, const std::string& content) {
      std::ofstream file (filePath, std::ios::out | std::ios::trunc);
      if (!file.is_open ()) {
        throw std::ios_base::failure ("Failed to open file: " + filePath.string ());
      }
      file << content;
    }

    inline std::filesystem::path getParentPath (const std::filesystem::path& filePath) {
      return filePath.parent_path ();
    }

    inline std::string getFileName (const std::filesystem::path& filePath) {
      return filePath.filename ().string ();
    }

    inline bool fileExists (const std::filesystem::path& filePath) {
      return std::filesystem::exists (filePath);
    }

    inline std::filesystem::path getExecutablePath () {
      std::filesystem::path path;

#ifdef _WIN32
      // C-Like is intended here for cross-platform compatibility
      char buffer[MAX_PATH];
      GetModuleFileNameA (NULL, buffer, MAX_PATH);
      path = buffer;
#elif defined(__APPLE__)
      // C-Like is intended here for cross-platform compatibility
      char buffer[PATH_MAX];
      uint32_t bufferSize = PATH_MAX;
      if (_NSGetExecutablePath (buffer, &bufferSize) == 0) {
        path = buffer;
      }
#elif defined(__EMSCRIPTEN__)
      // Emscripten doesn't have readlink, use current directory
      path = std::filesystem::current_path ().string ();
#elif defined(__linux__)
      // C-Like is intended here for cross-platform compatibility
      char buffer[4096];
      ssize_t len = readlink ("/proc/self/exe", buffer, sizeof (buffer) - 1);
      if (len != -1) {
        buffer[len] = '\0';
        path = buffer;
      }
#endif
      return path;
    }

    inline void createDirectory (const std::filesystem::path& dirPath) {
      if (!std::filesystem::exists (dirPath)) {
        std::filesystem::create_directories (dirPath);
      }
    }

    inline void remove (const std::filesystem::path& path) {
      if (std::filesystem::exists (path)) {
        std::filesystem::remove_all (path);
      }
    }

    inline std::vector<std::filesystem::path> listFiles (const std::filesystem::path& dirPath) {
      std::vector<std::filesystem::path> files;
      for (const auto& entry : std::filesystem::directory_iterator (dirPath)) {
        files.push_back (entry.path ());
      }
      return files;
    }
  } // namespace fs

  // String utilities
  namespace string {
    inline std::string addDots (const std::string& str) {
      std::string result;
      for (size_t i = 0; i < str.length (); ++i) {
        result += str[i];
        if ((str.length () - i - 1) % 3 == 0 && i != str.length () - 1) {
          result += '.';
        }
      }
      return result;
    }

    inline std::string removeDots (const std::string& str) {
      std::string result;
      for (size_t i = 0; i < str.length (); ++i) {
        if (str[i] != '.') {
          result += str[i];
        }
      }
      return result;
    }
  } // namespace string

  // JSON utilities
  namespace json {
      // Load JSON from file
      inline nlohmann::json loadFromFile (const std::filesystem::path& filePath) {
        if (!utils::fs::fileExists (filePath)) {
          throw std::ios_base::failure ("JSON file does not exist: " + filePath.string ());
        }

        try {
          std::string content = utils::fs::readFile (filePath);
          return nlohmann::json::parse (content);
        } catch (const nlohmann::json::parse_error& e) {
          throw std::runtime_error (
              "JSON parse error in file " + filePath.string () + ": " + e.what ());
        }
      }

      // Save JSON to file
      inline void saveToFile (const std::filesystem::path& filePath, const nlohmann::json& jsonData,
          int indent = 2) {
        try {
          std::string jsonString = jsonData.dump (indent);
          utils::fs::writeFile (filePath, jsonString);
        } catch (const std::exception& e) {
          throw std::runtime_error (
              "Failed to save JSON to file " + filePath.string () + ": " + e.what ());
        }
      }

    // Get value with default fallback
    template <typename T>
    inline T getValue (const nlohmann::json& json, const std::string& key,
        const T& defaultValue = T{}) {
      try {
        if (json.contains (key)) {
          return json[key].get<T> ();
        }
      } catch (const std::exception&) {
        // Return default on any error
      }
      return defaultValue;
    }

    // Get nested value with path (e.g., "strings/0/data/en")
    template <typename T>
    inline T getNestedValue (const nlohmann::json& json, const std::string& path,
        const T& defaultValue = T{}) {
      try {
        std::istringstream pathStream (path);
        std::string segment;
        nlohmann::json current = json;

        while (std::getline (pathStream, segment, '/')) {
          if (segment.empty ()) {
            continue;
          }

          // Check if segment is numeric (array index)
          if (std::all_of (segment.begin (), segment.end (), ::isdigit)) {
            size_t index = std::stoull (segment);
            if (current.is_array () && index < current.size ()) {
              current = current[index];
            } else {
              return defaultValue;
            }
          } else {
            // Object key
            if (current.contains (segment)) {
              current = current[segment];
            } else {
              return defaultValue;
            }
          }
        }

        return current.get<T> ();
      } catch (const std::exception&) {
        return defaultValue;
      }
    }

    // Find item by id in array
    inline std::optional<nlohmann::json> findById (const nlohmann::json& jsonArray,
        const std::string& id, const std::string& idField = "id") {
      try {
        if (jsonArray.is_array ()) {
          for (const auto& item : jsonArray) {
            if (item.contains (idField) && item[idField] == id) {
              return item;
            }
          }
        }
      } catch (const std::exception&) {
        // Return nullopt on any error
      }
      return std::nullopt;
    }

    // Get localized string from your custom format
    inline std::optional<std::string> getLocalizedString (const nlohmann::json& stringsJson,
        const std::string& id, const std::string& locale = "en") {
      try {
        if (stringsJson.contains ("strings") && stringsJson["strings"].is_array ()) {
          auto item = findById (stringsJson["strings"], id);
          if (item && item->contains ("data")) {
            const auto& data = (*item)["data"];
            if (data.contains (locale)) {
              return data[locale].get<std::string> ();
            }
            // Fallback to English if requested locale not found
            if (locale != "en" && data.contains ("en")) {
              return data["en"].get<std::string> ();
            }
          }
        }
      } catch (const std::exception&) {
        // Return nullopt on any error
      }
      return std::nullopt;
    }

    // Get EMAIL from your custom format
    inline std::optional<std::string> getEmail (const nlohmann::json& stringsJson,
        const std::string& id) {
      try {
        if (stringsJson.contains ("strings") && stringsJson["strings"].is_array ()) {
          auto item = findById (stringsJson["strings"], id);
          if (item && item->contains ("data")) {
            const auto& data = (*item)["data"];
            if (data.contains ("email")) {
              return data["email"].get<std::string> ();
            }
          }
        }
      } catch (const std::exception&) {
        // Return nullopt on any error
      }
      return std::nullopt;
    }

    // Get URL from your custom format
    inline std::optional<std::string> getUrl (const nlohmann::json& stringsJson,
        const std::string& id) {
      try {
        if (stringsJson.contains ("strings") && stringsJson["strings"].is_array ()) {
          auto item = findById (stringsJson["strings"], id);
          if (item && item->contains ("data")) {
            const auto& data = (*item)["data"];
            if (data.contains ("url")) {
              return data["url"].get<std::string> ();
            }
          }
        }
      } catch (const std::exception&) {
        // Return nullopt on any error
      }
      return std::nullopt;
    }

    // Get telephone from your custom format
    inline std::optional<std::string> getTel (const nlohmann::json& stringsJson,
        const std::string& id) {
      try {
        if (stringsJson.contains ("strings") && stringsJson["strings"].is_array ()) {
          auto item = findById (stringsJson["strings"], id);
          if (item && item->contains ("data")) {
            const auto& data = (*item)["data"];
            if (data.contains ("tel")) {
              return data["tel"].get<std::string> ();
            }
          }
        }
      } catch (const std::exception&) {
        // Return nullopt on any error
      }
      return std::nullopt;
    }

    // Validate JSON against basic schema
    inline bool validateStructure (const nlohmann::json& json,
        const std::vector<std::string>& requiredKeys) {
      return std::all_of (requiredKeys.begin (), requiredKeys.end (),
          [&json] (const std::string& key) { return json.contains (key); });
    }

    // Pretty print JSON to string
    inline std::string prettyPrint (const nlohmann::json& json, int indent = 2) {
      return json.dump (indent);
    }

    // Merge two JSON objects
    inline nlohmann::json merge (const nlohmann::json& base, const nlohmann::json& overlay) {
      nlohmann::json result = base;
      try {
        result.update (overlay);
      } catch (const std::exception&) {
        // Return base on error
      }
      return result;
    }

    // DEPRECATED: This function requires AssetContext which has been removed.
    // TODO: Refactor to accept IAssetManager parameter when Utils is refactored
    // get Author, Email, Phone, Website, GitHub, ...
    inline std::string getCustomStringSign () {
      // Temporarily disabled - requires IAssetManager injection
      return "Custom strings functionality disabled (pending Utils refactoring)";
    }

  } // namespace json

} // namespace utils
} // namespace dotnamecpp

// MIT License Copyright (c) 2024-2025 Tomáš Mark