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

template <class EntityType>
struct MakeIterators {
  typedef typename tl::Cast<Iterators, EntityType>::Type Type;
}; // struct MakeIterators<EntityType>

template <class HeadEntityType, class ...TailEntityTypes>
class Index<HeadEntityType, TailEntityTypes...>
  : private Index<TailEntityTypes...> {
 public:
  typedef typename MakeIterators<HeadEntityType>::Type IteratorsType;
  typedef tl::TypeList<HeadEntityType, TailEntityTypes...> EntityTypeList;
  typedef Index<HeadEntityType, TailEntityTypes...> Type;
  typedef Index<TailEntityTypes...> SuperType;

  template <class EntityType>
  using IndexOf = tl::IndexOf<EntityTypeList, EntityType>;

  template <class ...ArgTypes>
  Index(IteratorsType &&iterators, ArgTypes ...args) noexcept
    : SuperType(std::forward<ArgTypes>(args)...),
      iterators_(std::move(iterators)) {
  }
  template <class EntityType>
  typename MakeIterators<EntityType>::Type const& iterators() const noexcept {
    typedef typename GetAncestor<Type, IndexOf<EntityType>::value>::Type
        AncestorType;
    return AncestorType::iterators_;
  }

 protected:
  IteratorsType iterators_;
};

template <>
struct Index<> {
}; // struct Index<>

} // namespace entity
} // namespace gyros

#endif // include guard

