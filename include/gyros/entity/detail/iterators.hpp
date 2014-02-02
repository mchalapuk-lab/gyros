// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_DETAIL_ITERATORS_HPP_
#define GYROS_ENTITY_DETAIL_ITERATORS_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list.hpp"
#include "gyros/util/type_literal.hpp"
#include "gyros/component/iterator.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {
namespace detail {

template <class ...ComponentTypes>
struct Iterators {
}; // struct Iterators<ComponentTypes...>

template <class HeadComponentType, class ...TailComponentTypes>
class Iterators<HeadComponentType, TailComponentTypes...>
  : private Iterators<TailComponentTypes...> {
 public:
  typedef Iterators<HeadComponentType, TailComponentTypes...> Type;
  typedef TypeTraits<Type> Traits;
  typedef typename Traits::ComponentTypeList ComponentTypeList;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::IteratorType IteratorType;

  template <class ...ArgTypes>
  Iterators(IteratorType begin, IteratorType end, ArgTypes... args) noexcept
    : SuperType(std::forward<ArgTypes>(args)...), begin_(begin), end_(end) {
  }

  template <class ComponentType>
  component::PositionIterator<ComponentType> begin() const noexcept {
    static_assert(tl::Contains<ComponentTypeList, ComponentType>::value,
                  "requested component not contained in the type list");
    return begin(util::TypeLiteral<ComponentType>());
  }
  template <class ComponentType>
  component::PositionIterator<ComponentType> end() const noexcept {
    static_assert(tl::Contains<ComponentTypeList, ComponentType>::value,
                  "requested component not contained in the type list");
    return end(util::TypeLiteral<ComponentType>());
  }

 protected:
  IteratorType begin(util::TypeLiteral<HeadComponentType>) const noexcept {
    return begin_;
  }
  IteratorType end(util::TypeLiteral<HeadComponentType>) const noexcept {
    return end_;
  }

 private:
  IteratorType begin_;
  IteratorType end_;
}; // class Iterators<HeadComponentType, TailComponentTypes...>

template <>
struct Iterators<> {
}; // struct Iterators<>

} // namespace detail
} // namespace entity

template <class ...ComponentTypes>
struct TypeTraits<entity::detail::Iterators<ComponentTypes...>> {
  typedef tl::TypeList<ComponentTypes...> ComponentTypeList;
  typedef entity::detail::Iterators<ComponentTypes...> Type;
  typedef typename tl::PopFront<ComponentTypeList>::Type SuperComponentTypes;
  typedef typename tl::Get<ComponentTypeList, 0>::Type HeadComponentType;
  typedef typename TypeTraits<HeadComponentType>::IteratorType IteratorType;
  typedef typename tl::Cast<entity::detail::Iterators,
                            SuperComponentTypes>::Type SuperType;
}; // struct TypeTraits<Iterators<ComponentTypes...>>

} // namespace gyros

#endif // include guard

