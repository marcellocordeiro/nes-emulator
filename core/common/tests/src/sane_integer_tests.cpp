#include <utility>

#include <catch2/catch_template_test_macros.hpp>

#include "lib/sane_integer.hpp"

TEMPLATE_TEST_CASE("signed", "[test][signed]", (std::pair<u8_s, i8_s>), (std::pair<u16_s, i16_s>)) {
  using Lhs = typename TestType::first_type;
  using Rhs = typename TestType::second_type;

  const auto lhs = Lhs{1};
  const auto rhs = Rhs{-1};

  const auto result = lhs.add_signed(rhs);

  REQUIRE(result == 0);
}
