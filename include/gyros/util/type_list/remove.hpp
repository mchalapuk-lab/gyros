// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_REMOVE_HPP_
#define GYROS_UTIL_TYPE_LIST_REMOVE_HPP_

#include "gyros/util/type_list/front.hpp"
#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/contains.hpp"
#include "gyros/util/type_list/index_of.hpp"
#include "gyros/util/type_list/pop_front.hpp"
#include "gyros/util/type_list/cat.hpp"

#include <type_traits>

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType, class RemovedType>
struct Remove {
  typedef TypeListType Type;
}; // struct Remove<TypeListType, RemovedType>

template <class ...Types, class RemovedType>
struct Remove<TypeList<Types...>, RemovedType> {
  typedef TypeList<Types...> TypeListType;
  typedef typename PopFront<TypeListType>::Type WithoutFrontType;
  typedef typename Remove<WithoutFrontType, RemovedType>::Type WithTypeRemoved;
  typedef typename Front<TypeListType>::Type FirstType;
  typedef typename Cat<TypeList<FirstType>, WithTypeRemoved>::Type Type;
}; // struct Remove<TypeList<Types...>, RemovedType>

template <class ...AfterRemoved, class RemovedType>
struct Remove<TypeList<RemovedType, AfterRemoved...>, RemovedType> {
  typedef TypeList<AfterRemoved...> Type;
}; // struct Remove<TypeList<RemovedType, After...>, RemovedType>

template <class RemovedType>
struct Remove<TypeList<>, RemovedType> {
  typedef TypeList<> Type;
}; // struct Remove<TypeList<>, RemovedType>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

