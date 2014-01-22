// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_SCENE_HPP_
#define GYROS_SCENE_HPP_

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/scene.hpp"

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/contains.hpp"

namespace gyros {

template <class ComponentRotorType, class EntityIndexType>
struct Builder {
}; // Builder<ComponentRotorType, EntityIndexType>

typedef Builder<component::Rotor<>, entity::Index<>> SceneBuilder;

template <class RotorType, class ...ComponentTypesToAdd>
struct AddTypesToRotor {
}; // struct AddTypesToRotor<RotorType, ComponentTypesToAdd...>

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
      typename AddTypesToRotor<RotorType, TypesInTuple...>::Type,
      entity::Index<IndexedTupleTypes..., TypeList<TypesInTuple...>>
      >
  withEntity() {
    return decltype(withEntity<TypesInTuple...>())();
  }
}; // struct Builder<Rotor<ComponentTypes...>, Index<IndexedTupleTypes...>>

template <class ...ExistingTypes, class Head, class ...Tail>
struct AddTypesToRotor<component::Rotor<ExistingTypes...>, Head, Tail...> {
  typedef typename std::conditional<
      util::type_list::Contains<
          util::type_list::TypeList<ExistingTypes...>,
          Head
          >::value,
      typename AddTypesToRotor<
          component::Rotor<ExistingTypes...>,
          Tail...
          >::Type,
      typename AddTypesToRotor<
          component::Rotor<ExistingTypes..., Head>,
          Tail...
          >::Type
      >::type Type;
}; // struct AddTypesToRotor<Rotor<ExistingTypes...>, Head, Tail...>

template <class ...ExistingTypes>
struct AddTypesToRotor<component::Rotor<ExistingTypes...>> {
  typedef component::Rotor<ExistingTypes...> Type;
}; // struct AddTypesToRotor<Rotor<ExistingTypes...>>

} // namespace gyros

#endif // include guard

