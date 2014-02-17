// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include <type_traits>

#include "gyros/type_traits.hpp"
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
class Builder : private TypeTraits<Builder<EntityTypes...>>::SuperType {
 public:
  typedef TypeTraits<Builder<EntityTypes...>> Traits;
  typedef typename Traits::Type Type;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::HeadType HeadEntityType;
  typedef typename Traits::SceneType SceneType;
  typedef typename Traits::EmptyEntityBuilderType EmptyEntityBuilderType;
  typedef typename Traits::EntityBuilderType EntityBuilderType;
  typedef typename Traits::IteratorType IteratorType;
  typedef typename Traits::RotorBuilderType RotorBuilderType;
  typedef typename Traits::IndexBuilderType FinalIndexBuilderType;

  EmptyEntityBuilderType newEntity() {
    return EmptyEntityBuilderType(*this);
  }
  SceneType build() {
    RotorBuilderType rotor_builder;

    RecursiveForwardType forward {
      FactoriesAdder { *this, rotor_builder },
      BuildFinisher()
    };
    auto index_builder = forward(entity::IndexBuilder<>());

    auto rotor = rotor_builder.build();
    auto index = index_builder.build(rotor);
    return SceneType(std::move(rotor), std::move(index));
  }

  using SuperType::addEntity;
  Builder& addEntity(EntityBuilderType &&entity_builder) {
    entity_builders_.emplace_back(std::move(entity_builder));
  }

  IteratorType begin() const {
    return entity_builders_.begin();
  }
  IteratorType end() const {
    return entity_builders_.end();
  }

 private:
  template <class Type>
  using GetSuperType = GetAncestor<Type, 1>;

  struct BuildFinisher {
    FinalIndexBuilderType
    operator() (FinalIndexBuilderType &&ret_val) const noexcept {
      return ret_val;
    }
  }; // BuildFinisher

  struct FactoriesAdder {
    template <class CurrentType, class ForwarderType, class IndexBuilderType>
    FinalIndexBuilderType operator() (
        ForwarderType &&forward,
        IndexBuilderType &&index_builder
        ) const noexcept {

      auto begin = builder_.CurrentType::begin();
      auto end = builder_.CurrentType::end();
      for (auto it = begin; it != end; ++it) {
        it->addFactoriesTo(rotor_builder_);
      }

      typedef typename CurrentType::HeadEntityType EntityType;
      return forward(
          index_builder.template setEntityCount<EntityType>(end - begin)
          );
    }

    Type &builder_;
    RotorBuilderType &rotor_builder_;
  }; // FactoriesAdder

  typedef RecursiveForward<
      Type,
      Builder<>,
      GetSuperType,
      FactoriesAdder const,
      BuildFinisher const,
      FinalIndexBuilderType> RecursiveForwardType;

  std::vector<EntityBuilderType> entity_builders_;
}; // Builder<HeadEntityType, TailEntityTypes...>

template <> 
struct Builder<> {
 protected:
  void addEntity() {
  }
}; // Builder<>

template <class ...EntityTypes> 
struct TypeTraits<Builder<EntityTypes...>> {
  typedef Builder<EntityTypes...> Type;
  typedef tl::TypeList<EntityTypes...> TypeList;
  typedef typename tl::PopFront<TypeList>::Type SuperTypeList;
  typedef typename tl::Cast<Builder, SuperTypeList>::Type SuperType;
  typedef Scene<EntityTypes...> SceneType;
  typedef entity::Builder<Type, tl::TypeList<>> EmptyEntityBuilderType;
  typedef typename tl::Front<TypeList>::Type HeadType;
  typedef typename entity::Builder<Type, HeadType>::BaseType EntityBuilderType;
  typedef typename std::vector<EntityBuilderType>::const_iterator IteratorType;
  typedef typename SceneType::RotorType RotorType;
  typedef typename RotorType::BuilderType RotorBuilderType;
  typedef typename SceneType::IndexType IndexType;
  typedef typename entity::IndexBuilder<EntityTypes...> IndexBuilderType;
}; // TypeTraits<Builder<EntityTypes...>

} // namespace gyros

#endif // include guard

