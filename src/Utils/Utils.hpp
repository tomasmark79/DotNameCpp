// MIT License Copyright (c) 2024-2025 Tomáš Mark

#ifndef UTILS_HPP
#define UTILS_HPP

#define CUSTOM_STRINGS_FILE "customstrings.json"

#include "Logger/Logger.hpp"
#include <nlohmann/json.hpp>
#include <Assets/AssetContext.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <execution>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>

// fullfilled from ../cmake/tmplt-assets.cmake)
#ifndef UTILS_ASSET_PATH
  #define UTILS_ASSET_PATH ""
#endif
// fullfilled from ../cmake/tmplt-assets.cmake)
#ifndef UTILS_FIRST_ASSET_FILE
  #define UTILS_FIRST_ASSET_FILE ""
#endif
// fullfilled from ../cmake/tmplt-assets.cmake)
#ifndef UTILS_ASSET_FILES_DIVIDED_BY_COMMAS
  #define UTILS_ASSET_FILES_DIVIDED_BY_COMMAS ""
#endif

// We need to avoid conflicts with other libraries
#ifdef _WIN32
  #include <windows.h>
#elif defined(__APPLE__)
  #include <limits.h>
  #include <mach-o/dyld.h>
#else // Linux
  #include <limits.h>
  #include <unistd.h>
#endif

namespace DotNameUtils {

  namespace FileIO {
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
  } // namespace FileIO

  namespace PathUtils {
    inline std::filesystem::path getParentPath (const std::filesystem::path& filePath) {
      return filePath.parent_path ();
    }

    inline std::string getFileName (const std::filesystem::path& filePath) {
      return filePath.filename ().string ();
    }

    inline bool fileExists (const std::filesystem::path& filePath) {
      return std::filesystem::exists (filePath);
    }

    inline std::filesystem::path getStandalonePath () {
      std::filesystem::path path;

#ifdef _WIN32
      char buffer[MAX_PATH];
      GetModuleFileNameA (NULL, buffer, MAX_PATH);
      path = buffer;
#elif defined(__APPLE__)
      char buffer[PATH_MAX];
      uint32_t bufferSize = PATH_MAX;
      if (_NSGetExecutablePath (buffer, &bufferSize) == 0) {
        path = buffer;
      }
#else
      char buffer[PATH_MAX];
      ssize_t count = readlink ("/proc/self/exe", buffer, PATH_MAX);
      if (count != -1) {
        buffer[count] = '\0';
        path = buffer;
      }
#endif
      return path;
    }

  } // namespace PathUtils

  namespace FileManager {
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
  } // namespace FileManager

  namespace Dots {
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
  } // namespace Dots

  namespace JsonUtils {
    // Load JSON from file
    inline nlohmann::json loadFromFile (const std::filesystem::path& filePath) {
      if (!PathUtils::fileExists (filePath)) {
        throw std::ios_base::failure ("JSON file does not exist: " + filePath.string ());
      }

      try {
        std::string content = FileIO::readFile (filePath);
        return nlohmann::json::parse (content);
      } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error ("JSON parse error in file " + filePath.string () + ": "
                                  + e.what ());
      }
    }

