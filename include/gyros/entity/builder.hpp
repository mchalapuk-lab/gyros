// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_BUILDER_HPP_
#define GYROS_ENTITY_BUILDER_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/back.hpp"
#include "gyros/util/type_list/pop_back.hpp"

#include "gyros/entity/detail/builder_base.hpp"
#include "gyros/component/rotor_builder.hpp"

#include <functional>

namespace gyros {
namespace entity {
namespace tl = util::type_list;

template <class ...ComponentTypes>
class Builder : private detail::BuilderBase<tl::TypeList<ComponentTypes...>> {
 public:
  typedef TypeTraits<Builder<ComponentTypes...>> Traits;
  typedef typename Traits::BaseType BaseType;
  typedef typename Traits::BaseTraits::SuperType BaseSuperType;

  template <class FactoryType>
  Builder(BaseSuperType &&previousStep, FactoryType factory)
    : BaseType(std::move(previousStep), std::forward<FactoryType>(factory)) {
  }
  Builder(Builder const&) = delete;
  Builder(Builder &&rhs): BaseType(std::move(rhs)) {
  }

  template <class ComponentType, class ...ArgTypes>
  Builder<ComponentTypes..., ComponentType> emplace(ArgTypes&&... args) {
    auto factory = std::bind(
        &component::allocateComponent<ComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );

    return Builder<ComponentTypes..., ComponentType>(std::move(*this),
                                                     std::move(factory));
  }

  using BaseType::addFactoriesTo;
}; // Builder<TypeList<ComponentTypes...>>

template <>
class Builder<> : private detail::BuilderBase<tl::TypeList<>> {
 public:
  template <class ComponentType, class ...ArgTypes>
  Builder<ComponentType> emplace(ArgTypes&&... args) {
    auto factory = std::bind(
        &component::allocateComponent<ComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );

    return Builder<ComponentType>(std::move(*this), std::move(factory));
  }
}; // Builder<TypeList<>>

} // namespace entity

template <class ...ComponentTypes>
struct TypeTraits<entity::Builder<ComponentTypes...>> {
  typedef entity::Builder<ComponentTypes...> Type;
  typedef entity::detail::BuilderBase<tl::TypeList<ComponentTypes...>> BaseType;
  typedef TypeTraits<BaseType> BaseTraits;
}; // struct TypeTraits<entity::Builder<ComponentTypes...>>

} // namespace gyros

#endif // include guard

