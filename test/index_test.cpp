// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#include "gyros/entity/index.hpp"

#include "test/gyros/components.hpp"

template <class ...Types>
using TypeList = gyros::util::type_list::TypeList<Types...>;
using Simple = test::gyros::component::EmptyComponent;
using Mock = test::gyros::component::MockComponent;

class entity_Index : public ::testing::TestWithParam<ptrdiff_t> {
};

TEST_F(entity_Index, test_creating_empty_index) {
  gyros::entity::Index<> index;
}

