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

TEST_P(component_WritingIterator, test_write_offset) {
  typedef OneMemberComponent<size_t> TestedComponent;

  ptrdiff_t write_offset = GetParam();
  size_t length = abs(write_offset) + 1;
  TestedComponent components[length];

  size_t index = (length - write_offset) % length;
  size_t original_value =
      components[index].member_ = std::numeric_limits<size_t>::max();

  WritingIterator<TestedComponent, FakeLock> it(components + index,
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
