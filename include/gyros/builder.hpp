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
  typedef typename SceneType::RotorType RotorType;
  typedef typename SceneType::IndexType IndexType;
  typedef typename RotorType::BuilderType RotorBuilderType;

  EmptyEntityBuilderType newEntity() {
    return EmptyEntityBuilderType(*this);
  }
  SceneType build() {
    RotorBuilderType rotor_builder;
    addFactoriesTo(rotor_builder);
    return SceneType(rotor_builder.build(), IndexType());
  }

  Builder& addEntity(EntityBuilderType &&entity_builder) {
    entity_builders_.emplace_back(std::move(entity_builder));
  }
 private:
  std::vector<EntityBuilderType> entity_builders_;

  void addFactoriesTo(RotorBuilderType &rotor_builder) const {
    for (auto it = entity_builders_.begin(), end = entity_builders_.end();
         it != end;
         ++it) {
      it->addFactoriesTo(rotor_builder);
    }
    SuperType const* that = static_cast<SuperType const*>(this);
    that->addFactoriesTo(rotor_builder);
  }
}; // Builder<HeadEntityType, TailEntityTypes...>

template <> 
struct Builder<> {
  template <class RotorBuilderType>
  void addFactoriesTo(RotorBuilderType) const {
  }
}; // Builder<>

template <class HeadType, class ...TailTypes> 
struct TypeTraits<Builder<HeadType, TailTypes...>> {
  typedef Builder<HeadType, TailTypes...> Type;
  typedef Builder<TailTypes...> SuperType;
  typedef Scene<HeadType, TailTypes...> SceneType;
  typedef entity::Builder<Type, tl::TypeList<>> EmptyEntityBuilderType;
  typedef typename entity::Builder<Type, HeadType>::BaseType EntityBuilderType;
}; // TypeTraits<Builder<HeadType, TailTypes...>

} // namespace gyros

#endif // include guard

