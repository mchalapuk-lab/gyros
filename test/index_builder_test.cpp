// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/entity/index_builder.hpp"

#include "test/gyros/components.hpp"
#include "test/gyros/component/mock_rotor.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Complex = test::gyros::component::OneMemberComponent<int>;

using namespace ::testing;

template <class ...Types>
using MockRotor = test::gyros::component::MockRotor<Types...>;
template <class ...Types>
using IndexBuilder = gyros::entity::IndexBuilder<Types...>;

namespace gyros {
namespace detail {

template <>
struct MakeRotor<TypeList<Simple>> {
  typedef MockRotor<Simple> Type;
}; // class MakeRotor<TypeList<Simple>>

template <>
struct MakeRotor<TypeList<Simple>, TypeList<Complex>> {
  typedef MockRotor<Simple, Complex> Type;
}; // class MakeRotor<TypeList<Simple>, TypeList<Complex>>

template <>
struct MakeRotor<TypeList<Simple>, TypeList<Simple, Complex>> {
  typedef MockRotor<Simple, Complex> Type;
}; // class MakeRotor<TypeList<Simple>, TypeList<Simple, Complex>>

} // namespace detail
} // namespace gyros

struct entity_IndexBuilder : public ::testing::TestWithParam<ptrdiff_t> {
  entity_IndexBuilder()
      : count(1024) {
  }
  size_t count;
};

TEST_F(entity_IndexBuilder, test_creating_empty_builder) {
  IndexBuilder<> builder;
}

TEST_F(entity_IndexBuilder,
       test_begin_equals_end_after_building_index_with_zero_components) {
  MockRotor<Simple> rotor;

  auto index = IndexBuilder<>()
      .setEntityCount<TypeList<Simple>>(0)
      .build(rotor);
  auto iterators = index.iterators<TypeList<Simple>>();
  ASSERT_EQ(0, iterators.end<0>() - iterators.begin<0>());
}

TEST_F(entity_IndexBuilder,
       test_ptrdiff_after_building_index_with_one_entity) {
  MockRotor<Simple> rotor;

  auto index = IndexBuilder<>()
      .setEntityCount<TypeList<Simple>>(count)
      .build(rotor);
  auto iterators = index.iterators<TypeList<Simple>>();
  ASSERT_EQ(count, iterators.end<0>() - iterators.begin<0>());
}

TEST_F(entity_IndexBuilder,
       test_ptrdiff_after_building_index_with_two_entities) {
  MockRotor<Simple, Complex> rotor;

  auto index = IndexBuilder<>()
      .setEntityCount<TypeList<Simple>>(count)
      .setEntityCount<TypeList<Complex>>(count)
      .build(rotor);
  auto iterators0 = index.iterators<TypeList<Simple>>();
  ASSERT_EQ(count, iterators0.end<0>() - iterators0.begin<0>());
  auto iterators1 = index.iterators<TypeList<Complex>>();
  ASSERT_EQ(count, iterators1.end<0>() - iterators1.begin<0>());
}

