// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/util/type_list/type_list.hpp"

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/entity/builder.hpp"
#include "gyros/scene.hpp"

#include "gyros/detail/make_rotor_type.hpp"
#include "gyros/fwd/builder.hpp"

namespace gyros {
namespace tl = util::type_list;

template <class ...EntityTypes>
class Builder {
 public:
  typedef typename detail::MakeRotor<EntityTypes...>::Type RotorType;
  typedef entity::Index<EntityTypes...> IndexType;
  typedef Builder<EntityTypes...> Type;

  entity::Builder<Type, tl::TypeList<>> newEntity() { 
    return entity::Builder<Type, tl::TypeList<>>(*this);
  }
  template <class EntityType>
  Builder& addEntity(entity::Builder<Type, EntityType> &&entityBuilder) {
    static_assert(
        tl::Contains<tl::TypeList<EntityTypes...>, EntityType>::value,
        "type of built entity doesn't match entity types of this builder"
        );
  }
}; // Builder<EntityTypes...>

} // namespace gyros

#endif // include guard

