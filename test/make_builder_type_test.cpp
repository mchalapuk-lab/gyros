// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/make_builder_type.hpp"

#include "test/gyros/components.hpp"
#include "test/static_assert.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Mock = test::gyros::component::MockComponent;

void test_making_builder_type_with_one_component() {
  typedef gyros::component::Rotor<Simple> ExpectedRotorType;
  typedef gyros::entity::Index<TypeList<Simple>> ExpectedIndexType;
  typedef gyros::Builder<ExpectedRotorType, ExpectedIndexType> ExpectedBuilder;

  typedef gyros::MakeBuilder<TypeList<Simple>>::Type ActualBuilder;
  test::AssertIsSame<ExpectedBuilder, ActualBuilder>();
}

void test_making_builder_type_with_two_components() {
  typedef gyros::component::Rotor<Simple, Mock> ExpectedRotorType;
  typedef gyros::entity::Index<TypeList<Simple, Mock>> ExpectedIndexType;
  typedef gyros::Builder<ExpectedRotorType, ExpectedIndexType> ExpectedBuilder;

  typedef gyros::MakeBuilder<TypeList<Simple, Mock>>::Type ActualBuilder;
  test::AssertIsSame<ExpectedBuilder, ActualBuilder>();
}

void test_making_builder_type_with_two_tuples() {
  typedef gyros::component::Rotor<Simple, Mock>
      ExpectedRotorType;
  typedef gyros::entity::Index<TypeList<Simple>, TypeList<Mock>>
      ExpectedIndexType;
  typedef gyros::Builder<ExpectedRotorType, ExpectedIndexType>
      ExpectedBuilder;

  typedef gyros::MakeBuilder<TypeList<Simple>, TypeList<Mock>>::Type
      ActualBuilder;
  test::AssertIsSame<ExpectedBuilder, ActualBuilder>();
}

void test_making_builder_type_with_two_tuples_with_recurring_types() {
  typedef gyros::component::Rotor<Simple, Mock>
      ExpectedRotorType;
  typedef gyros::entity::Index<TypeList<Simple>, TypeList<Simple, Mock>>
      ExpectedIndexType;
  typedef gyros::Builder<ExpectedRotorType, ExpectedIndexType>
      ExpectedBuilder;

  typedef gyros::MakeBuilder<TypeList<Simple>, TypeList<Simple, Mock>>::Type
      ActualBuilder;
  test::AssertIsSame<ExpectedBuilder, ActualBuilder>();
}

