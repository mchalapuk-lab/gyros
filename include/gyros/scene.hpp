// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_SCENE_HPP_
#define GYROS_SCENE_HPP_

#include <cstddef>

#include "gyros/builder.hpp"
#include "gyros/state.hpp"

namespace gyros {

template <class ...EntityTypes>
struct Scene {
}; // struct Scene

template <class T, class ...L>
class Scene<T, L...> : private Scene<L...> {
 public:
  Scene(Scene<T, L...> &&rhs) noexcept
    : Scene<L...>(std::move(rhs)),
    pool_(rhs.pool_),
    capacity_(rhs.capacity_) {
  }
  ~Scene() {
    if (!capacity_) {
      return; // in case if moved
    }
    for (auto it = pool_, end = it + capacity_; it != end; ++it) {
      reinterpret_cast<T *>(it)->~T();
    }
    delete [] pool_;
  }

  template <class ComponentType>
  State<ComponentType> const acquireReadonly() noexcept {
    return State<ComponentType>(*this);
  }

  template <class ComponentType, class Visitor>
  void visitReadonly(Visitor &&visit) {
    for (auto const& tuple : acquireReadonly<ComponentType>()) {
      visit(tuple);
    }
  }

 protected:
  template <class ...ArgTypes>
  Scene(detail::RawMemory<T> *pool_start,
        size_t pool_capacity,
        ArgTypes &&...next_pool_info)
    : Scene<L...>(next_pool_info...),
    pool_(pool_start),
    capacity_(pool_capacity) {
  }

 private:
  detail::RawMemory<T> *pool_;
  size_t capacity_;

  friend class State<T>;
  friend class detail::SceneCreator<Scene<T, L...>>;
}; // class Scene<T, L...>

} // namespace gyros

#endif // include guard

