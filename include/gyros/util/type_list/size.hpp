// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_SIZE_HPP_
#define GYROS_UTIL_TYPE_LIST_SIZE_HPP_

#include <cstddef>

#include "gyros/util/type_list/type_list.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType>
struct Size {
}; // struct Size<TypeListType>

template <class HeadType, class ...TailTypes>
struct Size<TypeList<HeadType, TailTypes...>> {
  static constexpr size_t value = 1 + Size<TypeList<TailTypes...>>::value;
}; // Size<TypeList<HeadType, TailTypes...>>

template <>
struct Size<TypeList<>> {
  static constexpr size_t value = 0u;
}; // Size<TypeList<>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

