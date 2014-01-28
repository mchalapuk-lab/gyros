// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_SCENE_HPP_
#define GYROS_SCENE_HPP_

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"

#include "gyros/detail/make_rotor_type.hpp"

namespace gyros {

template <class ...EntityTypes>
class Scene {
 public:
  typedef typename detail::MakeRotor<EntityTypes...>::Type RotorType;
  typedef entity::Index<EntityTypes...> IndexType;

  Scene(Scene &&rhs) noexcept
    : rotor_(std::move(rhs.rotor_)),
      index_(std::move(rhs.index_)) {
  }

 private:
  RotorType rotor_;
  IndexType index_;

  Scene(RotorType && rotor, IndexType && index)
    : rotor_(std::move(rotor)), index_(std::move(index)) {
  }
}; // class Scene<EntityTypes...>

} // namespace gyros

#endif // include guard

