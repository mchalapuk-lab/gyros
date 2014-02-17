// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/builder.hpp"

#include "test/gyros/components.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Complex = test::gyros::component::OneMemberComponent<int>;

class gyros_Builder : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_F(gyros_Builder, test_building_scene_with_one_component) {
  gyros::Builder<TypeList<Simple>> builder;
  builder.newEntity()
      .emplace<Simple>();
  auto scene = builder.build();
}

TEST_F(gyros_Builder,
       test_building_scene_with_two_entities_of_same_type_with_one_component) {
  gyros::Builder<TypeList<Simple>> builder;
  builder.newEntity()
      .emplace<Simple>();
  builder.newEntity()
      .emplace<Simple>();
  auto scene = builder.build();
}

TEST_F(gyros_Builder,
       test_building_scene_with_two_entities_of_different_types) {
  gyros::Builder<TypeList<Simple>, TypeList<Complex>> builder;
  builder.newEntity()
      .emplace<Simple>();
  builder.newEntity()
      .emplace<Complex>();
  auto scene = builder.build();
}

