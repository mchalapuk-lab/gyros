// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/builder.hpp"

#include "test/gyros/components.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Mock = test::gyros::component::MockComponent;

class gyros_Builder : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_F(gyros_Builder, test_building_scene_with_one_component) {
  gyros::Builder<TypeList<Simple>> builder;
  builder.newEntity()
      .emplace<Simple>();
  auto scene = builder.build();
}

