// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_SCENE_HPP_
#define GYROS_SCENE_HPP_

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/fwd/scene.hpp"

namespace gyros {

template <class ComponentRotorType, class EntityIndexType>
struct SceneBuilder {
  Scene<ComponentRotorType, EntityIndexType> build() const;
}; // SceneBuilder<ComponentRotorType, EntityIndexType>

template <class ...ComponentTypes, class ...IndexedTupleTypes>
struct Builder<
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
}; // struct Builder<Rotor<ComponentTypes...>, Index<IndexedTupleTypes...>>

} // namespace gyros

#endif // include guard

