// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_MAKE_BUILDER_TYPE_HPP_
#define GYROS_MAKE_BUILDER_TYPE_HPP_

#include <type_traits>

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/contains.hpp"
#include "gyros/util/type_list/cat.hpp"

#include "gyros/fwd/component/rotor.hpp"
#include "gyros/fwd/entity/index.hpp"
#include "gyros/fwd/builder.hpp"

namespace gyros {
namespace detail {

template <class RotorType, class ...ComponentTypesToAdd>
struct AddTypesToRotor {
}; // struct AddTypesToRotor<RotorType, ComponentTypesToAdd...>

} // namespace detail

template <class ...TupleTypes>
struct MakeBuilder {
  typedef Builder<
      typename detail::AddTypesToRotor<
          component::Rotor<>,
          typename util::type_list::Cat<TupleTypes...>::Type
          >::Type,
      entity::Index<TupleTypes...>
      > Type;
}; // MakeBuilderType<TupleTypes...>

namespace detail {

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

