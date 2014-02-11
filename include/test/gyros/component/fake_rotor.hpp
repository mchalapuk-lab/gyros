// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENT_FAKE_ROTOR_HPP_
#define TEST_GYROS_COMPONENT_FAKE_ROTOR_HPP_

#include "test/gyros/components.hpp"
#include "gyros/type_traits.hpp"
#include "gyros/util/type_literal.hpp"

namespace test {
namespace gyros {
namespace component {

template <class ...Types>
struct FakeRotor {
}; // struct FakeRotor<Types...>

template <class HeadType, class ...TailTypes>
class FakeRotor<HeadType, TailTypes...> : public FakeRotor<TailTypes...> {
 public:
  typedef ::gyros::TypeTraits<FakeRotor<HeadType, TailTypes...>> Traits;
  typedef typename Traits::ComponentTypes TypeList;
  typedef typename Traits::HeadComponentType HeadComponentType;
  typedef typename Traits::IteratorType IteratorType;
  typedef typename Traits::SuperType SuperType;

  template <class ...ArgTypes>
  FakeRotor(IteratorType begin, IteratorType end, ArgTypes ...args)
    : SuperType(std::forward<ArgTypes>(args)...), begin_(begin), end_(end) {
  }

  template <class ComponentType>
  typename ::gyros::TypeTraits<ComponentType>::IteratorType begin() {
    return begin(TypeLiteral<ComponentType>());
  }
  template <class ComponentType>
  typename ::gyros::TypeTraits<ComponentType>::IteratorType end() {
    return end(TypeLiteral<ComponentType>());
  }

 protected:
  using SuperType::begin;
  using SuperType::end;

  template <class Type>
  using TypeLiteral = ::gyros::util::TypeLiteral<Type>;

  IteratorType begin(TypeLiteral<HeadType>) const {
    return begin_;
  }
  IteratorType end(TypeLiteral<HeadType>) const {
    return end_;
  }

 private:
  IteratorType begin_;
  IteratorType end_;
}; // struct FakeRotor<HeadType, TailTypes...>

template<>
struct FakeRotor<> {
  struct Unused {};
  void begin(::gyros::util::TypeLiteral<Unused>) {}
  void end(::gyros::util::TypeLiteral<Unused>) {}
}; // struct FakeRotor<>

} // namespace component
} // namespace gyros
} // namespace test

namespace gyros {
namespace tl = util::type_list;

template <class ...Types>
struct TypeTraits<test::gyros::component::FakeRotor<Types...>> {
  template <class ...T>
  using FakeRotor = test::gyros::component::FakeRotor<T...>;

  typedef FakeRotor<Types...> Type;
  typedef tl::TypeList<Types...> ComponentTypes;
  typedef typename tl::Get<ComponentTypes, 0>::Type HeadComponentType;
  typedef typename TypeTraits<HeadComponentType>::IteratorType IteratorType;
  typedef typename tl::PopFront<ComponentTypes>::Type SuperComponentTypes;
  typedef typename tl::Cast<FakeRotor, SuperComponentTypes>::Type SuperType;
}; // TypeTraits<FakeRotor<Types...>>

} // namespace gyros

#endif // include guard

