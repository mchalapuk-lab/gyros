// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENTS_HPP_
#define TEST_GYROS_COMPONENTS_HPP_

#include <cstdlib>

namespace test {
namespace gyros {
namespace component {

struct EmptyComponent {
}; // struct EmptyComponent

struct CountingComponent {
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
}; // struct CountingComponent

template <class MemberType>
struct OneMemberComponent {
  OneMemberComponent() {
  }
  OneMemberComponent(MemberType member)
      : member_(member) {
  }
  MemberType member_;
}; // struct OneMemberComponent

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

