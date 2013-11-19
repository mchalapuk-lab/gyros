// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_STATE_HPP_
#define GYROS_STATE_HPP_

#include <tuple>

#include "gyros/fwd/state.hpp"

namespace gyros {

template <class T>
class State {
 public:
  template <class ...L>
  State(Scene<T, L...> &scene) :
      pool_(scene.pool_),
      capacity_(scene.capacity_) {
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
}; // class State<T>

} // namespace gyros

#endif // include guard

