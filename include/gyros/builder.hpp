// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/util/type_list.hpp"
#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/entity/builder.hpp"
#include "gyros/scene.hpp"

#include "gyros/fwd/builder.hpp"

namespace gyros {
namespace tl = util::type_list;

template <class ...EntityTypes>
class Builder {
}; // Builder<EntitytTypes...>

template <class HeadEntityType, class ...TailEntityTypes> 
class Builder<HeadEntityType, TailEntityTypes...>
  : public Builder<TailEntityTypes...> {
 public:
  typedef TypeTraits<Builder<HeadEntityType, TailEntityTypes...>> Traits;
  typedef typename Traits::Type Type;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::SceneType SceneType;
  typedef typename Traits::EmptyEntityBuilderType EmptyEntityBuilderType;
  typedef typename Traits::EntityBuilderType EntityBuilderType;

  EmptyEntityBuilderType newEntity() {
    return EmptyEntityBuilderType(*this);
  }
  SceneType build() {
    throw "not implemented"; // TODO
  }

  Builder& addEntity(EntityBuilderType &&entity_builder) {
    entity_builders_.emplace_back(std::move(entity_builder));
  }
 private:
  std::vector<EntityBuilderType> entity_builders_;
}; // Builder<HeadEntityType, TailEntityTypes...>

template <> 
class Builder<> {
}; // Builder<>

template <class HeadType, class ...TailTypes> 
struct TypeTraits<Builder<HeadType, TailTypes...>> {
  typedef Builder<HeadType, TailTypes...> Type;
  typedef Builder<TailTypes...> SuperType;
  typedef Scene<HeadType, TailTypes...> SceneType;
  typedef entity::Builder<Type, tl::TypeList<>> EmptyEntityBuilderType;
  typedef entity::Builder<Type, HeadType> EntityBuilderType;
}; // TypeTraits<Builder<HeadType, TailTypes...>

} // namespace gyros

#endif // include guard

