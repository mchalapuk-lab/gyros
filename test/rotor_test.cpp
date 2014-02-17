// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/rotor.hpp"

#include "gyros/util/type_list.hpp"
#include "test/gyros/components.hpp"
#include "test/gyros/component/mock_visitor.hpp"
#include "test/gyros/visitor_wrapper.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using Simple = test::gyros::component::EmptyComponent;
using Counting = test::gyros::component::CountingComponent;
using Member = test::gyros::component::OneMemberComponent<int>;
using Mock = test::gyros::component::MockComponent;

using MockVisitor = test::gyros::component::MockVisitor;

namespace tl = gyros::util::type_list;
using namespace test::gyros;

template <class ...ComponentTypes>
struct TestTraits {
  typedef gyros::component::RotorBuilder<ComponentTypes...> RotorBuilderType;
  typedef gyros::component::Rotor<ComponentTypes...> RotorType;
}; // struct TestTraits

template <class TestedComponentType>
struct MakeTestingTypes {
  typedef tl::TypeList<Simple, Counting, Member, Mock> AllComponentTypes;
  typedef typename tl::Remove<AllComponentTypes, TestedComponentType>::Type
      NotTestedComponentTypes;
  typedef typename tl::Get<NotTestedComponentTypes, 0>::Type
      NotTestedComponent0;
  typedef typename tl::Get<NotTestedComponentTypes, 1>::Type
      NotTestedComponent1;
  typedef typename tl::Get<NotTestedComponentTypes, 2>::Type
      NotTestedComponent2;
  typedef testing::Types<
    TestTraits<TestedComponentType>,
    TestTraits<TestedComponentType,NotTestedComponent0>,
    TestTraits<TestedComponentType, NotTestedComponent1>,
    TestTraits<TestedComponentType, NotTestedComponent2>,
    TestTraits<NotTestedComponent1, TestedComponentType, NotTestedComponent0>,
    TestTraits<NotTestedComponent2, TestedComponentType, NotTestedComponent1>,
    TestTraits<NotTestedComponent1,
               NotTestedComponent0,
               NotTestedComponent2,
               TestedComponentType>
    > Type;
}; // MakeTestingTypes

using namespace testing;

template <class TypeParam>
class component_Rotor_Simple : public ::testing::Test {
};

TYPED_TEST_CASE_P(component_Rotor_Simple);

TYPED_TEST_P(component_Rotor_Simple, test_capacity) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  ASSERT_EQ(1, rotor.template capacity<Simple>());
}

TYPED_TEST_P(component_Rotor_Simple,
             test_begin_end_pos_iter_diff_equal_capacity) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  ptrdiff_t diff = rotor.template end<Simple>()-rotor.template begin<Simple>();
  ASSERT_EQ(rotor.template capacity<Simple>(), diff);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_iterator_equals_position_after_upgrade_readonly) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto state = rotor.acquireReadOnly();
  auto it = state.upgrade(begin);
  ASSERT_EQ(begin, it);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_iterator_equals_position_after_upgrade_readwrite) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto state = rotor.acquireReadOnly();
  auto it = state.upgrade(begin);
  ASSERT_EQ(begin, it);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_2_iterators_both_upgraded_readonly_are_equal) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto state0 = rotor.acquireReadOnly();
  auto it0 = state0.upgrade(begin);
  auto state1 = rotor.acquireReadOnly();
  auto it1 = state1.upgrade(begin);
  ASSERT_EQ(it0, it1);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_2_iterators_both_upgraded_readwrite_are_equal) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto state0 = rotor.acquireReadWrite();
  auto it0 = state0.upgrade(begin);
  auto state1 = rotor.acquireReadWrite();
  auto it1 = state1.upgrade(begin);
  ASSERT_EQ(it0, it1);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_upgraded_readonly_equal_upgraded_readwrite) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto state0 = rotor.acquireReadOnly();
  auto ro = state0.upgrade(begin);
  auto state1 = rotor.acquireReadWrite();
  auto rw = state1.upgrade(begin);
  ASSERT_EQ(ro, rw);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_upgraded_ro_equal_pos_after_locking_unlocking_rw) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  {
    auto state0 = rotor.acquireReadWrite();
  }
  auto state1 = rotor.acquireReadOnly();
  auto ro = state1.upgrade(begin);
  ASSERT_EQ(begin, ro);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_upgraded_rw_equal_pos_after_locking_unlocking_rw) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  {
    auto state0 = rotor.acquireReadWrite();
  }
  auto state1 = rotor.acquireReadWrite();
  auto rw = state1.upgrade(begin);
  ASSERT_EQ(begin, rw);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_pos_it_begin_plus_one_equal_upgraded_ro_plus_one) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto pos = begin + 1;
  auto state0 = rotor.acquireReadOnly();
  auto ro = state0.upgrade(begin) + 1;
  ASSERT_EQ(pos, ro);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_pos_it_begin_plus_one_equal_upgraded_rw_plus_one) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .template emplace<Simple>()
      .build();

  auto begin = rotor.template begin<Simple>();
  auto pos = begin + 1;
  auto state0 = rotor.acquireReadWrite();
  auto rw = state0.upgrade(begin) + 1;
  ASSERT_EQ(pos, rw);
}

TYPED_TEST_P(component_Rotor_Simple,
             test_begin_upgraded_ro_plus_capacity_equals_pos_end) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .template emplace<Simple>()
      .template emplace<Simple>()
      .build();

  auto state0 = rotor.acquireReadOnly();
  auto ro = state0.upgrade(rotor.template begin<Simple>())
      + rotor.template capacity<Simple>();
  ASSERT_EQ(rotor.template end<Simple>(), ro);
}


