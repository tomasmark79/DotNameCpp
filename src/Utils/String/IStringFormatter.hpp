#pragma once

#include <string>

namespace dotnamecpp::utils {

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
     * 
     * @param str 
     * @return std::string 
     */
    [[nodiscard]]
    virtual std::string addDots (const std::string& str) const = 0;

    /**
     * @brief Remove thousand separators (dots) from numeric string
     * 
     * @param str 
     * @return std::string 
     */
    [[nodiscard]]
    virtual std::string removeDots (const std::string& str) const = 0;
  };

} // namespace dotnamecpp::utils