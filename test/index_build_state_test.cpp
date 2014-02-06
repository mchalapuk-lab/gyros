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
using Mock = test::gyros::component::MockComponent;

namespace tl = gyros::util::type_list;

template <class ...Types>
using IndexBuildState = gyros::entity::detail::IndexBuildState<Types...>;
template <class Type>
using Traits = gyros::TypeTraits<Type>;

namespace gyros {
// for simplicity of testing pointers will be used
// instead of component::PositionIterator
template<> struct TypeTraits<Simple> { typedef Simple* IteratorType; };
template<> struct TypeTraits<Mock> { typedef Mock* IteratorType; };
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
  typedef tl::TypeList<Simple, Mock, Simple> ExpectedType;
  typedef IndexBuildState<Simple, Mock, Simple> TestedType;
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
  typedef IndexBuildState<Simple, Mock, Simple> ExpectedType;
  typedef IndexBuildState<Simple, Mock, Simple> TestedType;
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
  typedef Mock ExpectedType;
  typedef IndexBuildState<Mock, Simple, Simple> TestedType;
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
  typedef Mock* ExpectedType;
  typedef IndexBuildState<Mock, Simple, Simple> TestedType;
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
  typedef IndexBuildState<Mock, Simple, Simple> TestedType;
  typedef typename Traits<TestedType>::SuperType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

} // namespace

// dynamic test

template <class ...Types>
using MockRotor = test::gyros::component::MockRotor<Types...>;

class entity_IndexBuildState : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_F(entity_IndexBuildState, test_building_state_with_one_component) {
  MockRotor<Simple> rotor;
  IndexBuildState<Simple> tested_state(rotor);
}

TEST_F(entity_IndexBuildState, test_building_state_with_three_components) {
  MockRotor<Simple, Mock, Member<int>> rotor;
  IndexBuildState<Simple, Mock, Member<int>> tested_state(rotor);
}


