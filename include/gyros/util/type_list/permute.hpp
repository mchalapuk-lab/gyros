// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_PERMUTE_HPP_
#define GYROS_UTIL_TYPE_LIST_PERMUTE_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/cat.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class ...TypeLists>
struct Permute {
}; // struct Permute<TypeLists...>

template <class LeftHead, class ...LeftTail, class RightSets>
struct Permute<TypeList<LeftHead, LeftTail...>, RightSets> {
  typedef typename Permute<TypeList<LeftHead>, RightSets>::Type HeadPermuted;
  typedef typename Permute<TypeList<LeftTail...>, RightSets>::Type TailPermuted;
  typedef typename Cat<HeadPermuted, TailPermuted>::Type Type;
}; // struct Permute<TypeList<LeftHead, LeftTail...>, RightSets>

template <class ...L, class ...R, class ...T>
struct Permute<TypeList<TypeList<L...>>, TypeList<TypeList<R...>, T...>> {
  typedef TypeList<TypeList<L..., R...>> HeadPermuted;
  typedef typename Permute<TypeList<TypeList<L...>>, T...>::Type TailPermuted;
  typedef typename Cat<HeadPermuted, TailPermuted>::Type Type; 
}; // struct Permute<TypeList<TypeList<L...>>, TypeList<TypeList<R...>, T...>>

template <class ...Types>
struct Permute<TypeList<Types...>, TypeList<>> {
  typedef TypeList<Types...> Type;
}; // struct Permute<TypeList<Types...>, TypeList<>>

template <class ...Types>
struct Permute<TypeList<>, TypeList<Types...>> {
  typedef TypeList<Types...> Type;
}; // struct Permute<TypeList<>, TypeList<Types...>>

template <class ...Types>
struct Permute<TypeList<Types...>> {
  typedef TypeList<TypeList<Types>...> Type;
}; // struct Permute<TypeList<Types...>>

template <>
struct Permute<TypeList<>> {
  typedef TypeList<> Type;
}; // struct Permute<TypeList<Types...>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

