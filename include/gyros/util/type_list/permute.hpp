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

template <class ...Types>
struct Permute<TypeList<Types...>> {
  typedef TypeList<TypeList<Types>...> Type;
}; // struct Permute<TypeList<Types...>>

template <class FirstSet, class SecondSet, class ...Tail>
struct Permute<FirstSet, SecondSet, Tail...> {
  typedef typename Permute<FirstSet, SecondSet>::Type FirstPairPermuted;
  typedef typename Permute<FirstPairPermuted, Tail...>::Type Type;
}; // struct Permute<FirstSet, SecondSet, Tail...>

template <class FirstSetHead, class ...FirstSetTail, class ...SecondSet>
struct Permute<TypeList<FirstSetHead, FirstSetTail...>, TypeList<SecondSet...>>{
  typedef typename Permute<TypeList<FirstSetHead>,
                           TypeList<SecondSet...>>::Type HeadPermuted;
  typedef typename Permute<TypeList<FirstSetTail...>,
                           TypeList<SecondSet...>>::Type TailPermuted;
  typedef typename Cat<HeadPermuted, TailPermuted>::Type Type;
}; // struct Permute<TypeList<FirstSetHead, FirstSetTail...>, SecondSet>

template <class ...TypesInFirstListOfFirstSet,
          class ...TypesInFirstListOfSecondSet,
          class ...SecondSetTail>
struct Permute<
    TypeList<TypeList<TypesInFirstListOfFirstSet...>>,
    TypeList<TypeList<TypesInFirstListOfSecondSet...>, SecondSetTail...>
    > {
  typedef TypeList<TypeList<TypesInFirstListOfFirstSet...,
                            TypesInFirstListOfSecondSet...>> HeadPermuted;
  typedef typename Permute<
      TypeList<TypeList<TypesInFirstListOfFirstSet...>>,
      TypeList<SecondSetTail...>
          >::Type TailPermuted;
  typedef typename Cat<HeadPermuted, TailPermuted>::Type Type; 
}; // struct (well, complicated...)

template <class ...Types>
struct Permute<TypeList<TypeList<Types...>>, TypeList<>> {
  typedef TypeList<> Type;
}; // struct Permute<TypeList<TypeList<Types...>>, TypeList<>>

template <>
struct Permute<TypeList<>> {
  typedef TypeList<> Type;
}; // struct Permute<TypeList<Types...>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