    // Save JSON to file
    inline void saveToFile (const std::filesystem::path& filePath, const nlohmann::json& jsonData,
                            int indent = 2) {
      try {
        std::string jsonString = jsonData.dump (indent);
        FileIO::writeFile (filePath, jsonString);
      } catch (const std::exception& e) {
        throw std::runtime_error ("Failed to save JSON to file " + filePath.string () + ": "
                                  + e.what ());
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
          if (segment.empty ())
            continue;

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
                                                   const std::string& id,
                                                   const std::string& idField = "id") {
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
                                                          const std::string& id,
                                                          const std::string& locale = "en") {
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
      for (const auto& key : requiredKeys) {
        if (!json.contains (key)) {
          return false;
        }
      }
      return true;
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

    // get Author, Email, Phone, Website, GitHub, ...
    inline std::string getCustomStringSign () {
      std::string result;
      try {
        auto customStrings = loadFromFile (AssetContext::getAssetsPath () / CUSTOM_STRINGS_FILE);
        auto authorEn = getLocalizedString (customStrings, "Author", "en");
        auto authorCs = getLocalizedString (customStrings, "Author", "cs");
        auto email = getEmail (customStrings, "Email");
        auto phone = getTel (customStrings, "Phone");
        auto website = getUrl (customStrings, "Website");
        auto github = getUrl (customStrings, "GitHub");
        auto linkedin = getUrl (customStrings, "LinkedIn");
        auto discord = getUrl (customStrings, "Discord");

        if (email)
          result += "Email: " + *email + "\n";
        else
          result += "No email provided.\n";

        if (phone)
          result += "Phone: " + *phone + "\n";
        else
          result += "No phone provided.\n";

        if (website)
          result += "Website: " + *website + "\n";
        else
          result += "No website provided.\n";

        if (github)
          result += "GitHub: " + *github + "\n";
        else
          result += "No GitHub provided.\n";

        if (linkedin)
          result += "LinkedIn: " + *linkedin + "\n";
        else
          result += "No LinkedIn provided.\n";

        if (discord)
          result += "Discord: " + *discord;
        else
          result += "No Discord provided.";

      } catch (const std::exception& e) {
        result = "Error loading custom strings: " + std::string (e.what ());
      }
      return result;
    }

  } // namespace JsonUtils

  namespace Performance {

    inline double heavy_calculation (double x) {
      double result = x;
      for (int i = 0; i < 1000; i++) {
        result = sin (result) + cos (result);
      }
      return result;
    }

    inline void parUnseqHeavyCalculation (double initialValue) {
      LOG_I_STREAM << "Parallel unsequenced heavy calculation" << std::endl;
      const size_t N = 500'000; std::vector<double> data (N); iota (data.begin (), data.end (), initialValue);

#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L && !defined(__APPLE__)
          auto runTest
          = [&] (auto policy, const std::string& name) {
              iota (data.begin (), data.end (), initialValue);
              auto start = std::chrono::high_resolution_clock::now ();
              for_each (policy, data.begin (), data.end (),
                        [] (double& x) { x = heavy_calculation (x); });
              auto end = std::chrono::high_resolution_clock::now ();
              LOG_I_STREAM
                  << name << ": "
                  << std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count ()
                  << " ms\n";
            };

      runTest (std::execution::seq, "Sequential");
      runTest (std::execution::par, "Parallel");
      runTest (std::execution::par_unseq, "Parallel+Vectorization");
#else
          // Fallback for platforms without execution policy support (like macOS)
          auto runTest
          = [&] (const std::string& name) {
              iota (data.begin (), data.end (), initialValue);
              auto start = std::chrono::high_resolution_clock::now ();
              for_each (data.begin (), data.end (), [] (double& x) { x = heavy_calculation (x); });
              auto end = std::chrono::high_resolution_clock::now ();
              LOG_I_STREAM
                  << name << ": "
                  << std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count ()
                  << " ms\n";
            };

      runTest ("Sequential (execution policies not supported)");
      LOG_I_STREAM << "Note: Parallel execution policies not available on this platform"
                   << std::endl;
#endif
    }

    inline void simpleCpuBenchmark (std::chrono::microseconds duration
                                    = std::chrono::microseconds (1000000)) {
      LOG_I_STREAM << "Simple CPU benchmark" << std::endl;
      LOG_I_STREAM << "CPU cores: " << std::thread::hardware_concurrency () << std::endl;
      auto start = std::chrono::high_resolution_clock::now ();
      auto end = start + duration;
      long int iterations = 0;
      while (std::chrono::high_resolution_clock::now () < end) {
#if defined(__GNUC__) || defined(__clang__)
        asm volatile ("nop");
#elif defined(_MSC_VER)
        __nop ();
#else
        // Fallback for unsupported platforms
        std::this_thread::yield ();
#endif
        ++iterations;
      }
      auto actualEnd = std::chrono::high_resolution_clock::now ();
      auto actualDuration
          = std::chrono::duration_cast<std::chrono::milliseconds> (actualEnd - start);
      std::string iterationsStr = std::to_string (iterations);
      LOG_I_STREAM << "CPU benchmark duration: " << actualDuration.count () << " ms" << std::endl;
      LOG_I_STREAM << "Total iterations: " << Dots::addDots (iterationsStr) << std::endl;
    }
  } // namespace Performance

} // namespace DotNameUtils

#endif // UTILS_HPP