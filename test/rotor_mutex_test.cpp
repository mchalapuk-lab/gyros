// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/rotor_mutex.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace gyros::component;
using namespace testing;

class component_RotorMutex : public ::testing::TestWithParam<size_t> {
};

TEST_F(component_RotorMutex,
       test_same_index_when_acquiring_readonly_twice) {
  RotorMutex<3> mutex;
  size_t read_index0, read_index1, unused;
  auto lock0 = mutex.acquireReadOnly(&read_index0, &unused);
  auto lock1 = mutex.acquireReadOnly(&read_index1, &unused);
  ASSERT_EQ(read_index0, read_index1);
}

TEST_F(component_RotorMutex,
       test_same_state_version_when_acquiring_readonly_twice) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  auto lock0 = mutex.acquireReadOnly(&unused, &state_version0);
  auto lock1 = mutex.acquireReadOnly(&unused, &state_version1);
  ASSERT_EQ(state_version0, state_version1);
}

TEST_F(component_RotorMutex,
       test_same_index_when_acquiring_readonly_thrice) {
  RotorMutex<3> mutex;
  size_t read_index0, read_index1, read_index2, unused;
  auto lock0 = mutex.acquireReadOnly(&read_index0, &unused);
  auto lock1 = mutex.acquireReadOnly(&read_index1, &unused);
  auto lock2 = mutex.acquireReadOnly(&read_index2, &unused);
  ASSERT_EQ(read_index0, read_index1);
  ASSERT_EQ(read_index0, read_index2);
}

TEST_F(component_RotorMutex,
       test_same_state_version_when_acquiring_readonly_thrice) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, state_version2, unused;
  auto lock0 = mutex.acquireReadOnly(&unused, &state_version0);
  auto lock1 = mutex.acquireReadOnly(&unused, &state_version1);
  auto lock2 = mutex.acquireReadOnly(&unused, &state_version2);
  ASSERT_EQ(state_version0, state_version1);
  ASSERT_EQ(state_version0, state_version2);
}

TEST_F(component_RotorMutex,
       test_same_read_index_when_acquiring_readonly_and_readwrite) {
  RotorMutex<3> mutex;
  size_t read_index0, read_index1, unused;
  auto lock0 = mutex.acquireReadOnly(&read_index0, &unused);
  auto lock1 = mutex.acquireReadWrite(&read_index1, &unused, &unused);
  ASSERT_EQ(read_index0, read_index1);
}

TEST_F(component_RotorMutex,
       test_same_state_version_when_acquiring_readonly_and_readwrite) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  auto lock0 = mutex.acquireReadOnly(&unused, &state_version0);
  auto lock1 = mutex.acquireReadWrite(&unused, &state_version1, &unused);
  ASSERT_EQ(state_version0, state_version1);
}

TEST_F(component_RotorMutex,
       test_same_read_index_when_acquiring_readwrite_and_readonly) {
  RotorMutex<3> mutex;
  size_t read_index0, read_index1, unused;
  auto lock1 = mutex.acquireReadWrite(&read_index1, &unused, &unused);
  auto lock0 = mutex.acquireReadOnly(&read_index0, &unused);
  ASSERT_EQ(read_index0, read_index1);
}

TEST_F(component_RotorMutex,
       test_same_state_version_when_acquiring_readwrite_and_readonly) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  auto lock1 = mutex.acquireReadWrite(&unused, &state_version1, &unused);
  auto lock0 = mutex.acquireReadOnly(&unused, &state_version0);
  ASSERT_EQ(state_version0, state_version1);
}

TEST_F(component_RotorMutex,
       test_different_read_and_write_indices_when_acquiring_readwrite) {
  RotorMutex<3> mutex;
  size_t read_index, write_index, unused;
  auto lock0 = mutex.acquireReadWrite(&read_index, &unused, &write_index);
  ASSERT_NE(read_index, write_index);
}

TEST_F(component_RotorMutex,
       test_different_write_index_when_acquiring_rw_after_releasing_rw_lock) {
  RotorMutex<3> mutex;
  size_t write_index0, write_index1, unused;
  {
    auto lock0 = mutex.acquireReadWrite(&unused, &unused, &write_index0);
  }
  auto lock1 = mutex.acquireReadWrite(&unused, &unused, &write_index1);
  ASSERT_NE(write_index0, write_index1);
}

