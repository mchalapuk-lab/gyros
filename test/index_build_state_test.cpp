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

class entity_IndexBuildState : public ::testing::TestWithParam<ptrdiff_t> {
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
  Traits<Simple>::IteratorType expected;
  expected += 1024;

  MockRotor<Simple> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(expected));

  IndexBuildState<Simple> tested_state(rotor);
  ASSERT_EQ(expected, tested_state.it<Simple>());
}

TEST_F(entity_IndexBuildState,
       test_iterator_value_after_constructing_state_with_three_component) {
  Traits<Simple>::IteratorType expected0;
  expected0 += 1024;
  Traits<Complex>::IteratorType expected1;
  expected1 += 1024;
  Traits<Member<int>>::IteratorType expected2;
  expected2 += 1024;

  MockRotor<Simple, Complex, Member<int>> rotor;
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Simple>>()))
      .WillOnce(Return(expected0));
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Complex>>()))
      .WillOnce(Return(expected1));
  EXPECT_CALL(rotor, begin(A<TypeLiteral<Member<int>>>()))
      .WillOnce(Return(expected2));

  IndexBuildState<Simple, Complex, Member<int>> tested_state(rotor);
  ASSERT_EQ(expected0, tested_state.it<Simple>());
  ASSERT_EQ(expected1, tested_state.it<Complex>());
  ASSERT_EQ(expected2, tested_state.it<Member<int>>());
}

