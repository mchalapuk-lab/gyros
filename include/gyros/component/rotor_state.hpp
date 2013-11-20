// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_STATE_HPP_
#define GYROS_COMPONENT_ROTOR_STATE_HPP_

#include <tuple>

#include "gyros/fwd/component/rotor_state.hpp"

namespace gyros {
namespace component {

template <class T>
class RotorState {
 public:
  template <class ...L>
  RotorState(Rotor<T, L...> &rotor) :
      pool_(rotor.pool_),
      capacity_(rotor.capacity_) {
  }

  T* begin() {
    return reinterpret_cast<T *>(pool_);
  }
  T* end() {
    return begin() + capacity_;
  }
  T const* begin() const {
    return reinterpret_cast<T const*>(pool_);
  }
  T const* end() const {
    return begin() + capacity_;
  }
 private:
  detail::RawMemory<T> *pool_;
  size_t capacity_;
}; // class RotorState<T>

} // namespace component
} // namespace gyros

#endif // include guard

