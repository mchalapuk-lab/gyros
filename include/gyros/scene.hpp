// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_SCENE_HPP_
#define GYROS_SCENE_HPP_

#include "gyros/builder.hpp"
#include "gyros/state.hpp"
#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"

namespace gyros {

template <class ...T>
struct Scene {
 public:
  Scene(Scene<T...> &&rhs) noexcept
    : scene_(std::move(rhs.scene_)) {
  }

  template <class ...ComponentTypes>
  State<ComponentTypes...> const acquireReadonly() noexcept {
    return State<ComponentTypes...>(*this);
  }

  template <class ...ComponentTypes, class Visitor>
  void visitReadonly(Visitor &&visit) {
    for (auto const& tuple : acquireReadonly<ComponentTypes...>()) {
      visit(tuple);
    }
  }

 protected:
  typedef component::Rotor<ComponentTypes...> RotorType;
  typedef entity::Index<ComponentTypes...> IndexType;

  Scene(RotorType const& rotor, IndexType const& index)
    : rotor_(rotor), index_(index) {
  }

 private:
  RotorType const& rotor_;
  IndexType const& index_;

  template <class ...ComponentTypes>
  friend class State;
}; // struct Scene<T...>

} // namespace gyros

#endif // include guard


searching for changes
