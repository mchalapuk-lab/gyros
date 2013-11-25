// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_TUPLE_MOCK_VISITOR_HPP_
#define TEST_GYROS_TUPLE_MOCK_VISITOR_HPP_

#include "test/components.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test {
namespace gyros {
namespace tuple {

struct MockVisitor {
  typedef std::tuple<EmptyComponent const&, CountingComponent const&>
      EmptyCountingTuple;

  MOCK_METHOD1(call, void(EmptyCountingTuple const&));
}; // struct MockVisitor

} // namespace tuple
} // namespace gyros
} // namespace test

#endif // include guard

