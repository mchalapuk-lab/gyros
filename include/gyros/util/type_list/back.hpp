// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_UTIL_TYPE_LIST_BACK_HPP_
#define GYROS_UTIL_TYPE_LIST_BACK_HPP_

#include <cstddef>

#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/size.hpp"
#include "gyros/util/type_list/get.hpp"

namespace gyros {
namespace util {
namespace type_list {

template <class TypeListType>
struct Back {
  typedef typename Get<TypeListType, Size<TypeListType>::value - 1>::Type Type;
}; // struct Back<TypeListType>

} // namespace type_list
} // namespace util
} // namespace gyros

#endif // include guard

