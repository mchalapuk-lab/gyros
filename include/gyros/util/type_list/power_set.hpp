// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_POWER_SET_HPP_
#define GYROS_UTIL_TYPE_LIST_POWER_SET_HPP_

#include "gyros/util/type_list/type_list.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType>
struct PowerSet {
}; // struct PowerSet<TypeListType>

template <class SingleType>
struct PowerSet<TypeList<SingleType>> {
  typedef TypeList<TypeList<>, TypeList<SingleType>> Type;
}; // struct PowerSet<TypeList<SingleType>>

template <>
struct PowerSet<TypeList<>> {
  typedef TypeList<TypeList<>> Type;
}; // struct PowerSet<TypeList<>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

