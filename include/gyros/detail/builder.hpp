// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_DETAIL_BUILDER_HPP_
#define GYROS_DETAIL_BUILDER_HPP_

#include "gyros/fwd/component/rotor.hpp"

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/contains.hpp"

namespace gyros {
namespace detail {

template <class RotorType, class ...ComponentTypesToAdd>
struct AddTypesToRotor {
}; // struct AddTypesToRotor<RotorType, ComponentTypesToAdd...>

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

} // namespace detail
} // namespace gyros

#endif // include guard

