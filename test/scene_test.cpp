// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/scene.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test {

struct EmptyComponent {
}; // struct EmptyComponent

struct CountingComponent : public EmptyComponent {
  static size_t constructor_calls;
  static size_t destructor_calls;
  static void resetCounters() {
    constructor_calls = destructor_calls = 0;
  }

  CountingComponent()
    : id_(constructor_calls++) {
  }
  ~CountingComponent() {
    destructor_calls += 1;
  }
  size_t id_;
}; // CountingEmptyComponent

size_t CountingComponent::constructor_calls = 0;
size_t CountingComponent::destructor_calls = 0;

struct MockVisitor {
  typedef std::tuple<EmptyComponent const&, CountingComponent const&> TupleType;

  MOCK_METHOD1(call, void(EmptyComponent const&));
  MOCK_METHOD1(call, void(CountingComponent const&));
  MOCK_METHOD1(call, void(TupleType const&));
}; // struct MockVisitor

template <class VisitorType>
class MockVisitorWrapper {
 public:
  MockVisitorWrapper(VisitorType &visitor) : delegate_(visitor) {
  }
  template <class Type>
  void operator() (Type &obj) {
    delegate_.call(obj);
  }
 private:
  VisitorType &delegate_;
}; // struct MockVisitorWrapper

template <class VisitorType>
MockVisitorWrapper<VisitorType> wrap(VisitorType &visitor) {
  return MockVisitorWrapper<VisitorType>(visitor);
}

} // namespace test

using namespace test;
using namespace testing;

class game_Scene : public ::testing::TestWithParam<size_t> {
  void SetUp() {
    CountingComponent::resetCounters();
  }
};

TEST_F(game_Scene, test_constructor_called_once) {
  gyros::Builder<CountingComponent> builder;
  builder.emplace<CountingComponent>();
  auto scene = builder.build();
  
  ASSERT_EQ(1, CountingComponent::constructor_calls);
}

TEST_F(game_Scene, test_destructor_called_once_after_destroying_scene) {
  {
    gyros::Builder<CountingComponent> builder;
    builder.emplace<CountingComponent>();
    auto scene = builder.build();
  }
  ASSERT_EQ(1, CountingComponent::destructor_calls);
}

TEST_F(game_Scene, test_destructor_not_called_when_scene_not_destroyed) {
  gyros::Builder<CountingComponent> builder;
  builder.emplace<CountingComponent>();
  auto scene = builder.build();

  ASSERT_EQ(0, CountingComponent::destructor_calls);
}

TEST_F(game_Scene, test_visitor_called_one_time_if_one_entity_created) {
  gyros::Builder<EmptyComponent> builder;
  builder.emplace<EmptyComponent>();
  auto scene = builder.build();

  MockVisitor mock_visitor;
  EXPECT_CALL(mock_visitor, call(An<EmptyComponent const&>()))
    .Times(1);
  
  scene.visitReadonly<EmptyComponent>(wrap(mock_visitor));
}

