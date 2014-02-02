// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/scene.hpp"

#include "gyros/util/type_list.hpp"
#include "test/gyros/components.hpp"
#include "test/static_assert.hpp"

template <class Type>
using Member = test::gyros::component::OneMemberComponent<Type>;
using Simple = test::gyros::component::EmptyComponent;
using Mock = test::gyros::component::MockComponent;

namespace tl = gyros::util::type_list;

template <class ...Types>
using Iterators = gyros::entity::detail::Iterators<Types...>;
template <class Type>
using Traits = gyros::TypeTraits<Type>;

namespace gyros {
// for simplicity of testing pointers will be used
// instead of component::PositionIterator
template<> struct TypeTraits<Simple> { typedef Simple* IteratorType; };
template<> struct TypeTraits<Mock> { typedef Mock* IteratorType; };
template<class T> struct TypeTraits<Member<T>> {
  typedef Member<T>* IteratorType;
};
} // namespace gyros

namespace {
// static tests

void test_type_list_type_in_traits_of_iterators_with_one_component() {
  typedef tl::TypeList<Simple> ExpectedType;
  typedef typename Traits<Iterators<Simple>>::ComponentTypeList ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_type_list_type_in_traits_of_iterators_with_three_components() {
  typedef tl::TypeList<Simple, Mock, Simple> ExpectedType;
  typedef typename Traits<Iterators<Simple, Mock, Simple>>::ComponentTypeList
      ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_type_in_traits_of_iterators_with_one_component() {
  typedef Iterators<Simple> ExpectedType;
  typedef typename Traits<Iterators<Simple>>::Type ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_type_in_traits_of_iterators_with_three_components() {
  typedef Iterators<Simple, Mock, Simple> ExpectedType;
  typedef typename Traits<Iterators<Simple, Mock, Simple>>::Type ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_head_component_type_in_traits_of_iterators_with_one_component() {
  typedef Simple ExpectedType;
  typedef typename Traits<Iterators<Simple>>::HeadComponentType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_head_component_type_in_traits_of_iterators_with_three_components() {
  typedef Mock ExpectedType;
  typedef typename Traits<Iterators<Mock, Simple, Simple>>::HeadComponentType
      ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

void test_iterator_type_in_traits_of_iterators_with_one_component() {
  typedef Simple* ExpectedType;
  typedef typename Traits<Iterators<Simple>>::IteratorType ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}
void test_iterator_type_in_traits_of_iterators_with_three_components() {
  typedef Mock* ExpectedType;
  typedef typename Traits<Iterators<Mock, Simple, Simple>>::IteratorType
      ActualType;
  test::AssertIsSame<ExpectedType, ActualType>();
}

} // namespace

// dynamic tests

