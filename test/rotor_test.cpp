// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/rotor.hpp"

#include "test/gyros/components.hpp"
#include "test/gyros/component/mock_visitor.hpp"
#include "test/gyros/visitor_wrapper.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace gyros::component;
using namespace test::gyros;
using namespace test::gyros::component;
using namespace testing;

class component_Rotor : public ::testing::TestWithParam<size_t> {
  void SetUp() {
    CountingComponent::resetCounters();
  }
};

TEST_F(component_Rotor, test_capacity) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  ASSERT_EQ(1, rotor.capacity());
}

TEST_F(component_Rotor, test_constructor_called_once) {
  auto rotor = RotorBuilder<CountingComponent>()
      .emplace<CountingComponent>()
      .build();

  ASSERT_EQ(1, CountingComponent::constructor_calls);
}

TEST_F(component_Rotor, test_destructor_called_once_after_destroying_rotor) {
  {
    auto rotor = RotorBuilder<CountingComponent>()
        .emplace<CountingComponent>()
        .build();
  }
  ASSERT_EQ(1, CountingComponent::destructor_calls);
}

TEST_F(component_Rotor, test_destructor_not_called_when_rotor_not_destroyed) {
  auto rotor = RotorBuilder<CountingComponent>()
      .emplace<CountingComponent>()
      .build();

  ASSERT_EQ(0, CountingComponent::destructor_calls);
}

TEST_F(component_Rotor, test_begin_end_pos_iter_diff_equal_capacity) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  ptrdiff_t diff = rotor.end() - rotor.begin();
  ASSERT_EQ(rotor.capacity(), diff);
}

TEST_F(component_Rotor, test_dereferencing_after_upgrade_readonly) {
  typedef OneMemberComponent<int> TestedComponent;
  int value = 2;

  auto rotor = RotorBuilder<TestedComponent>()
      .emplace<TestedComponent>(value)
      .build();

  auto state = rotor.acquireReadOnly();
  auto it = state.upgrade(rotor.begin());
  auto const& dereferenced = *it;
  ASSERT_EQ(value, dereferenced.member_);
}

TEST_F(component_Rotor, test_iterator_equals_position_after_upgrade_readonly) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto state = rotor.acquireReadOnly();
  auto it = state.upgrade(begin);
  ASSERT_EQ(begin, it);
}

TEST_F(component_Rotor, test_iterator_equals_position_after_upgrade_readwrite) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto state = rotor.acquireReadOnly();
  auto it = state.upgrade(begin);
  ASSERT_EQ(begin, it);
}

TEST_F(component_Rotor, test_2_iterators_both_upgraded_readonly_are_equal) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto state0 = rotor.acquireReadOnly();
  auto it0 = state0.upgrade(begin);
  auto state1 = rotor.acquireReadOnly();
  auto it1 = state1.upgrade(begin);
  ASSERT_EQ(it0, it1);
}

TEST_F(component_Rotor, test_2_iterators_both_upgraded_readwrite_are_equal) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto state0 = rotor.acquireReadWrite();
  auto it0 = state0.upgrade(begin);
  auto state1 = rotor.acquireReadWrite();
  auto it1 = state1.upgrade(begin);
  ASSERT_EQ(it0, it1);
}

TEST_F(component_Rotor, test_upgraded_readonly_equal_upgraded_readwrite) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto state0 = rotor.acquireReadOnly();
  auto ro = state0.upgrade(begin);
  auto state1 = rotor.acquireReadWrite();
  auto rw = state1.upgrade(begin);
  ASSERT_EQ(ro, rw);
}

TEST_F(component_Rotor, test_upgraded_ro_equal_pos_after_locking_unlocking_rw) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  {
    auto state0 = rotor.acquireReadWrite();
  }
  auto state1 = rotor.acquireReadOnly();
  auto ro = state1.upgrade(begin);
  ASSERT_EQ(begin, ro);
}

