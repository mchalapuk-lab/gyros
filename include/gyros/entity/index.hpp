// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_ENTITY_INDEX_HPP_
#define GYROS_ENTITY_INDEX_HPP_

#include <tuple>

namespace gyros {
namespace entity {

namespace detail {

template <template <class ...> class TypeListType, class ...TypeLists>
struct CatTypeList {
}; // struct CatTypeList<TypeListType, TypeLists...>

template <template <class ...> class TypeListType,
          class ...Types1,
          class ...Types2,
          class ...TypeListTail>
struct CatTypeList<TypeListType, TypeListType<Types1...>, TypeListType<Types2...>, TypeListTail...> {
  typedef typename CatTypeList<
      TypeListType,
      TypeListType<Types1..., Types2...>,
      TypeListTail...
          >::Types Type;
}; // struct CatTypeList<TypeListType, TypeListType<Types1...>, TypesListTypes<Types2...>, TypeListTail...>

template <template <class ...> class TypeListType, class ...Types>
struct CatTypeList<TypeListType, TypeListType<Types...>> {
  typedef typename TypeListType<Types...> Type;
};

template <template <class ...> class TypeListType, class ...Types>
struct MakePowerSet {
}; // struct MakePowerSet<TypeListType, Types...>

template <template <class ...> class TypeListType,
          class HeadType,
          class ...TailTypes>
struct MakePowerSet<TypeListType, HeadType, TailTypes...> {
  typedef typename CatTypeList<
      std::tuple,
      std::tuple<TypeListType<HeadType>>,
      typename MakePowerSet<TypeListType, TailTypes...>::Types
      std::tuple<TypeListType<HeadType, TailTypes...>>
          >::Types Types;
}; // struct MakePowerSet<TypeListType, HeadType, TailTypes...>

template <template <class ...> class TypeListType>
struct MakePowerSet<TypeListType> {
  std::tuple<TypeListType<>> Types;
}; // struct MakePowerSet<TypeList>

} // namespace detail

template <class ...ComponentTypes>
struct Index {

}; // struct Index<ComponentTypes...>

} // namespace entity
} // namespace gyros

#endif // include guard

