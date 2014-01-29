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
#include "gyros/component/detail/rotor_creator.hpp"

#include <functional>

namespace gyros {
namespace entity {
namespace tl = util::type_list;

template <class SceneBuilderType, class ComponentTypeList>
struct Builder {
}; // class Builder<SceneBuilderType, ComponentTypeList>

template <class SceneBuilderType, class ...ComponentTypes>
class Builder<SceneBuilderType, tl::TypeList<ComponentTypes...>>
  : private detail::BuilderBase<tl::TypeList<ComponentTypes...>> {
 public:
  typedef Builder<SceneBuilderType, tl::TypeList<ComponentTypes...>> Type;
  typedef TypeTraits<Type> Traits;
  typedef typename Traits::BaseType BaseType;
  typedef typename Traits::BaseTraits::SuperType BaseSuperType;

  template <class ComponentType>
  using ExtendWith = typename Traits::template ExtendWith<ComponentType>::Type;

  template <class FactoryType>
  Builder(BaseSuperType &&previousStep,
          SceneBuilderType &builder,
          FactoryType factory)
    : BaseType(std::move(previousStep), std::forward<FactoryType>(factory)),
      scene_builder_(builder),
      destroyer_(std::bind(&Type::finalize, this)) {
  }
  Builder(Builder &&rhs)
    : BaseType(std::move(rhs)),
      scene_builder_(rhs.scene_builder_),
      destroyer_(std::bind(&Type::finalize, this)) {
    rhs.destroyer_ = [] {};
  }
  ~Builder() {
    destroyer_();
  }

  template <class ComponentType, class ...ArgTypes>
  ExtendWith<ComponentType> emplace(ArgTypes&&... args) {
    auto factory = std::bind(
        &component::detail::emplace<ComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );

    destroyer_ = [] {};
    return ExtendWith<ComponentType>(
        std::move(*this),
        scene_builder_,
        std::move(factory)
        );
  }

 private:
  SceneBuilderType &scene_builder_;
  std::function<void ()> destroyer_;

  void finalize() {
    scene_builder_.addEntity(BaseType(std::move(*this)));
  }
}; // Builder<SceneBuilderType, TypeList<ComponentTypes...>>

template <class SceneBuilderType>
class Builder<SceneBuilderType, tl::TypeList<>>
  : private detail::BuilderBase<tl::TypeList<>> {
 public:
  typedef Builder<SceneBuilderType, tl::TypeList<>> Type;
  typedef TypeTraits<Type> Traits;
  typedef typename Traits::BaseType BaseType;

  template <class ComponentType>
  using ExtendWith = typename Traits::template ExtendWith<ComponentType>::Type;

  Builder(SceneBuilderType &builder)
      : scene_builder_(builder) {
  }

  template <class ComponentType, class ...ArgTypes>
  ExtendWith<ComponentType> emplace(ArgTypes&&... args) {
    auto factory = std::bind(
        &component::detail::emplace<ComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );

    return ExtendWith<ComponentType>(
        std::move(*this),
        scene_builder_,
        std::move(factory)
        );
  }

 private:
  SceneBuilderType &scene_builder_;
}; // Builder<SceneBuilderType, TypeList<>>

} // namespace entity

template <class SceneBuilderType, class ...ComponentTypes>
struct TypeTraits<
    entity::Builder<SceneBuilderType, tl::TypeList<ComponentTypes...>>
    > {
  typedef tl::TypeList<ComponentTypes...> ComponentTypeList;
  typedef entity::Builder<SceneBuilderType, ComponentTypeList> Type;
  typedef entity::detail::BuilderBase<ComponentTypeList> BaseType;
  typedef TypeTraits<BaseType> BaseTraits;

  template <class ComponentType>
  struct ExtendWith {
    typedef entity::Builder<
        SceneBuilderType,
        tl::TypeList<ComponentTypes..., ComponentType>
        > Type;
  }; // struct ExtendWith<ComponentType>
}; // struct TypeTraits<entity::Builder<SceneBuilderType, ComponentTypes...>>

} // namespace gyros

#endif // include guard

