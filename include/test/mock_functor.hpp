// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_TEST_MOCK_FUNCTOR_HPP_
#define GYROS_TEST_MOCK_FUNCTOR_HPP_

#include <functional>

#include <gmock/gmock.h>

namespace test {

struct MockFunctor {
  MOCK_CONST_METHOD0(call, void());
  void operator() () const {
    call();
  }
}; // struct MockFunctor

template <class Functor>
std::function<void()> wrap(Functor & func) {
  return [&func] () { func(); };
}

} // namespace test

#endif // include guard

