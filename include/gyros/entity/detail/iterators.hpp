// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_DETAIL_ITERATORS_HPP_
#define GYROS_ENTITY_DETAIL_ITERATORS_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list.hpp"
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

  template <size_t index>
  using TypeAt = typename tl::Get<ComponentTypeList, index>::Type;

  template <class ...ArgTypes>
  constexpr Iterators(IteratorType begin,
                      IteratorType end,
                      ArgTypes... args) noexcept
    : SuperType(std::forward<ArgTypes>(args)...), begin_(begin), end_(end) {
  }

  template <size_t index>
  typename TypeTraits<TypeAt<index>>::IteratorType begin() const noexcept {
    typedef typename GetAncestor<Type, index>::Type AncestorType;
    return AncestorType::begin_;
  }
  template <size_t index>
  typename TypeTraits<TypeAt<index>>::IteratorType end() const noexcept {
    typedef typename GetAncestor<Type, index>::Type AncestorType;
    return AncestorType::end_;
  }

 protected:
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

