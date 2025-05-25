#include <utility>

#include <catch2/catch_template_test_macros.hpp>

#include "lib/integer.hpp"

template <typename Lhs, typename Rhs, typename Op>
concept supports_op = requires(Lhs a, Rhs b) {
  {
    Op{}(a, b)
  } -> std::same_as<Lhs>;
  {
    Op{}(b, a)
  } -> std::same_as<Lhs>;
};

template <typename Lhs, typename Rhs>
concept supports_all_binary_ops = supports_op<Lhs, Rhs, std::plus<>>
  && supports_op<Lhs, Rhs, std::minus<>>
  && supports_op<Lhs, Rhs, std::multiplies<>>
  && supports_op<Lhs, Rhs, std::divides<>>
  && supports_op<Lhs, Rhs, std::bit_and<>>
  && supports_op<Lhs, Rhs, std::bit_or<>>
  && supports_op<Lhs, Rhs, std::bit_xor<>>;

template <typename Lhs, typename Rhs>
concept supports_comparisons = requires(Lhs a, Rhs b) {
  {
    a <=> b
  };
};

static_assert(supports_comparisons<u8_s, u8_s>);
static_assert(supports_comparisons<u8_s, int>);
static_assert(supports_comparisons<int, u8_s>);
static_assert(supports_comparisons<unsigned int, u8_s>);
static_assert(!supports_comparisons<u16_s, u8_s>);

TEMPLATE_TEST_CASE(
  "binary ops",
  "[test][sane_integer][binary_ops]",
  (std::pair<u8_s, u8_s>),
  (std::pair<u16_s, u16_s>),
  (std::pair<u32_s, u32_s>),
  (std::pair<u8_s, int>),
  (std::pair<u16_s, int>),
  (std::pair<u32_s, int>),
  (std::pair<u8_s, unsigned int>),
  (std::pair<u16_s, unsigned int>),
  (std::pair<u32_s, unsigned int>)
) {
  using Lhs = TestType::first_type;
  using Rhs = TestType::second_type;

  static_assert(supports_all_binary_ops<Lhs, Lhs>);
  static_assert(supports_all_binary_ops<Lhs, Rhs>);
}

TEMPLATE_TEST_CASE(
  "mixed types binary ops are not supported",
  "[test][sane_integer][binary_ops]",
  (std::pair<u8_s, u16_s>),
  (std::pair<u8_s, u32_s>),
  (std::pair<u16_s, u32_s>)
) {
  using Lhs = TestType::first_type;
  using Rhs = TestType::second_type;

  static_assert(!supports_all_binary_ops<Lhs, Rhs>);
}
