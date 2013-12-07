// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/rotor_lock.hpp"

#include "test/mock_functor.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

using namespace gyros::component;
using namespace testing;
using namespace test;

class component_RotorLock : public ::testing::TestWithParam<size_t> {
};

TEST_F(component_RotorLock, test_functor_not_called_on_destruct) {
  MockFunctor functor;
  EXPECT_CALL(functor, call());

  RotorLock lock(wrap(functor));
}

TEST_F(component_RotorLock, test_functor_not_called_before_destruct) {
  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  RotorLock lock(wrap(functor));
  Mock::VerifyAndClearExpectations(&functor);
}

TEST_F(component_RotorLock, test_functor_not_called_on_move) {
  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  RotorLock lock0(wrap(functor));
  RotorLock lock1(std::move(lock0));
  Mock::VerifyAndClearExpectations(&functor);
}

TEST_F(component_RotorLock, test_functor_not_called_on_destruct_after_move) {
  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(0);

  std::unique_ptr<RotorLock> lock0(new RotorLock(wrap(functor)));
  RotorLock lock1(std::move(*lock0));
  lock0.reset();
  Mock::VerifyAndClearExpectations(&functor);
}

TEST_F(component_RotorLock, test_functor_called_only_once_after_2_moves) {
  MockFunctor functor;
  EXPECT_CALL(functor, call())
      .Times(1);

  RotorLock lock0(wrap(functor));
  RotorLock lock1(std::move(lock0));
  RotorLock lock2(std::move(lock1));
}

