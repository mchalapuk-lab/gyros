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

typedef ReadingIterator<EmptyComponent, FakeSharedLock> TestedIterator;
typedef ReadingIterator<MockComponent, FakeSharedLock> CallTestingIterator;
typedef ReadingIterator<EmptyComponent, FakeLock> LockTestingIterator;

class component_ReadingIterator : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_F(component_ReadingIterator, test_dereference) {
  EmptyComponent component;
  auto it = TestedIterator(&component, FakeSharedLock());
  auto const& dereferenced = *it;

  ASSERT_EQ(&(component), &(dereferenced));
}

TEST_F(component_ReadingIterator, test_method_invocation) {
  MockComponent component;
  auto it = CallTestingIterator(&component, FakeSharedLock());

  EXPECT_CALL(component, method());
  it->method();
}

TEST_F(component_ReadingIterator, test_lock_destoyed_with_last_iterator) {
  EmptyComponent component;
  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  auto it0 = LockTestingIterator(&component, FakeLock(wrap(functor)));
  {
    auto it1 = it0;
    auto it2 = it1;
    auto it3 = it0;
  }
  Mock::VerifyAndClearExpectations(&functor);
  EXPECT_CALL(functor, call())
      .Times(1);
}

