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
  typedef ::gyros::TypeTraits<MockRotor<HeadType, TailTypes...>> Traits;
  typedef typename Traits::HeadComponentType HeadComponentType;
  typedef typename Traits::IteratorType IteratorType;
  typedef typename Traits::SuperType SuperType;

  template <class Type>
  using TypeLiteral = ::gyros::util::TypeLiteral<Type>;
  using SuperType::begin;
  using SuperType::end;
  using SuperType::gmock_begin;
  using SuperType::gmock_end;

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
  struct Unused {};
  void begin(::gyros::util::TypeLiteral<Unused>) {}
  void end(::gyros::util::TypeLiteral<Unused>) {}
  void gmock_begin(::gyros::util::TypeLiteral<Unused>) {}
  void gmock_end(::gyros::util::TypeLiteral<Unused>) {}
}; // struct MockRotor<>

} // namespace component
} // namespace gyros
} // namespace test

namespace gyros {
namespace tl = util::type_list;

template <class ...Types>
struct TypeTraits<test::gyros::component::MockRotor<Types...>> {
  template <class ...T>
  using MockRotor = test::gyros::component::MockRotor<T...>;

  typedef MockRotor<Types...> Type;
  typedef tl::TypeList<Types...> ComponentTypes;
  typedef typename tl::Get<ComponentTypes, 0>::Type HeadComponentType;
  typedef typename TypeTraits<HeadComponentType>::IteratorType IteratorType;
  typedef typename tl::PopFront<ComponentTypes>::Type SuperComponentTypes;
  typedef typename tl::Cast<MockRotor, SuperComponentTypes>::Type SuperType;
}; // TypeTraits<MockRotor<Types...>>

} // namespace gyros

#endif // include guard

