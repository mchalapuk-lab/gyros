// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_GET_HPP_
#define GYROS_UTIL_TYPE_LIST_GET_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/size.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType, size_t index>
struct Get {
  static_assert(Size<TypeListType>::value > index, "type index out of range");
}; // struct Get<TypeListType, index>

template <class HeadType, class ...TailTypes, size_t index>
struct Get<TypeList<HeadType, TailTypes...>, index> {
  typedef typename Get<TypeList<TailTypes...>, index - 1>::Type Type;
}; // struct Get<TypeList<HeadType, TailTypes...>, index>

template <class HeadType, class ...TailTypes>
struct Get<TypeList<HeadType, TailTypes...>, 0> {
  typedef HeadType Type;
}; // struct Get<TypeList<HeadType, TailTypes...>, 0>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

