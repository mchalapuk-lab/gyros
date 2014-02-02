// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_INDEX_BUILD_STATE_HPP_
#define GYROS_ENTITY_INDEX_BUILD_STATE_HPP_

#include "gyros/util/type_list.hpp"
#include "gyros/util/type_literal.hpp"
#include "gyros/component/iterator.hpp"
#include "gyros/component/rotor.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {
namespace detail {

template <class ...ComponentTypes>
struct BuildState {
}; // struct BuildState<ComponentTypes...>

template <class HeadComponentType, class ...TailComponentTypes>
class BuildState<HeadComponentType, TailComponentTypes...>
  : private BuildState<TailComponentTypes...> {
 public:
  typedef component::Rotor<HeadComponentType, TailComponentTypes...> RotorType;
  typedef component::PositionIterator<HeadComponentType> IteratorType;
  typedef tl::TypeList<HeadComponentType, TailComponentTypes...>
      ComponentTypeList;

  BuildState(RotorType &rotor) noexcept
      : BuildState<TailComponentTypes...>(rotor),
        it_(rotor.template begin<HeadComponentType>()) {
  }
  template <class ComponentType>
  component::PositionIterator<ComponentType>& it() noexcept {
    static_assert(tl::Contains<ComponentTypeList, ComponentType>::value,
                  "requested component not contained in the type list");
    return it(util::TypeLiteral<ComponentType>());
  }

 protected:
  IteratorType& it(util::TypeLiteral<HeadComponentType>) noexcept {
    return it_;
  }
 private:
  IteratorType it_;
}; // BuildState<HeadComponentType, TailComponentTypes...>

template <>
struct BuildState<> {
  template <class RotorType>
  BuildState(RotorType const&) {}
}; // BuildState<>

template <class ...ComponentTypes>
BuildState<ComponentTypes...>
makeBuildState(component::Rotor<ComponentTypes...> &rotor) {
  return BuildState<ComponentTypes...>(rotor);
}

} // namespace detail
} // namespace entity
} // namespace gyros

#endif // include guard

