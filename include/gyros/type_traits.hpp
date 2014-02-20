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

template <class FunctionType>
struct FuncPointerReturn {
}; // struct FuncPointerReturn

template <class ReturnType, class ...ArgTypes>
struct FuncPointerReturn<ReturnType (*)(ArgTypes...)> {
  typedef ReturnType Type;
}; // struct FuncPointerReturn<ReturnType (*)(ArgTypes...)>

template <
    class CurrentType,
    class StopType,
    template <class> class TypeTransformer,
    class WorkerType,
    class FinisherType,
    class ReturnType = typename FuncPointerReturn<
                       decltype(&FinisherType::operator())>::Type
    >
struct RecursiveForward {
  typedef RecursiveForward<
      typename TypeTransformer<CurrentType>::Type,
      StopType,
      TypeTransformer,
      WorkerType,
      FinisherType,
      ReturnType> NextForwardType;

  template <class ...ArgTypes>
  ReturnType operator() (ArgTypes ...args) const {
    return work_.template operator()<CurrentType>(
        NextForwardType { work_, finish_ },
        std::forward<ArgTypes>(args)...
        );
  }

  WorkerType &work_;
  FinisherType &finish_;
}; // struct RecursiveForward

template <
    class StopType,
    template <class> class TypeTransformer,
    class WorkerType,
    class FinisherType,
    class ReturnType
    >
struct RecursiveForward<
    StopType, StopType, TypeTransformer, WorkerType, FinisherType, ReturnType
    > {
  template <class ...ArgTypes>
  ReturnType operator() (ArgTypes ...args) const {
    return work_.template operator()<StopType>(
        finish_, // finisher will be called as last forward object
        std::forward<ArgTypes>(args)...
        );
  }

  WorkerType &work_;
  FinisherType &finish_;
}; // struct RecursiveForward<StopType, StopType, ...>

} // namespace gyros

#endif // include guard

