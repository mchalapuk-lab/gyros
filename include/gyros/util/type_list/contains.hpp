// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_CONTAINS_HPP_
#define GYROS_UTIL_TYPE_LIST_CONTAINS_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/size.hpp"

#include <type_traits>

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType, class NeedleType>
struct Contains {
}; // struct Contains<TypeListType, NeedleType>

template <class HeadType, class ...TailTypes, class NeedleType>
struct Contains<TypeList<HeadType, TailTypes...>, NeedleType> {
  constexpr static bool value = std::is_same<HeadType, NeedleType>::value
      || Contains<TypeList<TailTypes...>, NeedleType>::value;
}; // struct Contains<TypeList<HeadType, TailTypes...>, NeedleType>

template <class NeedleType>
struct Contains<TypeList<>, NeedleType> {
  constexpr static bool value = false;
}; // struct Contains<TypeList<>, NeedleType>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

