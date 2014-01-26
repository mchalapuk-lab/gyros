// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_POP_BACK_HPP_
#define GYROS_UTIL_TYPE_LIST_POP_BACK_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/cat.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType>
struct PopBack {
}; // PopBack<TypeListType>

template <class HeadType, class ...TailTypes>
struct PopBack<TypeList<HeadType, TailTypes...>> {
  typedef typename Cat<
      TypeList<HeadType>,
      typename PopBack<TypeList<TailTypes...>>::Type
      >::Type Type;
}; // PopBack<TypeList<HeadType, TailTypes...>>

template <class TypeInList>
struct PopBack<TypeList<TypeInList>> {
  typedef TypeList<> Type;
}; // PopBack<TypeList<TypeInList>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

