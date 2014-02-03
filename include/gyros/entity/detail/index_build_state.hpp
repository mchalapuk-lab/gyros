// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_INDEX_BUILD_STATE_HPP_
#define GYROS_ENTITY_INDEX_BUILD_STATE_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list.hpp"
#include "gyros/util/type_literal.hpp"
#include "gyros/component/iterator.hpp"
#include "gyros/component/rotor.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {
namespace detail {

template <class ...ComponentTypes>
struct IndexBuildState {
}; // struct IndexBuildState<ComponentTypes...>

template <class HeadComponentType, class ...TailComponentTypes>
class IndexBuildState<HeadComponentType, TailComponentTypes...>
  : private IndexBuildState<TailComponentTypes...> {
 public:
  typedef IndexBuildState<HeadComponentType, TailComponentTypes...> Type;
  typedef TypeTraits<Type> Traits;
  typedef typename Traits::RotorType RotorType;
  typedef typename Traits::ComponentTypeList ComponentTypeList;
  typedef typename Traits::IteratorType IteratorType;
  typedef typename Traits::SuperType SuperType;

  template <class ComponentType>
  using IndexOf = tl::IndexOf<ComponentTypeList, ComponentType>;

  IndexBuildState(RotorType &rotor) noexcept
      : SuperType(rotor), it_(rotor.template begin<HeadComponentType>()) {
  }
  template <class ComponentType>
  component::PositionIterator<ComponentType>& it() noexcept {
    static_assert(tl::Contains<ComponentTypeList, ComponentType>::value,
                  "requested component not contained in the type list");
    typedef typename GetAncestor<Type, IndexOf<ComponentType>::value>::Type
        AncestorType;
    return AncestorType::it_;
  }

 protected:
  IteratorType it_;
}; // IndexBuildState<HeadComponentType, TailComponentTypes...>

template <>
struct IndexBuildState<> {
  template <class RotorType>
  IndexBuildState(RotorType const&) {}
}; // IndexBuildState<>

template <class ...ComponentTypes>
IndexBuildState<ComponentTypes...>
createIndexBuildState(component::Rotor<ComponentTypes...> &rotor) {
  return IndexBuildState<ComponentTypes...>(rotor);
}

} // namespace detail
} // namespace entity

template <class ...ComponentTypes>
struct TypeTraits<entity::detail::IndexBuildState<ComponentTypes...>> {
  typedef entity::detail::IndexBuildState<ComponentTypes...> Type;
  typedef component::Rotor<ComponentTypes...> RotorType;
  typedef tl::TypeList<ComponentTypes...> ComponentTypeList;
  typedef typename tl::Get<ComponentTypeList, 0>::Type HeadComponentType;
  typedef typename TypeTraits<HeadComponentType>::IteratorType IteratorType;
  typedef typename tl::PopFront<ComponentTypeList>::Type SuperTypeList;
  typedef typename tl::Cast<
      entity::detail::IndexBuildState, SuperTypeList
      >::Type SuperType;
}; // struct TypeTraits<IndexBuildState<ComponentTypes...>>

} // namespace gyros

#endif // include guard

