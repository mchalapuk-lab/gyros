// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_BUILDER_HPP_
#define GYROS_COMPONENT_ROTOR_BUILDER_HPP_

#include <vector>
#include <functional>

#include "gyros/util/type_literal.hpp"
#include "gyros/component/detail/rotor_creator.hpp"
#include "gyros/fwd/component/rotor.hpp"
#include "gyros/fwd/component/rotor_builder.hpp"

namespace gyros {
namespace component {

template <class ...ComponentTypes>
struct RotorBuilder {
}; // struct Builder

template <class T, class ...L>
class RotorBuilder<T, L...> : protected RotorBuilder<L...> {
 public:
  typedef RotorBuilder<T, L...> Type;
  typedef RotorBuilder<L...> SuperType;
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
    addFactory0(std::forward<FactoryType>(factory),
                util::TypeLiteral<EmplacedComponentType>());
    return *this;
  }

  RotorType build() const {
    detail::RotorCreator<RotorType, T, L...> create;
    return create(*this);
  }
 protected:
  std::vector<std::function<T *(void *)>> factories_;

  using SuperType::addFactory0;
  template <class FactoryType>
  void addFactory0(FactoryType factory, util::TypeLiteral<T>) {
    factories_.push_back(std::forward<FactoryType>(factory));
  }

  template <class RotorType, class ...ComponentTypes>
  friend class detail::RotorCreator;
}; // class RotorBuilder<T, L...>

template <>
struct RotorBuilder<> {
  void addFactory0() {
  }
}; // struct RotorBuilder<>

} // namespace component
} // namespace gyros

#endif // include guard