TEST_F(component_Rotor, test_upgraded_rw_equal_pos_after_locking_unlocking_rw) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  {
    auto state0 = rotor.acquireReadWrite();
  }
  auto state1 = rotor.acquireReadWrite();
  auto rw = state1.upgrade(begin);
  ASSERT_EQ(begin, rw);
}

TEST_F(component_Rotor, test_pos_it_begin_plus_one_equal_upgraded_ro_plus_one) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto pos = begin + 1;
  auto state0 = rotor.acquireReadOnly();
  auto ro = state0.upgrade(begin) + 1;
  ASSERT_EQ(pos, ro);
}

TEST_F(component_Rotor, test_pos_it_begin_plus_one_equal_upgraded_rw_plus_one) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto begin = rotor.begin();
  auto pos = begin + 1;
  auto state0 = rotor.acquireReadWrite();
  auto rw = state0.upgrade(begin) + 1;
  ASSERT_EQ(pos, rw);
}

TEST_F(component_Rotor, test_begin_upgraded_ro_plus_capacity_equals_pos_end) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .emplace<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  auto state0 = rotor.acquireReadOnly();
  auto ro = state0.upgrade(rotor.begin()) + rotor.capacity();
  ASSERT_EQ(rotor.end(), ro);
}


TEST_F(component_Rotor, test_write_point_different_that_read) {
  auto rotor = RotorBuilder<EmptyComponent>()
      .emplace<EmptyComponent>()
      .build();

  MockVisitor visitor;
  auto AssertWritePointNotEqualReadPoint = [] (EmptyComponent const& from,
                                               EmptyComponent &to) {
    ASSERT_NE(&from, &to);
  };
  EXPECT_CALL(visitor, call(Matcher<EmptyComponent const&>(_),
                            Matcher<EmptyComponent&>(_)))
      .WillOnce(Invoke(AssertWritePointNotEqualReadPoint));

  auto state0 = rotor.acquireReadWrite();
  auto it = state0.upgrade(rotor.begin());
  it.visit(wrap(visitor));
}

TEST_F(component_Rotor,
       test_source_component_contain_initial_value_when_visiting_after_build) {
  int number = 5;
  auto rotor = RotorBuilder<OneMemberComponent<int>>()
      .emplace<OneMemberComponent<int>>(number)
      .build();

  MockVisitor visitor;
  struct AssertMemberEquals {
    int expected;

    void operator() (OneMemberComponent<int> const& from,
                     OneMemberComponent<int> &) {
      ASSERT_EQ(expected, from.member_);
    }
  };
  EXPECT_CALL(visitor, call(Matcher<OneMemberComponent<int> const&>(_),
                            Matcher<OneMemberComponent<int> &>(_)))
      .WillOnce(Invoke(AssertMemberEquals{number}));

  auto state0 = rotor.acquireReadWrite();
  auto it = state0.upgrade(rotor.begin());
  it.visit(wrap(visitor));
}

TEST_F(component_Rotor,
       test_component_contain_value_set_during_previous_upgrade) {
  int initial = 0;
  int number = 5;
  auto rotor = RotorBuilder<OneMemberComponent<int>>()
      .emplace<OneMemberComponent<int>>(initial)
      .build();

  {
    auto state0 = rotor.acquireReadWrite();
    auto it = state0.upgrade(rotor.begin());
    it.visit([number] (OneMemberComponent<int> const&,
                       OneMemberComponent<int> &target) {
               target.member_ = number;
             });
  }

  MockVisitor visitor;
  struct AssertMemberEquals {
    int expected;

    void operator() (OneMemberComponent<int> const& from,
                     OneMemberComponent<int> &) {
      ASSERT_EQ(expected, from.member_);
    }
  };
  EXPECT_CALL(visitor, call(Matcher<OneMemberComponent<int> const&>(_),
                            Matcher<OneMemberComponent<int> &>(_)))
      .WillOnce(Invoke(AssertMemberEquals{number}));

  auto state1 = rotor.acquireReadWrite();
  auto it = state1.upgrade(rotor.begin());
  it.visit(wrap(visitor));
}

