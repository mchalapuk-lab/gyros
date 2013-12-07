// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_LOCK_HPP_
#define GYROS_COMPONENT_ROTOR_LOCK_HPP_

#include <functional>

namespace gyros {
namespace component {

class RotorLock {
 public:
  template <class Deleter>
  RotorLock(Deleter deleter)
      : deleter_(deleter) {
  }
  RotorLock(RotorLock const&) = delete;
  RotorLock(RotorLock &&rhs)
      : deleter_(rhs.deleter_) {
    rhs.deleter_ = [] () {};
  }
  ~RotorLock() {
    deleter_();
  }
 private:
  std::function<void ()> deleter_;
}; // class RotorLock

} // namespace component
} // namespace gyros

#endif // include guard

