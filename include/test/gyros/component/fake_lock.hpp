// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENT_FAKE_LOCK_HPP_
#define TEST_GYROS_COMPONENT_FAKE_LOCK_HPP_

#include <memory>

namespace test {
namespace gyros {
namespace component {

struct FakeLock {
  template <class Deleter>
  FakeLock(Deleter deleter)
      : deleter_(deleter) {
  }
  ~FakeLock() {
    deleter_();
  }
 private:
  std::function<void ()> deleter_;
}; // struct FakeLock

struct FakeSharedLock {
  template <class Deleter>
  FakeSharedLock(Deleter deleter)
      : shared_(nullptr, [deleter] (std::nullptr_t) { deleter(); }) {
  }
  FakeSharedLock() {
  }
 private:
  std::shared_ptr<void> shared_;
}; // struct FakeSharedLock

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

