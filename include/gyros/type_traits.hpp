// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_TYPE_TRAITS_HPP_
#define GYROS_TYPE_TRAITS_HPP_

#include <cstddef>
#include <utility>

#include "gyros/fwd/component/iterator.hpp"

namespace gyros {

template <class Type>
struct TypeTraits {
  typedef component::PositionIterator<Type> IteratorType;
}; // struct TypeTraits<Type>

template <class CurrentType, size_t level>
struct GetAncestor {
  typedef typename GetAncestor<typename CurrentType::SuperType, level-1>::Type
      Type;
}; // GetAncestor

template <class CurrentType>
struct GetAncestor<CurrentType, 0> {
  typedef CurrentType Type;
}; // GetAncestor<IteratorsType, 0>

template <
    class CurrentType,
    class StopType,
    template <class> class TypeTransformer,
    class ReturnType = void
    >
struct RecursiveForward {
  template <
      class Forwarder,
      class Finisher,
      class ...ArgTypes
      >
  ReturnType operator() (Forwarder fwd, Finisher fin, ArgTypes ...args) const {
    typedef typename TypeTransformer<CurrentType>::Type NextType;

    return fwd.template operator()<CurrentType>(
        RecursiveForward<NextType, StopType, TypeTransformer, ReturnType>(),
        std::forward<Forwarder>(fwd),
        std::forward<Finisher>(fin),
        std::forward<ArgTypes>(args)...
        );
  }
}; // struct RecursiveForward<CurrentType, StopType, Transformer, ReturnType>

template <
    class StopType,
    template <class> class TypeTransformer,
    class ReturnType
    >
struct RecursiveForward<StopType, StopType, TypeTransformer, ReturnType> {
  template <
      class Forwarder,
      class Finisher,
      class ...ArgTypes
      >
  ReturnType operator() (Forwarder, Finisher fin, ArgTypes ...args) const {
    return fin(std::forward<ArgTypes>(args)...);
  }
}; // struct RecursiveForward<StopType, StopType, Transformer, ReturnType>

} // namespace gyros

#endif // include guard

