// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_INDEX_BUILDER_HPP_
#define GYROS_ENTITY_INDEX_BUILDER_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list.hpp"
#include "gyros/detail/make_rotor_type.hpp"
#include "gyros/entity/detail/index_build_state.hpp"
#include "gyros/entity/detail/iterators_creator.hpp"
#include "gyros/entity/index.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {

template <class ...EntityTypes>
class IndexBuilder
  : private TypeTraits<IndexBuilder<EntityTypes...>>::SuperType {
 public:
  typedef TypeTraits<IndexBuilder<EntityTypes...>> Traits;
  typedef typename Traits::Type Type;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::RotorType RotorType;
  typedef typename Traits::IndexType IndexType;
  typedef typename Traits::BuildStateType BuildStateType;
  typedef typename Traits::HeadEntityType HeadEntityType;
  typedef typename Traits::IteratorsType IteratorType;

  template <class NextEntityType>
  using ExtendWith = IndexBuilder<EntityTypes..., NextEntityType>;

  IndexBuilder(SuperType const& previousStep, size_t entity_count) noexcept
      : SuperType(previousStep), entity_count_(entity_count) {
  }
  IndexBuilder(IndexBuilder const& builder) noexcept
      : SuperType(builder), entity_count_(builder.entity_count_) {
  }
  IndexBuilder(IndexBuilder &&builder) noexcept
      : SuperType(std::move(builder)), entity_count_(builder.entity_count_) {
  }

  template <class EntityType>
  ExtendWith<EntityType> setEntityCount(size_t entity_count) const noexcept {
    return ExtendWith<EntityType>(*this, entity_count);
  }
  IndexType build(RotorType &rotor) noexcept {
    auto state = detail::createIndexBuildState(rotor);
    return createIndex<IndexType>(state);
  }

 protected:
  template <class CreatedIndexType, class ...ArgTypes>
  CreatedIndexType createIndex(BuildStateType &state,
                               ArgTypes... args) const noexcept {
    auto iterators = detail::createIterators<HeadEntityType>(state,
                                                             entity_count_);
    SuperType const* that = static_cast<SuperType const*>(this);
    return that->createIndex<CreatedIndexType>(state,
                                               std::forward<ArgTypes>(args)...,
                                               std::move(iterators));
  }
 private:
  size_t entity_count_;
}; // struct IndexBuilder<EntityTypes...>

template <>
struct IndexBuilder<> {
  template <class EntityType>
  IndexBuilder<EntityType> setEntityCount(size_t entity_count) const noexcept {
    return IndexBuilder<EntityType>(*this, entity_count);
  }
  template <class IndexType, class BuildStateType, class ...ArgTypes>
  IndexType createIndex(BuildStateType&, ArgTypes... args) const noexcept {
    return IndexType(std::forward<ArgTypes>(args)...);
  }
}; // struct IndexBuilder<>

} // namespace entity

template <class ...EntityTypes>
struct TypeTraits<entity::IndexBuilder<EntityTypes...>> {
  typedef entity::IndexBuilder<EntityTypes...> Type;
  typedef entity::Index<EntityTypes...> IndexType;
  typedef tl::TypeList<EntityTypes...> EntityTypesList;
  typedef typename tl::PopBack<EntityTypesList>::Type SuperEntityTypesList;
  typedef typename tl::Cast<entity::IndexBuilder, SuperEntityTypesList>::Type
      SuperType;
  typedef typename detail::MakeRotor<EntityTypes...>::Type RotorType;
  typedef typename RotorType::TypeList RotorTypeList;
  typedef typename tl::Cast<
      entity::detail::IndexBuildState, RotorTypeList
      >::Type BuildStateType;
  typedef typename tl::Get<EntityTypesList, 0>::Type HeadEntityType;
  typedef typename tl::Cast<entity::Iterators, HeadEntityType>::Type
      IteratorsType;
}; // TypeTraits<IndexBuilder<EntityTypes...>>

} // namespace gyros

#endif // include guard

