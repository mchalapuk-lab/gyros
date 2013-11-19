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

  template <class EmplacedComponentType, class ...ArgTypes>
  Builder<T, L...>& emplace(ArgTypes &&... args) {
    auto factory = std::bind(
        &detail::emplace<EmplacedComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );
    typedef typename detail::FindSubtype<Builder,
                                         EmplacedComponentType,
                                         T, L...>::type BuilderType;
    static_cast<BuilderType *>(this)->factories_.push_back(factory);
    return *this;
  }
  SceneType build() const {
    detail::SceneCreator<SceneType, T, L...> create;
    return create(*this);
  }

 private:
  std::vector<std::function<T *(void *)>> factories_;

  template <class SceneType, class ...ComponentTypes>
  friend class detail::SceneCreator;
}; // class Builder<T, L...>

} // namespace gyros

#endif // include guard

