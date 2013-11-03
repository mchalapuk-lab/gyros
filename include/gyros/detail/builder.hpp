// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_DETAIL_BUILDER_HPP_
#define GYROS_DETAIL_BUILDER_HPP_

#include <type_traits>

#include "gyros/fwd/builder.hpp"

namespace gyros {
namespace detail {

const size_t N_COPIES = 3;

template <class T, class ...ParamTypes>
static T* emplace(void *placement, ParamTypes &&...params) noexcept {
  return new (placement) T(std::forward<ParamTypes>(params)...);
}

template <class T>
using RawMemory = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

template <class SceneType, class ...ComponentTypes>
struct SceneCreator {
  template <class ...ArgTypes>
  SceneType operator() (Builder<> const&, ArgTypes... args) const {
    return SceneType(args...);
  }
}; // struct SceneCreator

template <class SceneType, class T, class ...L>
struct SceneCreator<SceneType, T, L...> {
  typedef Builder<T, L...> BuilderType;

  template <class ...ArgTypes>
  SceneType operator() (BuilderType const& builder, ArgTypes ...args) const {
    size_t capacity = builder.factories_.size();
    RawMemory<T> *pool = new RawMemory<T>[capacity * N_COPIES];

    auto it = pool;
    for (auto factory : builder.factories_) {
      factory(it++);
    }
    std::copy(pool, it, it);
    it += capacity;
    std::copy(it, it, it + capacity);

    SceneCreator<SceneType, L...> create;
    return create(builder, args..., pool, capacity);
  }
}; // struct SceneCreator<SceneType, T, L...>

} // namespace detail
} // namespace gyros

#endif // include guard

