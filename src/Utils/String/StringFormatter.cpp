#include "StringFormatter.hpp"

namespace dotnamecpp {
  namespace utils {

    std::string StringFormatter::addDots (const std::string& str) const {
      if (str.empty ()) {
        return str;
      }

      std::string result;
      result.reserve (str.length () + (str.length () / 3)); // Pre-allocate

      for (size_t i = 0; i < str.length (); ++i) {
        result += str[i];
        // Add dot every 3 digits from the right (except at the end)
        if ((str.length () - i - 1) % 3 == 0 && i != str.length () - 1) {
          result += '.';
        }
      }

      return result;
    }

    std::string StringFormatter::removeDots (const std::string& str) const {
      if (str.empty ()) {
        return str;
      }

      std::string result;
      result.reserve (str.length ());

      for (char ch : str) {
        if (ch != '.') {
          result += ch;
        }
      }

      return result;
    }

  } // namespace utils
} // namespace dotnamecpp
