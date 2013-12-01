// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/iterator.hpp"

#include <gtest/gtest.h>

#include "test/gyros/components.hpp"
#include "test/gyros/component/fake_lock.hpp"
#include "test/mock_functor.hpp"

using namespace gyros::component;
using namespace test::gyros::component;
using namespace test;
using namespace testing;

typedef WritingIterator<EmptyComponent, FakeLock> TestedIterator;

class component_WritingIterator : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_P(component_WritingIterator, test_diff_after_stepping_forward) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before + steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_stepping_backward) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before - steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_preincrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    ++after;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_predecrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    --after;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_increment_assigning) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before;
  after += steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_decrement_assigning) {
  ptrdiff_t steps = GetParam();

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before;
  after -= steps;

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_postincrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    after++;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(steps, diff);
}

TEST_P(component_WritingIterator, test_diff_after_postdecrementing) {
  ptrdiff_t steps = GetParam();
  if (steps < 0) return;

  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto after = before;
  for (ptrdiff_t i = 0; i < steps; ++i) {
    after--;
  }

  ptrdiff_t diff = after - before;
  ASSERT_EQ(-steps, diff);
}

TEST_P(component_WritingIterator, test_write_offset) {
  typedef OneMemberComponent<size_t> TestedComponent;

  ptrdiff_t write_offset = GetParam();
  size_t length = abs(write_offset) + 1;
  TestedComponent components[length];

  size_t index = (length - write_offset) % length;
  size_t original_value =
      components[index].member_ = std::numeric_limits<size_t>::max();

  auto it = WritingIterator<TestedComponent, FakeLock>(components + index,
                                                       write_offset,
                                                       FakeLock());
  it.visit(
      [] (TestedComponent const& source, TestedComponent &target) {
        target.member_ = source.member_ - 1;
      });

  ASSERT_EQ(original_value - 1, components[index + write_offset].member_);
}

INSTANTIATE_TEST_CASE_P(
    DiffTests,
    component_WritingIterator,
    ValuesIn((ptrdiff_t[]) { -1, 0, 1, 8, 2048 })
    );

TEST_F(component_WritingIterator, test_equals_operator) {
  auto first = TestedIterator(nullptr, 0, FakeLock());
  auto second = TestedIterator(nullptr, 0, FakeLock());

  ASSERT_TRUE(first == second);
}

TEST_F(component_WritingIterator, test_not_equals_operator) {
  auto first = TestedIterator(nullptr, 0, FakeLock());
  auto second = first + 1;

  ASSERT_TRUE(first != second);
}

TEST_F(component_WritingIterator, test_equal_after_copying) {
  auto first = TestedIterator(nullptr, 0, FakeLock());
  auto second = first;

  ASSERT_EQ(first, second);
}

TEST_F(component_WritingIterator, test_postincrement_returns_new_instance) {
  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto const& after = before++;

  ASSERT_NE(&(before), &(after));
}

TEST_F(component_WritingIterator, test_postincrement_returns_same_value) {
  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto copy = before;
  auto after = before++;

  ASSERT_EQ(copy, after);
}

TEST_F(component_WritingIterator, test_postdecrement_returns_new_instance) {
  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto const& after = before--;

  ASSERT_NE(&(before), &(after));
}

TEST_F(component_WritingIterator, test_postdecrement_returns_same_value) {
  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto copy = before;
  auto after = before--;

  ASSERT_EQ(copy, after);
}

TEST_F(component_WritingIterator, test_preincrement_returns_same_instance) {
  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto const& after = ++before;

  ASSERT_EQ(&(before), &(after));
}

TEST_F(component_WritingIterator, test_predecrement_returns_same_instance) {
  auto before = TestedIterator(nullptr, 0, FakeLock());
  auto const& after = --before;

  ASSERT_EQ(&(before), &(after));
}

TEST_F(component_WritingIterator, test_dereference) {
  EmptyComponent component;
  auto it = TestedIterator(&component, 0, FakeLock());
  auto const& dereferenced = *it;

  ASSERT_EQ(&(component), &(dereferenced));
}

TEST_F(component_WritingIterator, test_last_lock_destoyed_with_last_iterator) {
  EmptyComponent component;
  MockFunctor functor;
  {
    auto it0 = TestedIterator(&component, 0, FakeLock(wrap(functor)));
    {
      auto it1 = it0;
      {
        auto it2 = it1;
        Mock::VerifyAndClearExpectations(&functor);
      }
      Mock::VerifyAndClearExpectations(&functor);
    }
    Mock::VerifyAndClearExpectations(&functor);
    EXPECT_CALL(functor, call());
  }
}

