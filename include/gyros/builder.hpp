// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include <vector>
#include <functional>

#include "gyros/detail/findsubtype.hpp"
#include "gyros/detail/builder.hpp"
#include "gyros/fwd/scene.hpp"
#include "gyros/fwd/builder.hpp"

namespace gyros {

template <class ...ComponentTypes>
struct Builder {
}; // struct Builder

template <class T, class ...L>
class Builder<T, L...> : private Builder<L...> {
 public:
  typedef Scene<T, L...> SceneType;

  EntityBuilder<T, L...> newEntity() {
    return EntityBuilder<T, L...>(*this);
  }
  SceneType build() const {
    detail::SceneCreator<SceneType, T, L...> create;
    return create(*this);
  }

 private:
  std::vector<std::function<T *(void *)>> factories_;

  template <class ...ComponentTypes>
  friend class EntityBuilder;
  template <class SceneType, class ...ComponentTypes>
  friend class detail::SceneCreator;
}; // class Builder<T, L...>

template <class ...ComponentTypes>
class EntityBuilder {
 public:
  EntityBuilder(EntityBuilder<ComponentTypes...> &&builder) noexcept
      : scene_builder_(builder.scene_builder_) {
  }

  template <class EmplacedComponentType, class ...ArgTypes>
  EntityBuilder<ComponentTypes...>& emplace(ArgTypes &&... args) {
    auto factory = std::bind(
        &detail::emplace<EmplacedComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );
    typedef typename detail::FindSubtype<Builder,
                                         EmplacedComponentType,
                                         ComponentTypes...>::type BuilderType;
    pushFactory<BuilderType>(scene_builder_, factory);
    return *this;
  }
 private:
  EntityBuilder(Builder<ComponentTypes...> &scene_builder)
      : scene_builder_(scene_builder) {
  }

  Builder<ComponentTypes...> &scene_builder_;

  template <class BuilderType, class Factory>
  void pushFactory(BuilderType &builder, Factory const& factory) {
    builder.factories_.push_back(factory);
  }

  friend class Builder<ComponentTypes...>;
}; // class EntityBuilder

} // namespace gyros

#endif // include guard

