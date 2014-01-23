// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/builder.hpp"

#include "test/gyros/components.hpp"
#include "test/static_assert.hpp"

#include <gtest/gtest.h>

using namespace gyros;
using namespace gyros::component;
using namespace gyros::entity;
using namespace gyros::util::type_list;
using namespace test::gyros::component;
using namespace test;
using namespace testing;

void test_entity_index_type_after_adding_one_index() {
  typedef Rotor<EmptyComponent> ExpectedRotorType;
  typedef Index<TypeList<EmptyComponent>> ExpectedIndexType;
  typedef Builder<ExpectedRotorType, ExpectedIndexType> ExpectedBuilder;

  SceneBuilder builder;
  typedef decltype(builder.withEntityIndex<EmptyComponent>()) ActualBuilder;
  AssertIsSame<ExpectedBuilder, ActualBuilder>();
}

