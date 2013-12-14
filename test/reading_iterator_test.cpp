// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/iterator.hpp"

#include <gtest/gtest.h>

#include "test/gyros/components.hpp"

using namespace gyros::component;
using namespace test::gyros::component;
using namespace test;
using namespace testing;

typedef ReadingIterator<EmptyComponent> TestedIterator;
typedef ReadingIterator<MockComponent> CallTestingIterator;

class component_ReadingIterator : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_P(component_ReadingIterator, test_read_offset) {
  typedef OneMemberComponent<size_t> TestedComponent;

  ptrdiff_t read_offset = GetParam();
  size_t length = abs(read_offset) + 1;
  TestedComponent components[length];

  size_t index = (length - read_offset) % length;
  size_t expected = components[index + read_offset].member_
      = std::numeric_limits<size_t>::max();

  ReadingIterator<TestedComponent> it(components + index, read_offset);
  ASSERT_EQ(expected, it->member_);
}

INSTANTIATE_TEST_CASE_P(
    DiffTests,
    component_ReadingIterator,
    ValuesIn((ptrdiff_t[]) { -1, 0, 1, 8, 2048 })
    );

TEST_F(component_ReadingIterator, test_dereference) {
  EmptyComponent component;
  auto it = TestedIterator(&component, 0);
  auto const& dereferenced = *it;

  ASSERT_EQ(&(component), &(dereferenced));
}

TEST_F(component_ReadingIterator, test_method_invocation) {
  MockComponent component;
  auto it = CallTestingIterator(&component, 0);

  EXPECT_CALL(component, method());
  it->method();
}

