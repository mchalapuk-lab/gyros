// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_POP_FRONT_HPP_
#define GYROS_UTIL_TYPE_LIST_POP_FRONT_HPP_

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/cat.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType>
struct PopFront;

template <class HeadType, class ...TailTypes>
struct PopFront<TypeList<HeadType, TailTypes...>> {
  typedef TypeList<TailTypes...> Type;
}; // PopFront<TypeList<HeadType, TailTypes...>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

