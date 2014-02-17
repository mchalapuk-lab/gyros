// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_FRONT_HPP_
#define GYROS_UTIL_TYPE_LIST_FRONT_HPP_

#include <cstddef>

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/get.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType>
struct Front {
  typedef typename Get<TypeListType, 0>::Type Type;
}; // struct Front<TypeListType>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

