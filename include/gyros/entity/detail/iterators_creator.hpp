// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_DETAIL_ITERATORS_CREATOR_HPP_
#define GYROS_ENTITY_DETAIL_ITERATORS_CREATOR_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list.hpp"
#include "gyros/util/type_literal.hpp"
#include "gyros/entity/detail/iterators.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {
namespace detail {

template <class EntityType, class IteratorsType>
class IteratorsCreator
  : private TypeTraits<IteratorsCreator<EntityType, IteratorsType>>::SuperType {
 public:
  typedef TypeTraits<IteratorsCreator<EntityType, IteratorsType>> Traits;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::HeadComponentType HeadComponentType;

  template <class BuildStateType, class ...ArgTypes>
  IteratorsType operator() (BuildStateType &state,
                            size_t entity_count,
                            ArgTypes... args) const noexcept {
    auto &it = state.template it<HeadComponentType>();
    auto begin = it;
    it += entity_count;
    auto end = it;

    SuperType const* that = static_cast<SuperType const*>(this);
    return that->operator()
        (state, std::forward<ArgTypes>(args)..., entity_count, begin, end);
  }
}; // IteratorsCreator<EntityType, IteratorsType>

template <class IteratorsType>
struct IteratorsCreator<tl::TypeList<>, IteratorsType> {
  template <class BuildStateType, class ...ArgTypes>
  IteratorsType operator() (BuildStateType &,
                            size_t,
                            ArgTypes... args) const noexcept {
    return IteratorsType(std::forward<ArgTypes>(args)...);
  }
}; // struct IteratorsCreator<TypeList<>, IteratorsType>

template <class EntityType,
          class BuildStateType,
          class IteratorsType = typename tl::Cast<Iterators, EntityType>::Type>
IteratorsType createIterators(BuildStateType &state, size_t entity_count) {
  IteratorsCreator<EntityType,
                   typename tl::Cast<Iterators, EntityType>::Type> create;
  create(state, entity_count);
}

} // namespace detail
} // namespace entity

template <class ...ComponentTypes, class IteratorsType>
struct TypeTraits<
    entity::detail::IteratorsCreator<
        tl::TypeList<ComponentTypes...>,
        IteratorsType
        >
    > {
  typedef tl::TypeList<ComponentTypes...> EntityType;
  typedef typename tl::PopBack<EntityType>::Type SuperEntityType;
  typedef typename tl::Get<EntityType, 0>::Type HeadComponentType;
  typedef entity::detail::IteratorsCreator<EntityType, IteratorsType> Type;
  typedef entity::detail::IteratorsCreator<SuperEntityType, IteratorsType>
      SuperType;
}; // TypeTraits<IteratorsCreator<TypeList<ComponentTypes...>, IteratorsType>>

template <class IteratorsType>
struct TypeTraits<
  entity::detail::IteratorsCreator<tl::TypeList<>, IteratorsType>
  > {
}; // TypeTraits<IteratorsCreator<TypeList<>, IteratorsType>>

} // namespace gyros

#endif // include guard

