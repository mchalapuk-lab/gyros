// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_DETAIL_BUILDER_BASE_HPP_
#define GYROS_ENTITY_DETAIL_BUILDER_BASE_HPP_

#include "gyros/type_traits.hpp"
#include "gyros/util/type_list/type_list.hpp"
#include "gyros/util/type_list/back.hpp"
#include "gyros/util/type_list/pop_back.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace entity {
namespace detail {

template <class TypeListType>
class BuilderBase {
}; // BuilderBase<TypeListType>

template <class ...ComponentTypes>
class BuilderBase<tl::TypeList<ComponentTypes...>>
  : private TypeTraits<BuilderBase<tl::TypeList<ComponentTypes...>>>::SuperType
{
 public:
  typedef BuilderBase<tl::TypeList<ComponentTypes...>> Type;
  typedef TypeTraits<Type> Traits;
  typedef typename Traits::SuperType SuperType;
  typedef typename Traits::LastComponentType LastComponentType;

  template <class FactoryType>
  BuilderBase(SuperType &&previousStep, FactoryType factory)
    : SuperType(std::move(previousStep)),
      factory_(std::move(factory)) {
  }
  BuilderBase(BuilderBase &&rhs)
    : SuperType(std::move(rhs)),
      factory_(std::move(rhs.factory_)) {
  }

 private:
  std::function<LastComponentType *(void *)> factory_;  
}; // struct BuilderBase<TypeList<ComponentTypes...>>

template <>
struct BuilderBase<tl::TypeList<>> {
}; // struct BuilderBase<TypeList<>>

} // namespace detail
} // namespace entity

template <class ...ComponentTypes>
struct TypeTraits<entity::detail::BuilderBase<tl::TypeList<ComponentTypes...>>>
{
  typedef tl::TypeList<ComponentTypes...> ComponentTypeList;
  typedef typename tl::Back<ComponentTypeList>::Type LastComponentType;
  typedef typename tl::PopBack<ComponentTypeList>::Type SuperComponentTypeList;
  typedef entity::detail::BuilderBase<ComponentTypeList> Type;
  typedef entity::detail::BuilderBase<SuperComponentTypeList> SuperType;
}; // TypeTraits<entity::detail::BuilderBase<ComponentTypes...>>

} // namespace gyros

#endif // include guard

