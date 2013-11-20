// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_POOL_HPP_
#define GYROS_COMPONENT_POOL_HPP_

#include <cstddef>

#include "gyros/component/rotor_builder.hpp"
#include "gyros/component/rotor_state.hpp"

namespace gyros {
namespace component {

template <class ...ComponentTypes>
struct Rotor {
}; // struct Rotor

template <class T, class ...L>
class Rotor<T, L...> : private Rotor<L...> {
 public:
  Rotor(Rotor<T, L...> &&rhs) noexcept
    : Rotor<L...>(std::move(rhs)),
    pool_(rhs.pool_),
    capacity_(rhs.capacity_) {
  }
  ~Rotor() {
    if (!capacity_) {
      return; // in case if moved
    }
    for (auto it = pool_, end = it + capacity_; it != end; ++it) {
      reinterpret_cast<T *>(it)->~T();
    }
    delete [] pool_;
  }

  template <class ComponentType>
  RotorState<ComponentType> const acquireReadonly() noexcept {
    return RotorState<ComponentType>(*this);
  }

  template <class ComponentType, class Visitor>
  void visitReadonly(Visitor &&visit) {
    for (auto const& tuple : acquireReadonly<ComponentType>()) {
      visit(tuple);
    }
  }

 protected:
  template <class ...ArgTypes>
  Rotor(detail::RawMemory<T> *pool_start,
        size_t pool_capacity,
        ArgTypes &&...next_pool_info)
    : Rotor<L...>(next_pool_info...),
    pool_(pool_start),
    capacity_(pool_capacity) {
  }

 private:
  detail::RawMemory<T> *pool_;
  size_t capacity_;

  friend class RotorState<T>;
  friend class detail::RotorCreator<Rotor<T, L...>>;
}; // class Rotor<T, L...>

} // namespace component
} // namespace gyros

#endif // include guard

