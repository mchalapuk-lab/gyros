// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/fwd/builder.hpp"

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/scene.hpp"
#include "gyros/util/type_list/back.hpp"

#include "gyros/detail/make_rotor_type.hpp"
#include "gyros/component/detail/rotor_creator.hpp"

namespace gyros {

template <class ...ComponentTypes>
class EntityBuilder {
 public:
  EntityBuilder() {
  }
  template <class ComponentType, class ...ArgTypes>
  EntityBuilder<ComponentTypes..., ComponentType> emplace(ArgTypes&&... args) {
    auto factory = std::bind(
        &component::detail::emplace<ComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );
    return EntityBuilder<ComponentTypes..., ComponentType>(factory);
  }
 private:
  typedef typename util::type_list::Back<ComponentTypes...>::Type
      LastComponentType;
  std::function<LastComponentType *(void *)> factory_;  

  template <class FactoryType>
  EntityBuilder(FactoryType factory) : factory_(factory) {
  }
}; // EntityBuilder<ComponentTypes...>

template <>
class EntityBuilder<> {
 public:
  template <class ComponentType, class ...ArgTypes>
  EntityBuilder<ComponentType> emplace(ArgTypes&&... args) {
    auto factory = std::bind(
        &component::detail::emplace<ComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );
    return EntityBuilder<ComponentType>(factory);
  }
}; // EntityBuilder<>

template <class ...TupleTypes>
class Builder {
 public:
  typedef typename detail::MakeRotor<TupleTypes...>::Type RotorType;
  typedef entity::Index<TupleTypes...> IndexType;
  
  EntityBuilder<TupleTypes...> newEntity() {
    return EntityBuilder<TupleTypes...>();
  }
}; // Builder<TupleTypes...>

} // namespace gyros

#endif // include guard

