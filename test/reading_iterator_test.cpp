// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/iterator.hpp"
#include "gyros/component/rotor_lock.hpp"

#include <gtest/gtest.h>

#include "test/gyros/components.hpp"
#include "test/gyros/component/fake_lock.hpp"
#include "test/mock_functor.hpp"

using namespace gyros::component;
using namespace test::gyros::component;
using namespace test;
using namespace testing;

typedef ReadingIterator<EmptyComponent, FakeLock> TestedIterator;
typedef ReadingIterator<MockComponent, FakeLock> CallTestingIterator;
typedef ReadingIterator<EmptyComponent, RotorLock> LockTestingIterator;

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

  ReadingIterator<TestedComponent, FakeLock> it(components + index,
                                                read_offset,
                                                FakeLock());
  ASSERT_EQ(expected, it->member_);
}

INSTANTIATE_TEST_CASE_P(
    DiffTests,
    component_ReadingIterator,
    ValuesIn((ptrdiff_t[]) { -1, 0, 1, 8, 2048 })
    );

TEST_F(component_ReadingIterator, test_dereference) {
  EmptyComponent component;
  auto it = TestedIterator(&component, 0, FakeLock());
  auto const& dereferenced = *it;

  ASSERT_EQ(&(component), &(dereferenced));
}

TEST_F(component_ReadingIterator, test_method_invocation) {
  MockComponent component;
  auto it = CallTestingIterator(&component, 0, FakeLock());

  EXPECT_CALL(component, method());
  it->method();
}

TEST_F(component_ReadingIterator, test_lock_destoyed_with_last_iterator) {
  EmptyComponent component;
  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  auto it0 = LockTestingIterator(&component, 0, RotorLock(wrap(functor)));
  {
    auto it1 = it0;
    auto it2 = it1;
    auto it3 = it0;
  }
  Mock::VerifyAndClearExpectations(&functor);
  EXPECT_CALL(functor, call())
      .Times(1);
}

