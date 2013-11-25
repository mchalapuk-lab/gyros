// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/scene.hpp"

#include "test/component.hpp"
#include "test/tuple/mock_visitor.hpp"
#include "test/visitor_wrapper.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace gyros;
using namespace test::gyros;
using namespace test::gyros::tuple;
using namespace testing;

class gyros_Scene : public ::testing::TestWithParam<size_t> {
  void SetUp() {
    CountingComponent::resetCounters();
  }
};

TEST_F(gyros_Scene, test_constructor_called_once) {
  SceneBuilder<CountingComponent> builder;
  builder.newEntity().emplace<CountingComponent>();
  auto scene = builder.build();

  ASSERT_EQ(1, CountingComponent::constructor_calls);
}

TEST_F(gyros_Scene, test_destructor_called_once_after_destroying_scene) {
  {
    SceneBuilder<CountingComponent> builder;
    builder.newEntity().emplace<CountingComponent>();
    auto scene = builder.build();
  }
  ASSERT_EQ(1, CountingComponent::destructor_calls);
}

TEST_F(gyros_Scene, test_destructor_not_called_when_scene_not_destroyed) {
  SceneBuilder<CountingComponent> builder;
  builder.newEntity().emplace<CountingComponent>();
  auto scene = builder.build();

  ASSERT_EQ(0, CountingComponent::destructor_calls);
}

TEST_F(gyros_Scene, test_visitor_called_one_time_if_one_entity_created) {
  SceneBuilder<EmptyComponent> builder;
  builder.newEntity().emplace<EmptyComponent>();
  auto scene = builder.build();

  MockVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, call(An<EmptyComponent const&>()))
    .Times(1);
  
  scene.visitReadonly<EmptyComponent>(wrap(mock_visitor));
}

