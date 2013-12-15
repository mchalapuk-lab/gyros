// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_CAT_HPP_
#define GYROS_UTIL_TYPE_LIST_CAT_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/push_back.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class ...TypeLists>
struct Cat {
}; // struct Cat<TypeLists...>

template <class FirstTypeList, class SecondTypeList, class ...Tail>
struct Cat<FirstTypeList, SecondTypeList, Tail...> {
  typedef typename Cat<FirstTypeList, SecondTypeList>::Type FirstTwoLists;
  typedef typename Cat<FirstTwoLists, Tail...>::Type Type;
}; // struct Cat<FirstTypeList, SecondTypeList, Tail...>

template <class TypeListType, class HeadType, class ...TailTypes>
struct Cat<TypeListType, TypeList<HeadType, TailTypes...>> {
  typedef typename PushBack<TypeListType, HeadType>::Type WithHeadTypePushed;
  typedef typename Cat<WithHeadTypePushed, TypeList<TailTypes...>>::Type Type;
}; // Cat<TypeListType, TypeList<HeadType, TailTypes...>>

template <class TypeListType>
struct Cat<TypeListType, TypeList<>> {
  // invoking Cat once again increases type safety
  typedef typename Cat<TypeListType>::Type Type;
}; // Cat<TypeListType, TypeList<>>

template <class ...Types>
struct Cat<TypeList<Types...>> {
  typedef TypeList<Types...> Type;
}; // Cat<TypeList<Types...>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

