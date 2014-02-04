// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENT_MOCK_ROTOR_HPP_
#define TEST_GYROS_COMPONENT_MOCK_ROTOR_HPP_

#include "test/gyros/components.hpp"
#include "gyros/type_traits.hpp"
#include "gyros/util/type_literal.hpp"

#include <gmock/gmock.h>

namespace test {
namespace gyros {
namespace component {

template <class ...Types>
struct MockRotor {
}; // struct MockRotor<Types...>

template <class HeadType, class ...TailTypes>
struct MockRotor<HeadType, TailTypes...> : public MockRotor<TailTypes...> {
  template <class Type> using TypeLiteral = ::gyros::util::TypeLiteral<Type>;
  typedef typename ::gyros::TypeTraits<HeadType>::IteratorType IteratorType;

  MOCK_METHOD1_T(begin, IteratorType (TypeLiteral<HeadType>));
  MOCK_METHOD1_T(end, IteratorType (TypeLiteral<HeadType>));

  template <class ComponentType>
  typename ::gyros::TypeTraits<ComponentType>::IteratorType begin() {
    return begin(TypeLiteral<ComponentType>());
  }
  template <class ComponentType>
  typename ::gyros::TypeTraits<ComponentType>::IteratorType end() {
    return end(TypeLiteral<ComponentType>());
  }
}; // struct MockRotor<HeadType, TailTypes...>

template<>
struct MockRotor<> {
}; // struct MockRotor<>

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