TEST_F(component_RotorMutex,
       test_different_read_index_when_acquiring_rw_after_releasing_rw_lock) {
  RotorMutex<3> mutex;
  size_t read_index0, read_index1, unused;
  {
    auto lock0 = mutex.acquireReadWrite(&read_index0, &unused, &unused);
  }
  auto lock1 = mutex.acquireReadWrite(&read_index1, &unused, &unused);
  ASSERT_NE(read_index0, read_index1);
}

TEST_F(component_RotorMutex,
       test_different_state_version_when_acquiring_rw_after_releasing_rw_lock) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  {
    auto lock0 = mutex.acquireReadWrite(&unused, &state_version0, &unused);
  }
  auto lock1 = mutex.acquireReadWrite(&unused, &state_version1, &unused);
  ASSERT_NE(state_version0, state_version1);
}

TEST_F(component_RotorMutex,
       test_same_read_index_after_full_rw_cycle) {
  RotorMutex<3> mutex;
  size_t read_index0, read_index1, unused;
  {
    auto lock0 = mutex.acquireReadWrite(&read_index0, &unused, &unused);
  }
  {
    auto lock1 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  {
    auto lock2 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  auto lock3 = mutex.acquireReadWrite(&read_index1, &unused, &unused);
  ASSERT_EQ(read_index0, read_index1);
}

TEST_F(component_RotorMutex,
       test_same_write_index_after_full_rw_cycle) {
  RotorMutex<3> mutex;
  size_t write_index0, write_index1, unused;
  {
    auto lock0 = mutex.acquireReadWrite(&unused, &unused, &write_index0);
  }
  {
    auto lock1 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  {
    auto lock2 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  auto lock3 = mutex.acquireReadWrite(&unused, &unused, &write_index1);
  ASSERT_EQ(write_index0, write_index1);
}
TEST_F(component_RotorMutex,
       test_shorter_rw_cycle_if_not_releasing_ro_lock) {
  RotorMutex<3> mutex;
  size_t write_index0, write_index1, unused;
  auto lock0 = mutex.acquireReadOnly(&unused, &unused);
  {
    auto lock1 = mutex.acquireReadWrite(&unused, &unused, &write_index0);
  }
  {
    auto lock2 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  auto lock3 = mutex.acquireReadWrite(&unused, &unused, &write_index1);
  ASSERT_EQ(write_index0, write_index1);
}

TEST_F(component_RotorMutex,
       test_state_version_diff_after_full_rw_cycle) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  {
    auto lock0 = mutex.acquireReadWrite(&unused, &state_version0, &unused);
  }
  {
    auto lock1 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  {
    auto lock2 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  auto lock3 = mutex.acquireReadWrite(&unused, &state_version1, &unused);
  ASSERT_EQ(state_version0 + 3, state_version1);
}

TEST_F(component_RotorMutex,
       test_same_version_when_relock_ro_with_rw_locked_between_ro_lock_unlock) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  std::unique_ptr<RotorLock> lock0(
      new RotorLock(mutex.acquireReadOnly(&unused, &state_version0))
      );
  auto lock1 = mutex.acquireReadWrite(&unused, &unused, &unused);
  lock0.reset();
  auto lock2 = mutex.acquireReadOnly(&unused, &state_version1);
  ASSERT_EQ(state_version0, state_version1);
}

TEST_F(component_RotorMutex,
       test_same_version_when_relock_ro_with_rw_unlocked_between_ro_lock_unlo) {
  RotorMutex<3> mutex;
  size_t state_version0, state_version1, unused;
  std::unique_ptr<RotorLock> lock0(
      new RotorLock(mutex.acquireReadOnly(&unused, &state_version0))
      );
  {
    auto lock1 = mutex.acquireReadWrite(&unused, &unused, &unused);
  }
  lock0.reset();
  auto lock2 = mutex.acquireReadOnly(&unused, &state_version1);
  ASSERT_NE(state_version0, state_version1);
}

TEST_F(component_RotorMutex,
       test_throw_when_full_locking_rw) {
  RotorMutex<3> mutex;
  size_t unused;
  auto lock0 = mutex.acquireReadWrite(&unused, &unused, &unused);
  auto lock1 = mutex.acquireReadWrite(&unused, &unused, &unused);
  ASSERT_THROW({ mutex.acquireReadWrite(&unused, &unused, &unused); },
               std::logic_error);
}


