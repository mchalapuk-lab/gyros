// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/util/type_list.hpp"
#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/entity/index_builder.hpp"
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
  typedef typename RotorType::BuilderType RotorBuilderType;
  typedef typename SceneType::IndexType IndexType;

  EmptyEntityBuilderType newEntity() {
    return EmptyEntityBuilderType(*this);
  }
  SceneType build() {
    RotorBuilderType rotor_builder;
    auto index_builder = addFactoriesTo(rotor_builder,
                                        entity::IndexBuilder<>());
    RotorType rotor = rotor_builder.build();
    IndexType index = index_builder.build(rotor);
    return SceneType(std::move(rotor), std::move(index));
  }

  Builder& addEntity(EntityBuilderType &&entity_builder) {
    entity_builders_.emplace_back(std::move(entity_builder));
  }

 protected:
  template <class IndexBuilderType>
  entity::IndexBuilder<HeadEntityType, TailEntityTypes...>
  addFactoriesTo(RotorBuilderType &rotor_builder,
                 IndexBuilderType index_builder) const {
    auto begin = entity_builders_.begin(), end = entity_builders_.end();
    for (auto it = begin; it != end; ++it) {
      it->addFactoriesTo(rotor_builder);
    }
    SuperType const* that = static_cast<SuperType const*>(this);
    return that->addFactoriesTo(
        rotor_builder,
        index_builder.template setEntityCount<HeadEntityType>(end - begin)
        );
  }
 private:
  std::vector<EntityBuilderType> entity_builders_;
}; // Builder<HeadEntityType, TailEntityTypes...>

template <> 
struct Builder<> {
  template <class RotorBuilderType, class IndexBuilderType>
  IndexBuilderType const& addFactoriesTo(RotorBuilderType,
                                   IndexBuilderType const& builder) const {
    return builder;
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

