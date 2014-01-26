// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_CAST_HPP_
#define GYROS_UTIL_TYPE_LIST_CAST_HPP_

#include "gyros/util/type_list/type_list.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <template <class ...> class TemplateType, class TypeListType>
struct Cast {
}; // Cast<template<...> TemplateType, TypeListType>

template <template <class ...> class TemplateType, class ...TypesInList>
struct Cast<TemplateType, TypeList<TypesInList...>> {
  typedef TemplateType<TypesInList...> Type;
}; // Cast<template<...> TemplateType, TypeList<TypesInList...>>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

