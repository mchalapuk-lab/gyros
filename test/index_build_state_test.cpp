// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/entity/detail/index_build_state.hpp"

#include "gyros/util/type_list.hpp"
#include "test/gyros/components.hpp"
#include "test/static_assert.hpp"
#include "test/gyros/component/mock_rotor.hpp"

template <class Type>
using Member = test::gyros::component::OneMemberComponent<Type>;
using Simple = test::gyros::component::EmptyComponent;
using Complex = test::gyros::component::MockComponent;

namespace tl = gyros::util::type_list;

template <class ...Types>
using IndexBuildState = gyros::entity::detail::IndexBuildState<Types...>;
template <class Type>
using Traits = gyros::TypeTraits<Type>;

namespace gyros {
// for simplicity of testing pointers will be used
// instead of component::PositionIterator
template<> struct TypeTraits<Simple> { typedef Simple* IteratorType; };
template<> struct TypeTraits<Complex> { typedef Complex* IteratorType; };
template<class T> struct TypeTraits<Member<T>> {
  typedef Member<T>* IteratorType;
};
} // namespace gyros

namespace {
// static tests

void test_type_list_type_in_traits_of_build_state_with_one_component() {
  typedef tl::TypeList<Simple> ExpectedType;
  typedef IndexBuildState<Simple> TestedType;
  typedef typename Traits<TestedType>::ComponentTypeList ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_type_list_type_in_traits_of_build_state_with_three_components() {
  typedef tl::TypeList<Simple, Complex, Simple> ExpectedType;
  typedef IndexBuildState<Simple, Complex, Simple> TestedType;
  typedef typename Traits<TestedType>::ComponentTypeList ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_type_in_traits_of_build_state_with_one_component() {
  typedef IndexBuildState<Simple> ExpectedType;
  typedef IndexBuildState<Simple> TestedType;
  typedef typename Traits<TestedType>::Type ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_type_in_traits_of_build_state_with_three_components() {
  typedef IndexBuildState<Simple, Complex, Simple> ExpectedType;
  typedef IndexBuildState<Simple, Complex, Simple> TestedType;
  typedef typename Traits<TestedType>::Type ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_head_component_type_in_traits_of_build_state_with_one_component() {
  typedef Simple ExpectedType;
  typedef IndexBuildState<Simple> TestedType;
  typedef typename Traits<TestedType>::HeadComponentType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_head_component_type_in_traits_of_build_state_with_three_components() {
  typedef Complex ExpectedType;
  typedef IndexBuildState<Complex, Simple, Simple> TestedType;
  typedef typename Traits<TestedType>::HeadComponentType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_iterator_type_in_traits_of_build_state_with_one_component() {
  typedef Simple* ExpectedType;
  typedef IndexBuildState<Simple> TestedType;
  typedef typename Traits<TestedType>::IteratorType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_iterator_type_in_traits_of_build_state_with_three_components() {
  typedef Complex* ExpectedType;
  typedef IndexBuildState<Complex, Simple, Simple> TestedType;
  typedef typename Traits<TestedType>::IteratorType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_super_type_in_traits_of_build_state_with_one_component() {
  typedef IndexBuildState<> ExpectedType;
  typedef IndexBuildState<Simple> TestedType;
  typedef typename Traits<TestedType>::SuperType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_super_type_in_traits_of_build_state_with_three_components() {
  typedef IndexBuildState<Simple, Simple> ExpectedType;
  typedef IndexBuildState<Complex, Simple, Simple> TestedType;
  typedef typename Traits<TestedType>::SuperType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

} // namespace

// dynamic test

template <class ...Types>
using MockRotor = test::gyros::component::MockRotor<Types...>;
template <class Type>
using TypeLiteral = gyros::util::TypeLiteral<Type>;
using namespace ::testing;

struct entity_IndexBuildState : public ::testing::TestWithParam<ptrdiff_t> {
  entity_IndexBuildState()
      : diff(1024),
      begin0(Traits<Simple>::IteratorType() + diff),
      begin1(Traits<Complex>::IteratorType() + diff * 2),
      begin2(Traits<Member<int>>::IteratorType() + diff * 3) {
  }
  ptrdiff_t diff = 1024;

  Traits<Simple>::IteratorType begin0;
  Traits<Complex>::IteratorType begin1;
  Traits<Member<int>>::IteratorType begin2;
};

TEST_F(entity_IndexBuildState, test_creating_state_with_one_component) {
  MockRotor<Simple> rotor;
  IndexBuildState<Simple> tested_state(rotor);
}

TEST_F(entity_IndexBuildState, test_creating_state_with_three_components) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
}

TEST_F(entity_IndexBuildState,
       test_iterator_value_after_constructing_state_with_one_component) {
  MockRotor<Simple> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple> tested_state(rotor);
  ASSERT_EQ(begin0, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_first_it_value_after_constructing_state_with_three_component) {

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(begin0, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_third_it_value_after_constructing_state_with_three_component) {

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Complex>>()))
      .WillOnce(Return(begin1));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(begin1, tested_state.it<Complex>());
}

TEST_F(entity_IndexBuildState,
       test_iterator_value_after_constructing_state_with_three_component) {

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Member<int>>>()))
      .WillOnce(Return(begin2));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(begin2, tested_state.it<Member<int>>());
}

TEST_F(entity_IndexBuildState,
       test_iterator_value_after_intcrementing_on_state_with_one_component) {
  auto expected = begin0 + diff;

  MockRotor<Simple> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple> tested_state(rotor);
  ASSERT_EQ(expected, tested_state.increment<Simple>(diff));
  ASSERT_EQ(expected, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_first_it_value_after_incrementing_on_state_with_three_component) {
  auto expected0 = begin0 + diff;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(expected0, tested_state.increment<Simple>(diff));
  ASSERT_EQ(expected0, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_second_itr_value_after_incrementing_on_state_with_three_component) {
  auto expected1 = begin1 + diff;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Complex>>()))
      .WillOnce(Return(begin1));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(expected1, tested_state.increment<Complex>(diff));
  ASSERT_EQ(expected1, tested_state.it<Complex>());
}

TEST_F(entity_IndexBuildState,
       test_third_it_value_after_incrementing_on_state_with_three_component) {
  auto expected2 = begin2 + diff;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Member<int>>>()))
      .WillOnce(Return(begin2));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(expected2, tested_state.increment<Member<int>>(diff));
  ASSERT_EQ(expected2, tested_state.it<Member<int>>());
}

TEST_F(entity_IndexBuildState,
       test_iterator_value_after_intcrementing_twice_on_state_with_one_comp) {
  auto expected = begin0 + diff;

  MockRotor<Simple> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple> tested_state(rotor);
  ASSERT_EQ(expected, tested_state.increment<Simple>(diff));
  ASSERT_EQ(expected, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_first_it_value_after_incrementing_twice_on_state_with_three_comp) {
  auto expected0 = begin0 + 2*diff;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Simple>(diff);
  ASSERT_EQ(expected0, tested_state.increment<Simple>(diff));
  ASSERT_EQ(expected0, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_second_it_value_after_incrementing_twice_on_state_with_three_comp) {
  auto expected1 = begin1 + 2*diff;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Complex>>()))
      .WillOnce(Return(begin1));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Complex>(diff);
  ASSERT_EQ(expected1, tested_state.increment<Complex>(diff));
  ASSERT_EQ(expected1, tested_state.it<Complex>());
}

TEST_F(entity_IndexBuildState,
       test_third_it_value_after_incrementing_twice_on_state_with_three_comp) {
  auto expected2 = begin2 + 2*diff;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Member<int>>>()))
      .WillOnce(Return(begin2));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Member<int>>(diff);
  ASSERT_EQ(expected2, tested_state.increment<Member<int>>(diff));
  ASSERT_EQ(expected2, tested_state.it<Member<int>>());
}

TEST_F(entity_IndexBuildState,
       test_first_it_unchanged_after_incr_second_on_state_with_three_comp) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Complex>(diff);
  ASSERT_EQ(begin0, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_first_it_unchanged_after_incr_third_on_state_with_three_comp) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(begin0));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Member<int>>(diff);
  ASSERT_EQ(begin0, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_second_it_unchanged_after_incr_first_on_state_with_three_comp) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Complex>>()))
      .WillOnce(Return(begin1));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Simple>(diff);
  ASSERT_EQ(begin1, tested_state.it<Complex>());
}

TEST_F(entity_IndexBuildState,
       test_second_it_unchanged_after_incr_third_on_state_with_three_comp) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Complex>>()))
      .WillOnce(Return(begin1));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Member<int>>(diff);
  ASSERT_EQ(begin1, tested_state.it<Complex>());
}

TEST_F(entity_IndexBuildState,
       test_third_it_unchanged_after_incr_first_on_state_with_three_comp) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Member<int>>>()))
      .WillOnce(Return(begin2));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Simple>(diff);
  ASSERT_EQ(begin2, tested_state.it<Member<int>>());
}

TEST_F(entity_IndexBuildState,
       test_third_it_unchanged_after_incr_second_on_state_with_three_comp) {
  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Member<int>>>()))
      .WillOnce(Return(begin2));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  tested_state.increment<Complex>(diff);
  ASSERT_EQ(begin2, tested_state.it<Member<int>>());
}

