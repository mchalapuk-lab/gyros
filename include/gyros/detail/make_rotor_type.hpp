// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_DETAIL_MAKE_ROTOR_TYPE_HPP_
#define GYROS_DETAIL_MAKE_ROTOR_TYPE_HPP_

#include <type_traits>

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/contains.hpp"
#include "gyros/util/type_list/cat.hpp"

#include "gyros/fwd/component/rotor.hpp"

namespace gyros {
namespace detail {

template <class RotorType, class ...ComponentTypesToAdd>
struct AddTypesToRotor {
}; // struct AddTypesToRotor<RotorType, ComponentTypesToAdd...>

template <class ...TupleTypes>
struct MakeRotor {
  typedef typename detail::AddTypesToRotor<
      component::Rotor<>,
      typename util::type_list::Cat<TupleTypes...>::Type
      >::Type Type;
}; // MakeRotor<TupleTypes...>

template <class ...ExistingTypes, class Head, class ...Tail>
struct AddTypesToRotor<
    component::Rotor<ExistingTypes...>,
    util::type_list::TypeList<Head, Tail...>
    > {
  typedef typename std::conditional<
      util::type_list::Contains<
          util::type_list::TypeList<ExistingTypes...>,
          Head
          >::value,
      typename AddTypesToRotor<
          component::Rotor<ExistingTypes...>,
          util::type_list::TypeList<Tail...>
          >::Type,
      typename AddTypesToRotor<
          component::Rotor<ExistingTypes..., Head>,
          util::type_list::TypeList<Tail...>
          >::Type
      >::type Type;
}; // struct AddTypesToRotor<Rotor<ExistingTypes...>, TypeList<Head, Tail...>>

template <class ...ExistingTypes>
struct AddTypesToRotor<
    component::Rotor<ExistingTypes...>,
    util::type_list::TypeList<>
    > {
  typedef component::Rotor<ExistingTypes...> Type;
}; // struct AddTypesToRotor<Rotor<ExistingTypes...>, TypeList<>>

} // namespace detail
} // namespace gyros

#endif // include guard

