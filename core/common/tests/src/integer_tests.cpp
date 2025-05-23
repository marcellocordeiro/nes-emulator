#include <utility>

#include <catch2/catch_template_test_macros.hpp>

#include "lib/integer.hpp"

template<typename Lhs, typename Rhs, typename Op>
concept supports_op = requires(Lhs a, Rhs b) {
  { Op{}(a, b) } -> std::same_as<Lhs>;
  { Op{}(b, a) } -> std::same_as<Lhs>;
};

template <typename Lhs, typename Rhs>
concept supports_all_binary_ops =
  supports_op<Lhs, Rhs, std::plus<>> &&
  supports_op<Lhs, Rhs, std::minus<>> &&
  supports_op<Lhs, Rhs, std::multiplies<>> &&
  supports_op<Lhs, Rhs, std::divides<>> &&
  supports_op<Lhs, Rhs, std::bit_and<>> &&
  supports_op<Lhs, Rhs, std::bit_or<>> &&
  supports_op<Lhs, Rhs, std::bit_xor<>>;

template <typename Lhs, typename Rhs>
concept supports_comparisons = requires(Lhs a, Rhs b) {
  { a <=> b };
};

static_assert(supports_comparisons<u8, u8>);
static_assert(supports_comparisons<u8, int>);
static_assert(supports_comparisons<int, u8>);
static_assert(supports_comparisons<unsigned int, u8>);
static_assert(!supports_comparisons<u16, u8>);

TEMPLATE_TEST_CASE(
  "binary ops",
  "[test][sane_integer][binary_ops]",
  (std::pair<u8, u8>),
  (std::pair<u16, u16>),
  (std::pair<u32, u32>),
  (std::pair<u8, int>),
  (std::pair<u16, int>),
  (std::pair<u32, int>),
  (std::pair<u8, unsigned int>),
  (std::pair<u16, unsigned int>),
  (std::pair<u32, unsigned int>)
) {
  using Lhs = TestType::first_type;
  using Rhs = TestType::second_type;

  static_assert(supports_all_binary_ops<Lhs, Lhs>);
  static_assert(supports_all_binary_ops<Lhs, Rhs>);
}

TEMPLATE_TEST_CASE(
  "mixed types binary ops are not supported",
  "[test][sane_integer][binary_ops]",
  (std::pair<u8, u16>),
  (std::pair<u8, u32>),
  (std::pair<u16, u32>)
) {
  using Lhs = TestType::first_type;
  using Rhs = TestType::second_type;

  static_assert(!supports_all_binary_ops<Lhs, Rhs>);
}
