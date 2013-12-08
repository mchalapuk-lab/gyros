// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENT_MOCK_VISITOR_HPP_
#define TEST_GYROS_COMPONENT_MOCK_VISITOR_HPP_

#include "test/gyros/components.hpp"

#include <gmock/gmock.h>

namespace test {
namespace gyros {
namespace component {

struct MockVisitor {
  MOCK_METHOD2(call, void(EmptyComponent const&, EmptyComponent&));
  MOCK_METHOD2(call, void(CountingComponent const&, CountingComponent&));
  MOCK_METHOD2(call, void(MockComponent const&, MockComponent&));
  MOCK_METHOD2(call, void(OneMemberComponent<int> const&,
                          OneMemberComponent<int> &));

  template <class ComponentType>
  void operator() (ComponentType const& source, ComponentType &target) {
    call(source, target);
  }
}; // struct MockVisitor

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

