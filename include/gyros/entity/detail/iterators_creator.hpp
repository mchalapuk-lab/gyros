// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_DETAIL_ITERATORS_CREATOR_HPP_
#define GYROS_ENTITY_DETAIL_ITERATORS_CREATOR_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_literal.hpp"
#include "gyros/util/type_list.hpp"
#include "gyros/fwd/entity/iterators.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {
namespace detail {

template <class EntityType>
struct MakeIterators {
  typedef typename tl::Cast<Iterators, EntityType>::Type Type;
}; // struct MakeIterators<EntityType>

template <class EntityType, class BuildStateType>
class IteratorsCreator
  : public TypeTraits<IteratorsCreator<EntityType,BuildStateType>>::SuperType {
 public:
  typedef TypeTraits<IteratorsCreator<EntityType, BuildStateType>> Traits;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::HeadComponentType HeadComponentType;
  typedef typename MakeIterators<EntityType>::Type IteratorsType;

  constexpr IteratorsCreator(BuildStateType &state, size_t entity_count)
      : SuperType(state, entity_count) {
  }
  template <class ...ArgTypes>
  IteratorsType operator() (ArgTypes... args) const noexcept {
    return forward<IteratorsType>(std::forward<ArgTypes>(args)...);
  }

 protected:
  template <class CreatedIteratorsType, class ...ArgTypes>
  CreatedIteratorsType forward(ArgTypes... args) const noexcept {
    auto begin = this->state_.template it<HeadComponentType>();
    auto end = this->state_
        .template increment<HeadComponentType>(this->entity_count_);

    return SuperType::template forward<CreatedIteratorsType>(
        std::forward<ArgTypes>(args)..., begin, end
        );
  }
}; // class IteratorsCreator<EntityType, BuildStateType>

template <class BuildStateType>
class IteratorsCreator<tl::TypeList<>, BuildStateType> {
 public:
  constexpr IteratorsCreator(BuildStateType &state, size_t entity_count)
      : state_(state), entity_count_(entity_count) {
  }
 protected:
  template <class IteratorsType, class ...ArgTypes>
  IteratorsType forward(ArgTypes... args) const noexcept {
    return IteratorsType(std::forward<ArgTypes>(args)...);
  }
  BuildStateType &state_;
  size_t entity_count_;
}; // class IteratorsCreator<TypeList<>, BuildStateType>

template <class EntityType, class BuildStateType>
typename MakeIterators<EntityType>::Type createIterators(BuildStateType &state,
                                                         size_t entity_count) {
  IteratorsCreator<EntityType, BuildStateType> create(state, entity_count);
  return create();
}

} // namespace detail
} // namespace entity

template <class ...ComponentTypes, class BuildStateType>
struct TypeTraits<
    entity::detail::IteratorsCreator<
        tl::TypeList<ComponentTypes...>,
        BuildStateType
        >
    > {
  typedef tl::TypeList<ComponentTypes...> EntityType;
  typedef typename tl::PopBack<EntityType>::Type SuperEntityType;
  typedef typename tl::Get<EntityType, 0>::Type HeadComponentType;
  typedef entity::detail::IteratorsCreator<EntityType, BuildStateType> Type;
  typedef entity::detail::IteratorsCreator<SuperEntityType, BuildStateType>
      SuperType;
}; // TypeTraits<IteratorsCreator<TypeList<ComponentTypes...>, IteratorsType>>

template <class BuildStateType>
struct TypeTraits<
  entity::detail::IteratorsCreator<tl::TypeList<>, BuildStateType>
  > {
}; // TypeTraits<IteratorsCreator<TypeList<>, IteratorsType>>

} // namespace gyros

#endif // include guard

