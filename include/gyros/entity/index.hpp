// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_INDEX_HPP_
#define GYROS_ENTITY_INDEX_HPP_

#include "gyros/util/type_list.hpp"
#include "gyros/entity/iterators.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {

template <class ...EntityTypes>
struct Index {
}; // Index<EntityTypes...>

template <class HeadEntityType, class ...TailEntityTypes>
class Index<HeadEntityType, TailEntityTypes...>
  : private Index<TailEntityTypes...> {
 public:
  typedef typename tl::Cast<Iterators, HeadEntityType>::Type IteratorsType;
  typedef Index<TailEntityTypes...> SuperType;

  template <class ...ArgTypes>
  Index(IteratorsType iterators, ArgTypes ...args) noexcept
    : SuperType(std::forward<ArgTypes>(args)...),
      iterators_(std::forward<IteratorsType>(iterators)) {
  }
  IteratorsType const& iterators() const noexcept {
    return iterators_;
  }
 private:
  IteratorsType iterators_;
};

template <>
struct Index<> {
}; // struct Index<>

} // namespace entity
} // namespace gyros

#endif // include guard