TYPED_TEST_P(component_Rotor_Simple,
             test_write_point_different_that_read) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Simple>()
      .build();

  MockVisitor visitor;
  auto AssertWritePointNotEqualReadPoint = [] (Simple const& from, Simple &to) {
    ASSERT_NE(&from, &to);
  };
  EXPECT_CALL(visitor, call(Matcher<Simple const&>(_), Matcher<Simple&>(_)))
      .WillOnce(Invoke(AssertWritePointNotEqualReadPoint));

  auto state0 = rotor.acquireReadWrite();
  auto it = state0.upgrade(rotor.template begin<Simple>());
  it.visit(wrap(visitor));
}

REGISTER_TYPED_TEST_CASE_P(
    component_Rotor_Simple,
    test_capacity,
    test_begin_end_pos_iter_diff_equal_capacity,
    test_iterator_equals_position_after_upgrade_readonly,
    test_iterator_equals_position_after_upgrade_readwrite,
    test_2_iterators_both_upgraded_readonly_are_equal,
    test_2_iterators_both_upgraded_readwrite_are_equal,
    test_upgraded_readonly_equal_upgraded_readwrite,
    test_upgraded_ro_equal_pos_after_locking_unlocking_rw,
    test_upgraded_rw_equal_pos_after_locking_unlocking_rw,
    test_pos_it_begin_plus_one_equal_upgraded_ro_plus_one,
    test_pos_it_begin_plus_one_equal_upgraded_rw_plus_one,
    test_begin_upgraded_ro_plus_capacity_equals_pos_end,
    test_write_point_different_that_read
    );

INSTANTIATE_TYPED_TEST_CASE_P(
    component_Rotor_SimpleComponentTests,
    component_Rotor_Simple,
    typename MakeTestingTypes<Simple>::Type
    );

template <class TypeParam>
class component_Rotor_Counting : public ::testing::Test {
  void SetUp() {
    Counting::resetCounters();
  }
};

TYPED_TEST_CASE_P(component_Rotor_Counting);

TYPED_TEST_P(component_Rotor_Counting,
       test_constructor_called_once) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Counting>()
      .build();

  ASSERT_EQ(1, Counting::constructor_calls);
}

TYPED_TEST_P(component_Rotor_Counting,
       test_destructor_called_once_after_destroying_rotor) {
  {
    auto rotor = typename TypeParam::RotorBuilderType()
        .template emplace<Counting>()
        .build();
  }
  ASSERT_EQ(1, Counting::destructor_calls);
}

TYPED_TEST_P(component_Rotor_Counting,
       test_destructor_not_called_when_rotor_not_destroyed) {
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Counting>()
      .build();

  ASSERT_EQ(0, Counting::destructor_calls);
}

REGISTER_TYPED_TEST_CASE_P(
    component_Rotor_Counting,
    test_constructor_called_once,
    test_destructor_called_once_after_destroying_rotor,
    test_destructor_not_called_when_rotor_not_destroyed
    );

INSTANTIATE_TYPED_TEST_CASE_P(
    component_Rotor_CountingComponentTests,
    component_Rotor_Counting,
    typename MakeTestingTypes<Counting>::Type
    );

template <class TypeParam>
class component_Rotor_Member : public ::testing::Test {
};

TYPED_TEST_CASE_P(component_Rotor_Member);

TYPED_TEST_P(component_Rotor_Member,
             test_dereferencing_after_upgrade_readonly) {
  int value = 2;

  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Member>(value)
      .build();

  auto state = rotor.acquireReadOnly();
  auto it = state.upgrade(rotor.template begin<Member>());
  auto const& dereferenced = *it;
  ASSERT_EQ(value, dereferenced.member_);
}

TYPED_TEST_P(
    component_Rotor_Member,
    test_source_component_contain_initial_value_when_visiting_after_build
    ) {
  int number = 5;
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Member>(number)
      .build();

  MockVisitor visitor;
  struct AssertMemberEquals {
    int expected;

    void operator() (Member const& from, Member &) {
      ASSERT_EQ(expected, from.member_);
    }
  };
  EXPECT_CALL(visitor, call(Matcher<Member const&>(_), Matcher<Member &>(_)))
      .WillOnce(Invoke(AssertMemberEquals{number}));

  auto state0 = rotor.acquireReadWrite();
  auto it = state0.upgrade(rotor.template begin<Member>());
  it.visit(wrap(visitor));
}

TYPED_TEST_P(component_Rotor_Member,
             test_component_contain_value_set_during_previous_upgrade) {
  int initial = 0;
  int number = 5;
  auto rotor = typename TypeParam::RotorBuilderType()
      .template emplace<Member>(initial)
      .build();

  {
    auto state0 = rotor.acquireReadWrite();
    auto it = state0.upgrade(rotor.template begin<Member>());
    it.visit(
        [number] (Member const&, Member &target) { target.member_ = number; }
        );
  }

  MockVisitor visitor;
  struct AssertMemberEquals {
    int expected;

    void operator() (Member const& from, Member &) {
      ASSERT_EQ(expected, from.member_);
    }
  };
  EXPECT_CALL(visitor, call(Matcher<Member const&>(_), Matcher<Member &>(_)))
      .WillOnce(Invoke(AssertMemberEquals{number}));

  auto state1 = rotor.acquireReadWrite();
  auto it = state1.upgrade(rotor.template begin<Member>());
  it.visit(wrap(visitor));
}

REGISTER_TYPED_TEST_CASE_P(
    component_Rotor_Member,
    test_dereferencing_after_upgrade_readonly,
    test_source_component_contain_initial_value_when_visiting_after_build,
    test_component_contain_value_set_during_previous_upgrade
    );

INSTANTIATE_TYPED_TEST_CASE_P(
    component_Rotor_MemberComponentTests,
    component_Rotor_Member,
    typename MakeTestingTypes<Member>::Type
    );

