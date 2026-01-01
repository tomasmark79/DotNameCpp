#include "StringFormatter.hpp"

#include <algorithm>
#include <ranges>

namespace dotnamecpp::utils {

  std::string StringFormatter::addDots(const std::string &str) const {
    if (str.empty()) {
      return str;
    }

    std::string result;
    result.reserve(str.length() + (str.length() / 3));

    int count = 0;
    for (char it : std::ranges::reverse_view(str)) {
      if (count > 0 && count % 3 == 0) {
        result += '.';
      }
      result += it;
      count++;
    }

    std::ranges::reverse(result);
    return result;
  }

  std::string StringFormatter::removeDots(const std::string &str) const {
    if (str.empty()) {
      return str;
    }

    std::string result;
    result.reserve(str.length());

    for (char ch : str) {
      if (ch != '.') {
        result += ch;
      }
    }

    return result;
  }

} // namespace dotnamecpp::utils