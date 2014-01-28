// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/scene.hpp"

#include "test/gyros/components.hpp"
#include "test/static_assert.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Mock = test::gyros::component::MockComponent;

// static tests

static
void test_rotor_type_in_builder_type_with_one_component() {
  typedef gyros::component::Rotor<Simple> ExpectedRotorType;
  typedef typename gyros::Scene<TypeList<Simple>>::RotorType ActualRotorType;
  test::AssertIsSame<ExpectedRotorType, ActualRotorType>();
}

static
void test_rotor_type_in_builder_with_two_components() {
  typedef gyros::component::Rotor<Simple, Mock>
      ExpectedRotorType;
  typedef typename gyros::Scene<TypeList<Simple, Mock>>::RotorType
      ActualRotorType;
  test::AssertIsSame<ExpectedRotorType, ActualRotorType>();
}

static
void test_rotor_type_in_builder_with_two_tuples() {
  typedef gyros::component::Rotor<Simple, Mock>
      ExpectedRotorType;
  typedef typename gyros::Scene<TypeList<Simple>, TypeList<Mock>>::RotorType
      ActualRotorType;
  test::AssertIsSame<ExpectedRotorType, ActualRotorType>();
}

static
void test_rotor_type_in_builder_type_with_two_tuples_with_recurring_types() {
  typedef gyros::component::Rotor<Simple, Mock>
      ExpectedRotorType;
  typedef typename gyros::Scene<
      TypeList<Simple>,
      TypeList<Simple, Mock>
      >::RotorType ActualRotorType;
  test::AssertIsSame<ExpectedRotorType, ActualRotorType>();
}

// dynamic tests

