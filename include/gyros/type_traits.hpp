// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_TYPE_TRAITS_HPP_
#define GYROS_TYPE_TRAITS_HPP_

namespace gyros {

template <class Type>
struct TypeTraits {
  typedef Type* IteratorType;
}; // struct TypeTraits<Type>

} // namespace gyros

#endif // include guard

