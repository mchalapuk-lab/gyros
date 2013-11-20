// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_DETAIL_ROTOR_BUILDER_HPP_
#define GYROS_COMPONENT_DETAIL_ROTOR_BUILDER_HPP_

#include <type_traits>

#include "gyros/fwd/component/rotor_builder.hpp"

namespace gyros {
namespace component {
namespace detail {

const size_t N_COPIES = 3;

template <class T, class ...ParamTypes>
static T* emplace(void *placement, ParamTypes &&...params) noexcept {
  return new (placement) T(std::forward<ParamTypes>(params)...);
}

template <class T>
using RawMemory = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

template <class RotorType, class ...ComponentTypes>
struct RotorCreator {
  template <class ...ArgTypes>
  RotorType operator() (RotorBuilder<> const&, ArgTypes... args) const {
    return RotorType(args...);
  }
}; // struct RotorCreator

template <class RotorType, class T, class ...L>
struct RotorCreator<RotorType, T, L...> {
  typedef RotorBuilder<T, L...> BuilderType;

  template <class ...ArgTypes>
  RotorType operator() (BuilderType const& builder, ArgTypes ...args) const {
    size_t capacity = builder.factories_.size();
    RawMemory<T> *pool = new RawMemory<T>[capacity * N_COPIES];

    auto it = pool;
    for (auto factory : builder.factories_) {
      factory(it++);
    }
    std::copy(pool, it, it);
    it += capacity;
    std::copy(it, it, it + capacity);

    RotorCreator<RotorType, L...> create;
    return create(builder, args..., pool, capacity);
  }
}; // struct RotorCreator<RotorType, T, L...>

} // namespace detail
} // namespace component
} // namespace gyros

#endif // include guard

