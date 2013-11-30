// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/position_iterator.hpp"

#include <gtest/gtest.h>

#include "test/gyros/components.hpp"

using namespace gyros::component;
using namespace test::gyros::component;
using namespace testing;

class component_PositionIterator : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_P(component_PositionIterator, test_diff_after_stepping_forward) {
  ptrdiff_t steps = GetParam();

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before + steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_stepping_backward) {
  ptrdiff_t steps = GetParam();

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before - steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_preincrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    ++after;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_predecrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    --after;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_increment_assigning) {
  ptrdiff_t steps = GetParam();

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before;
  after += steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_decrement_assigning) {
  ptrdiff_t steps = GetParam();

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before;
  after -= steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_postincrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    after++;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_PositionIterator, test_diff_after_postdecrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    after--;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

ptrdiff_t diffs[] = { -1, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
INSTANTIATE_TEST_CASE_P(DifferenceTests,
                        component_PositionIterator,
                        ::testing::ValuesIn(diffs));

TEST_F(component_PositionIterator, test_equals_operator) {
  auto first = PositionIterator<EmptyComponent>(nullptr);
  auto second = PositionIterator<EmptyComponent>(nullptr);

  ASSERT_TRUE(first == second);
}

TEST_F(component_PositionIterator, test_not_equals_operator) {
  auto first = PositionIterator<EmptyComponent>(nullptr);
  auto second = first + 1;

  ASSERT_TRUE(first != second);
}

TEST_F(component_PositionIterator, test_equal_after_copying) {
  auto first = PositionIterator<EmptyComponent>(nullptr);
  auto second = first;

  ASSERT_EQ(first, second);
}

TEST_F(component_PositionIterator, test_postincrement_returns_new_instance) {
  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto const& after = before++;

  ASSERT_NE(&(before), &(after));
}

TEST_F(component_PositionIterator, test_postincrement_returns_same_value) {
  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto copy = before;
  auto after = before++;

  ASSERT_EQ(copy, after);
}

TEST_F(component_PositionIterator, test_postdecrement_returns_new_instance) {
  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto const& after = before--;

  ASSERT_NE(&(before), &(after));
}

TEST_F(component_PositionIterator, test_postdecrement_returns_same_value) {
  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto copy = before;
  auto after = before--;

  ASSERT_EQ(copy, after);
}

TEST_F(component_PositionIterator, test_preincrement_returns_same_instance) {
  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto const& after = ++before;

  ASSERT_EQ(&(before), &(after));
}

TEST_F(component_PositionIterator, test_predecrement_returns_same_instance) {
  auto before = PositionIterator<EmptyComponent>(nullptr);
  auto const& after = --before;

  ASSERT_EQ(&(before), &(after));
}

