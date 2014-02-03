// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_INDEX_OF_HPP_
#define GYROS_UTIL_TYPE_LIST_INDEX_OF_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/size.hpp"

#include <type_traits>

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType, class NeedleType, size_t index = 0>
struct IndexOf {
}; // struct IndexOf<TypeListType, NeedleType>

template <class HeadType, class ...TailTypes, class NeedleType, size_t index>
struct IndexOf<TypeList<HeadType, TailTypes...>, NeedleType, index> {
  constexpr static size_t value =
      IndexOf<TypeList<TailTypes...>, NeedleType, index + 1>::value;
}; // struct IndexOf<TypeList<HeadType, TailTypes...>, NeedleType, index>

template <class ...TailTypes, class NeedleType, size_t index>
struct IndexOf<TypeList<NeedleType, TailTypes...>, NeedleType, index> {
  constexpr static size_t const value = index;
}; // struct IndexOf<TypeList<NeedleType, TailTypes...>, NeedleType, index>

template <class NeedleType, size_t index>
struct IndexOf<TypeList<>, NeedleType, index> {
  constexpr static size_t value = -1;
}; // struct IndexOf<TypeList<>, NeedleType>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

