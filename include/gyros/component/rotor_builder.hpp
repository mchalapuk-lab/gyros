// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_BUILDER_HPP_
#define GYROS_COMPONENT_ROTOR_BUILDER_HPP_

#include <vector>
#include <functional>

#include "gyros/component/detail/find_subtype.hpp"
#include "gyros/component/detail/rotor_creator.hpp"
#include "gyros/fwd/component/rotor.hpp"
#include "gyros/fwd/component/rotor_builder.hpp"

namespace gyros {
namespace component {

template <class ...ComponentTypes>
struct RotorBuilder {
}; // struct Builder

template <class T, class ...L>
class RotorBuilder<T, L...> : private RotorBuilder<L...> {
 public:
  typedef Rotor<T, L...> RotorType;

  template <class EmplacedComponentType, class ...ArgTypes>
  RotorBuilder<T, L...>& emplace(ArgTypes &&... args) {
    auto factory = std::bind(
        &detail::emplace<EmplacedComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );
    return addFactory<EmplacedComponentType>(std::move(factory));
  }

  template <class EmplacedComponentType, class FactoryType>
  RotorBuilder<T, L...>& addFactory(FactoryType factory) {
    typedef typename detail::FindSubtype<RotorBuilder,
                                         EmplacedComponentType,
                                         T, L...>::type BuilderType;
    BuilderType *that = static_cast<BuilderType *>(this);
    that->factories_.push_back(std::forward<FactoryType>(factory));
    return *this;
  }

  RotorType build() const {
    detail::RotorCreator<RotorType, T, L...> create;
    return create(*this);
  }
 private:
  std::vector<std::function<T *(void *)>> factories_;

  template <class RotorType, class ...ComponentTypes>
  friend class detail::RotorCreator;
}; // class RotorBuilder<T, L...>

} // namespace component
} // namespace gyros

#endif // include guard

