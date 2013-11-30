// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/iterator.hpp"

#include <gtest/gtest.h>

#include "test/gyros/components.hpp"
#include "test/gyros/component/fake_lock.hpp"

using namespace gyros::component;
using namespace test::gyros::component;
using namespace testing;

typedef ReadingIterator<EmptyComponent, FakeLock<>> TestedIterator;

class component_ReadingIterator : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_P(component_ReadingIterator, test_diff_after_stepping_forward) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before + steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_stepping_backward) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before - steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_preincrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    ++after;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_predecrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    --after;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_increment_assigning) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before;
  after += steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_decrement_assigning) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before;
  after -= steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_postincrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    after++;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_ReadingIterator, test_diff_after_postdecrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, FakeLock<>());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    after--;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

INSTANTIATE_TEST_CASE_P(
    DiffTests,
    component_ReadingIterator,
    ValuesIn((ptrdiff_t[]) { -1,0,1,2,4,8,16,32,64,128,256,512,1024,2048 })
    );

TEST_F(component_ReadingIterator, test_equals_operator) {
  auto first = TestedIterator(nullptr, FakeLock<>());
  auto second = TestedIterator(nullptr, FakeLock<>());

  ASSERT_TRUE(first == second);
}

TEST_F(component_ReadingIterator, test_not_equals_operator) {
  auto first = TestedIterator(nullptr, FakeLock<>());
  auto second = first + 1;

  ASSERT_TRUE(first != second);
}

TEST_F(component_ReadingIterator, test_equal_after_copying) {
  auto first = TestedIterator(nullptr, FakeLock<>());
  auto second = first;

  ASSERT_EQ(first, second);
}

TEST_F(component_ReadingIterator, test_postincrement_returns_new_instance) {
  auto before = TestedIterator(nullptr, FakeLock<>());
  auto const& after = before++;

  ASSERT_NE(&(before), &(after));
}

TEST_F(component_ReadingIterator, test_postincrement_returns_same_value) {
  auto before = TestedIterator(nullptr, FakeLock<>());
  auto copy = before;
  auto after = before++;

  ASSERT_EQ(copy, after);
}

TEST_F(component_ReadingIterator, test_postdecrement_returns_new_instance) {
  auto before = TestedIterator(nullptr, FakeLock<>());
  auto const& after = before--;

  ASSERT_NE(&(before), &(after));
}

TEST_F(component_ReadingIterator, test_postdecrement_returns_same_value) {
  auto before = TestedIterator(nullptr, FakeLock<>());
  auto copy = before;
  auto after = before--;

  ASSERT_EQ(copy, after);
}

TEST_F(component_ReadingIterator, test_preincrement_returns_same_instance) {
  auto before = TestedIterator(nullptr, FakeLock<>());
  auto const& after = ++before;

  ASSERT_EQ(&(before), &(after));
}

TEST_F(component_ReadingIterator, test_predecrement_returns_same_instance) {
  auto before = TestedIterator(nullptr, FakeLock<>());
  auto const& after = --before;

  ASSERT_EQ(&(before), &(after));
}

TEST_F(component_ReadingIterator, test_dereference) {
  EmptyComponent component;
  auto it = TestedIterator(&component, FakeLock<>());
  auto const& dereferenced = *it;

  ASSERT_EQ(&(component), &(dereferenced));
}


