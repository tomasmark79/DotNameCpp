#pragma once

#include <Utils/String/IStringFormatter.hpp>

namespace dotnamecpp::utils {

  class StringFormatter final : public IStringFormatter {
  public:
    StringFormatter () = default;
    ~StringFormatter () override = default;

    StringFormatter (const StringFormatter&) = delete;
    StringFormatter& operator= (const StringFormatter&) = delete;
    StringFormatter (StringFormatter&&) = delete;
    StringFormatter& operator= (StringFormatter&&) = delete;

    [[nodiscard]]
    std::string addDots (const std::string& str) const override;
    [[nodiscard]]
    std::string removeDots (const std::string& str) const override;
  };

} // namespace dotnamecpp::utils