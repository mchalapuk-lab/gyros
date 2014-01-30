// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_INDEX_HPP_
#define GYROS_ENTITY_INDEX_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list.hpp"

namespace gyros {
namespace entity {
namespace tl = util::type_list;

namespace detail {

template <class ...ComponentTypes>
struct Iterators {
}; // struct Iterators<ComponentTypes>

template <class HeadComponentType, class ...TailComponentTypes>
class Iterators<HeadComponentType, TailComponentTypes...>
  : private Iterators<TailComponentTypes...> {
 public:
  typedef typename TypeTraits<HeadComponentType>::IteratorType IteratorType;

  IteratorType begin_;
  IteratorType end_;
}; // class Iterators<HeadType, TailTypes>

template <>
struct Iterators<> {
}; // struct Iterators<>

} // namespace detail

template <class ...EntityTypes>
struct Index {
}; // struct Index<EntityTypes...>

template <class HeadEntityType, class ...TailEntityTypes>
class Index<HeadEntityType, TailEntityTypes...>
  : private Index<TailEntityTypes...> {
 public:
  typedef typename tl::Cast<detail::Iterators, HeadEntityType>::Type
      IteratorsType;
  typedef Index<TailEntityTypes...> SuperType;

  template <class ...ArgTypes>
  Index(IteratorsType iterators, ArgTypes ...args)
    : SuperType(std::forward<ArgTypes>(args)...),
      iterators_(std::forward<IteratorsType>(iterators)) {
  }
  Index() {}
  
 private:
  IteratorsType iterators_;
};

} // namespace entity
} // namespace gyros

#endif // include guard

