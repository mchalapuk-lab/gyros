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
class IteratorsCreator {
 public:
  typedef typename MakeIterators<EntityType>::Type IteratorsType;

  constexpr IteratorsCreator(BuildStateType &state, size_t entity_count)
      : incrementer_{ state, entity_count } {
  }
  IteratorsType operator() () const noexcept {
    RecursiveForwardType build0 { incrementer_, Finisher() };
    return build0();
  }

 private:
  template <class CurrentType>
  struct Next {
    static constexpr size_t next_index
        = tl::IndexOf<EntityType, CurrentType>::value + 1;
    typedef typename tl::Get<EntityType, next_index>::Type Type;
  }; // struct Next

  struct Finisher {
    template <class ...ArgTypes>
    IteratorsType operator() (ArgTypes... args) const noexcept {
      return IteratorsType(std::forward<ArgTypes>(args)...);
    }
  }; // class Finisher

  struct IteratorsIncrementer {
    template <class CurrentType, class ForwarderType, class ...ArgTypes>
    IteratorsType operator() (ForwarderType &&forward,
                              ArgTypes... args) const noexcept {
      auto begin = state_.template it<CurrentType>();
      auto end = state_.template increment<CurrentType>(entity_count_);

      return forward(std::forward<ArgTypes>(args)..., begin, end);
    }
    BuildStateType &state_;
    size_t entity_count_;
  }; // struct IteratorsIncrementer

  typedef RecursiveForward<typename tl::Front<EntityType>::Type,
                           typename tl::Back<EntityType>::Type,
                           Next,
                           IteratorsIncrementer const,
                           Finisher const,
                           IteratorsType> RecursiveForwardType;
 
  IteratorsIncrementer incrementer_;
}; // class IteratorsCreator<EntityType, BuildStateType>

template <class EntityType, class BuildStateType>
typename MakeIterators<EntityType>::Type createIterators(BuildStateType &state,
                                                         size_t entity_count) {
  IteratorsCreator<EntityType, BuildStateType> create(state, entity_count);
  return create();
}

} // namespace detail
} // namespace entity

} // namespace gyros

#endif // include guard

