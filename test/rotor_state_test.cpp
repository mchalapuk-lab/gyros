// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/rotor_state.hpp"
#include "gyros/component/rotor_lock.hpp"

#include <gtest/gtest.h>

#include "test/gyros/components.hpp"
#include "test/gyros/component/fake_lock.hpp"
#include "test/mock_functor.hpp"

using namespace gyros::component;
using namespace test::gyros::component;
using namespace test;
using namespace testing;

class component_RotorState : public ::testing::TestWithParam<ptrdiff_t> {
};

struct UnusedType {
}; // struct UnusedType
UnusedType unused;

TEST_F(component_RotorState, test_lock_destoyed_with_last_copy_of_ro_state) {
  typedef ReadOnlyRotorState<UnusedType, UnusedType, UnusedType, RotorLock>
      TestedState;

  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  TestedState state0(unused, unused, unused, 0, RotorLock(wrap(functor)));
  {
    auto state1 = state0;
    auto state2 = state1;
    auto state3 = state0;
  }
  Mock::VerifyAndClearExpectations(&functor);
  EXPECT_CALL(functor, call())
      .Times(1);
}

TEST_F(component_RotorState, test_lock_destoyed_with_last_copy_of_rw_state) {
  typedef ReadWriteRotorState<Unused, Unused, Unused, RotorLock> TestedState;

  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  TestedState state0(unused, unused, unused, 0, 0, RotorLock(wrap(functor)));
  {
    auto state1 = state0;
    auto state2 = state1;
    auto state3 = state0;
  }
  Mock::VerifyAndClearExpectations(&functor);
  EXPECT_CALL(functor, call())
      .Times(1);
}

