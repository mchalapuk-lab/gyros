// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/component/rotor.hpp"

#include "test/gyros/components.hpp"
#include "test/gyros/component/mock_visitor.hpp"
#include "test/gyros/visitor_wrapper.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace gyros::component;
using namespace test::gyros;
using namespace test::gyros::component;
using namespace testing;

class component_Rotor : public ::testing::TestWithParam<size_t> {
  void SetUp() {
    CountingComponent::resetCounters();
  }
};

TEST_F(component_Rotor, test_constructor_called_once) {
  RotorBuilder<CountingComponent> builder;
  builder.emplace<CountingComponent>();
  auto rotor = builder.build();

  ASSERT_EQ(1, CountingComponent::constructor_calls);
}

TEST_F(component_Rotor, test_destructor_called_once_after_destroying_rotor) {
  {
    RotorBuilder<CountingComponent> builder;
    builder.emplace<CountingComponent>();
    auto rotor = builder.build();
  }
  ASSERT_EQ(1, CountingComponent::destructor_calls);
}

TEST_F(component_Rotor, test_destructor_not_called_when_rotor_not_destroyed) {
  RotorBuilder<CountingComponent> builder;
  builder.emplace<CountingComponent>();
  auto rotor = builder.build();

  ASSERT_EQ(0, CountingComponent::destructor_calls);
}

TEST_F(component_Rotor, test_position_iterator_difference) {
  RotorBuilder<EmptyComponent> builder;
  builder.emplace<EmptyComponent>();
  auto rotor = builder.build();

  ptrdiff_t diff = rotor.end() - rotor.begin();
  ASSERT_EQ(1L, diff);
}

