// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/entity/index_builder.hpp"

#include "test/gyros/components.hpp"
#include "test/gyros/component/fake_rotor.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Complex = test::gyros::component::OneMemberComponent<int>;

using namespace ::testing;

template <class ...Types>
using FakeRotor = test::gyros::component::FakeRotor<Types...>;
template <class ...Types>
using IndexBuilder = gyros::entity::IndexBuilder<Types...>;

template <class Type>
using Traits = gyros::TypeTraits<Type>;

namespace gyros {
// for simplicity of testing pointers will be used
// instead of component::PositionIterator
template<> struct TypeTraits<Simple> { typedef Simple* IteratorType; };
template<> struct TypeTraits<Complex> { typedef Complex* IteratorType; };
namespace detail {
// rotor types must be FakeRotor
template <>
struct MakeRotor<TypeList<Simple>> {
  typedef FakeRotor<Simple> Type;
}; // class MakeRotor<TypeList<Simple>>
template <>
struct MakeRotor<TypeList<Simple>, TypeList<Complex>> {
  typedef FakeRotor<Simple, Complex> Type;
}; // class MakeRotor<TypeList<Simple>, TypeList<Complex>>
template <>
struct MakeRotor<TypeList<Simple>, TypeList<Simple, Complex>> {
  typedef FakeRotor<Simple, Complex> Type;
}; // class MakeRotor<TypeList<Simple>, TypeList<Simple, Complex>>
} // namespace detail
} // namespace gyros

struct entity_IndexBuilder : public ::testing::TestWithParam<ptrdiff_t> {
  size_t count;

  entity_IndexBuilder()
      : count(1024) {
  }

  template <class Type>
  typename Traits<Type>::IteratorType iterator() {
    return typename Traits<Type>::IteratorType();
  }
};

TEST_F(entity_IndexBuilder, test_creating_empty_builder) {
  IndexBuilder<> builder;
}

TEST_F(entity_IndexBuilder,
       test_begin_equals_end_after_building_index_with_zero_components) {
  FakeRotor<Simple> rotor { iterator<Simple>(), iterator<Simple>() };

  auto index = IndexBuilder<>()
      .setEntityCount<TypeList<Simple>>(0)
      .build(rotor);
  auto iterators = index.iterators<TypeList<Simple>>();
  ASSERT_EQ(0, iterators.end<0>() - iterators.begin<0>());
}

TEST_F(entity_IndexBuilder,
       test_ptrdiff_after_building_index_with_entities_of_one_type) {
  FakeRotor<Simple> rotor { iterator<Simple>(), iterator<Simple>() + count };

  auto index = IndexBuilder<>()
      .setEntityCount<TypeList<Simple>>(count)
      .build(rotor);
  auto iterators = index.iterators<TypeList<Simple>>();
  ASSERT_EQ(count, iterators.end<0>() - iterators.begin<0>());
}

TEST_F(entity_IndexBuilder,
       test_ptrdiff_after_building_index_with_two_entities) {
  FakeRotor<Simple, Complex> rotor {
    iterator<Simple>(), iterator<Simple>() + count,
    iterator<Complex>(), iterator<Complex>() + count,
  };

  auto index = IndexBuilder<>()
      .setEntityCount<TypeList<Simple>>(count)
      .setEntityCount<TypeList<Complex>>(count)
      .build(rotor);
  auto iterators0 = index.iterators<TypeList<Simple>>();
  ASSERT_EQ(count, iterators0.end<0>() - iterators0.begin<0>());
  auto iterators1 = index.iterators<TypeList<Complex>>();
  ASSERT_EQ(count, iterators1.end<0>() - iterators1.begin<0>());
}

