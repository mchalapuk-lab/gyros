// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/scene.hpp"

#include "gyros/detail/builder.hpp"

namespace gyros {

template <class ComponentRotorType, class EntityIndexType>
struct Builder {
}; // Builder<ComponentRotorType, EntityIndexType>

typedef Builder<component::Rotor<>, entity::Index<>> SceneBuilder;

template <class ...ComponentTypes, class ...IndexedTupleTypes>
struct Builder<
    component::Rotor<ComponentTypes...>,
    entity::Index<IndexedTupleTypes...>
    > {
 public:
  typedef component::Rotor<ComponentTypes...> RotorType;
  typedef entity::Index<IndexedTupleTypes...> IndexType;
  template <class ...Types>
  using TypeList = util::type_list::TypeList<Types...>;

  template <class ...TypesInTuple>
  Builder<
      typename detail::AddTypesToRotor<RotorType, TypesInTuple...>::Type,
      entity::Index<IndexedTupleTypes..., TypeList<TypesInTuple...>>
      >
  withEntity() {
    return decltype(withEntity<TypesInTuple...>())();
  }
}; // struct Builder<Rotor<ComponentTypes...>, Index<IndexedTupleTypes...>>

} // namespace gyros

#endif // include guard

