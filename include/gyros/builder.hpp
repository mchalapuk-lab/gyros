// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/fwd/builder.hpp"

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/scene.hpp"

namespace gyros {

template <class ComponentRotorType, class EntityIndexType>
struct Builder {
}; // Builder<ComponentRotorType, EntityIndexType>

template <class ...ComponentTypes, class ...IndexedTupleTypes>
struct Builder<
    component::Rotor<ComponentTypes...>,
    entity::Index<IndexedTupleTypes...>
    > {
 public:
  typedef component::Rotor<ComponentTypes...> RotorType;
  typedef entity::Index<IndexedTupleTypes...> IndexType;
}; // struct Builder<Rotor<ComponentTypes...>, Index<IndexedTupleTypes...>>

} // namespace gyros

#endif // include guard

