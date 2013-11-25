// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_TEST_COMPONENT_MOCK_VISITOR_HPP_
#define GYROS_TEST_COMPONENT_MOCK_VISITOR_HPP_

#include "test/gyros/components.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test {
namespace gyros {
namespace component {

struct MockVisitor {
  MOCK_METHOD1(call, void(EmptyComponent const&));
  MOCK_METHOD1(call, void(CountingComponent const&));
}; // struct MockVisitor

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

