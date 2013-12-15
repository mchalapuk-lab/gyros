// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_PUSH_BACK_HPP_
#define GYROS_UTIL_TYPE_LIST_PUSH_BACK_HPP_

#include "gyros/util/type_list/type_list.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType, class TypeToPush>
struct PushBack {
}; // PushBack<TypeListType, TypeToPush>

template <class ...TypesInList, class TypeToPush>
struct PushBack<TypeList<TypesInList...>, TypeToPush> {
  typedef TypeList<TypesInList..., TypeToPush> Type;
}; // PushBack<TypeList<TypesInList...>, TypeToPush>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

