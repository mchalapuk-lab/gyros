// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_SCENE_HPP_
#define GYROS_SCENE_HPP_

#include "gyros/builder.hpp"
#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"

namespace gyros {

template <class ComponentRotorType, class EntityIndexType>
struct Scene {
}; // Scene<ComponentRotorType, EntityIndexType>

template <class ...ComponentTypes, class ...IndexedTupleTypes>
struct Scene<
    component::Rotor<ComponentTypes...>,
    entity::Index<IndexedTupleTypes...>
    > {
 public:
  typedef component::Rotor<ComponentTypes...> RotorType;
  typedef entity::Index<IndexedTupleTypes...> IndexType;

  Scene(Scene<RotorType, IndexType> && rhs) noexcept
    : rotor_(std::move(rhs.rotor_)),
      index_(std::move(rhs.index_)) {
  }

 private:
  RotorType rotor_;
  IndexType index_;

  Scene(RotorType && rotor, IndexType && index)
    : rotor_(std::move(rotor)), index_(std::move(index)) {
  }
}; // struct Scene<Rotor<ComponentTypes...>, Index<IndexedTupleTypes...>>

template <class ComponentRotorType, class EntityIndexType>
struct SceneBuilder {
  Scene<ComponentRotorType, EntityIndexType> build() const;
}; // SceneBuilder<ComponentRotorType, EntityIndexType>

template <class ...ComponentTypes, class ...IndexedTupleTypes>
struct SceneBuilder<
    component::Rotor<ComponentTypes...>,
    entity::Index<IndexedTupleTypes...>
    > {
 public:
  typedef component::Rotor<ComponentTypes...> RotorType;
  typedef entity::Index<IndexedTupleTypes...> IndexType;

  template <class ...TypesInTuple>
  SceneBuilder<
      RotorType,
      entity::Index<IndexedTupleTypes..., TypeList<TypesInTyple...>
      >
  withEntityIndex() {
    return decltype(withEntityIndex<TypesInTuple...>())(); // TODO
  }
  Scene<RotorType, IndexType> build() const {
    return Scene<RotorType, IndexType>(); // TODO
  }
}; // struct SceneBuilder<Rotor<ComponentTypes...>, Index<IndexedTupleTypes...>>

} // namespace gyros

#endif // include guard

