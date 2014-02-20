// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/builder.hpp"

#include "test/gyros/components.hpp"

using Simple = test::gyros::component::EmptyComponent;
using Counting = test::gyros::component::CountingComponent;
using Complex = test::gyros::component::OneMemberComponent<int>;
using Mock = test::gyros::component::MockComponent;

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;

namespace entity = gyros::entity;

class gyros_Builder : public ::testing::Test {
};

TEST_F(gyros_Builder, test_building_with_one_1c_entity) {
  auto scene = gyros::Builder<TypeList<Simple>>()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>())
      .build();
}

TEST_F(gyros_Builder, test_building_with_two_1c_entities_of_same_type) {
  auto scene = gyros::Builder<TypeList<Simple>>()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>())
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>())
      .build();
}

TEST_F(gyros_Builder, test_building_with_one_2c_entity) {
  auto scene = gyros::Builder<TypeList<Simple, Complex>>()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Complex>())
      .build();
}

TEST_F(gyros_Builder, test_building_with_two_2c_entities_of_the_same_type) {
  auto scene = gyros::Builder<TypeList<Simple, Complex>>()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Complex>())
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Complex>())
      .build();
}

TEST_F(gyros_Builder, test_building_with_two_1c_entities_of_different_types) {
  auto scene = gyros::Builder<TypeList<Simple>, TypeList<Complex>>()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>())
      .addEntity(
          entity::Builder<>()
          .emplace<Complex>())
      .build();
}

TEST_F(gyros_Builder, test_building_with_two_2c_ent_with_disjoint_c_types) {
  typedef gyros::Builder<TypeList<Simple, Counting>, TypeList<Complex, Mock>>
      TestedBuilder;
  auto scene = TestedBuilder()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Counting>())
      .addEntity(
          entity::Builder<>()
          .emplace<Complex>()
          .emplace<Mock>())
      .build();
}

TEST_F(gyros_Builder,
       test_building_with_two_2c_ent_with_1_c_type_intersect_same_pos) {
  typedef gyros::Builder<TypeList<Simple, Complex>, TypeList<Mock, Complex>>
      TestedBuilder;
  auto scene = TestedBuilder()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Complex>())
      .addEntity(
          entity::Builder<>()
          .emplace<Mock>()
          .emplace<Complex>())
      .build();
}

TEST_F(gyros_Builder,
       test_building_with_two_2c_ent_with_1_c_type_intersect_different_pos) {
  typedef gyros::Builder<TypeList<Simple, Complex>, TypeList<Complex, Mock>>
      TestedBuilder;
  auto scene = TestedBuilder()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Complex>())
      .addEntity(
          entity::Builder<>()
          .emplace<Complex>()
          .emplace<Mock>())
      .build();
}

TEST_F(
    gyros_Builder,
    test_building_with_two_2c_e_of_different_types_with_all_c_types_intersect
    ) {
  typedef gyros::Builder<TypeList<Simple, Complex>, TypeList<Complex, Simple>>
     TestedType;
  auto scene = TestedType()
      .addEntity(
          entity::Builder<>()
          .emplace<Simple>()
          .emplace<Complex>())
      .addEntity(
          entity::Builder<>()
          .emplace<Complex>()
          .emplace<Simple>())
      .build();
}

