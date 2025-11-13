#pragma once

#include <string>

namespace dotname {

  /**
 * @brief Interface for string formatting operations
 * 
 * Provides utilities for string manipulation and formatting.
 */
  class IStringFormatter {
  public:
    virtual ~IStringFormatter () = default;

    /**
     * @brief Add thousand separators (dots) to numeric string
     * @param str Input numeric string (e.g., "1000000")
     * @return Formatted string (e.g., "1.000.000")
     */
    [[nodiscard]] virtual std::string addDots (const std::string& str) const = 0;

    /**
     * @brief Remove thousand separators (dots) from numeric string
     * @param str Input formatted string (e.g., "1.000.000")
     * @return Plain numeric string (e.g., "1000000")
     */
    [[nodiscard]] virtual std::string removeDots (const std::string& str) const = 0;
  };

} // namespace dotname
