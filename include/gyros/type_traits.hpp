// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_TYPE_TRAITS_HPP_
#define GYROS_TYPE_TRAITS_HPP_

#include <cstddef>

#include "gyros/fwd/component/iterator.hpp"

namespace gyros {

template <class Type>
struct TypeTraits {
  typedef component::PositionIterator<Type> IteratorType;
}; // struct TypeTraits<Type>

template <class IteratorsType, size_t ancestor_level>
struct GetAncestor {
  typedef typename GetAncestor<
      typename IteratorsType::SuperType,
      ancestor_level - 1>::Type Type;
}; // GetAncestor

template <class IteratorsType>
struct GetAncestor<IteratorsType, 0> {
  typedef IteratorsType Type;
}; // GetAncestor<IteratorsType, 0>

} // namespace gyros

#endif // include guard

