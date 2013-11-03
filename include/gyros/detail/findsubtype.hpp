// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_DETAIL_FINDSUBTYPE_HPP_
#define GYROS_DETAIL_FINDSUBTYPE_HPP_

namespace gyros {
namespace detail {

template <template <class ...> class Template,
          class TargetType,
          class ...TypeList>
struct FindSubtype {
  typedef void type;
}; // struct FindSubtype

template <template <class ...> class Template,
          class TargetType,
          class ...TypeList>
struct FindSubtype<Template, TargetType, TargetType, TypeList...> {
  typedef Template<TargetType, TypeList...> type;
}; // struct FindSubtype<Template, TargetType, TargetType, TypeList...>

template <template <class ...> class Template,
          class TargetType,
          class HeadType,
          class ...TailTypes>
struct FindSubtype<Template, TargetType, HeadType, TailTypes...> {
  typedef typename FindSubtype<Template, TargetType, TailTypes...>::type type;
  static_assert(!std::is_same<void, type>::value,
                "target type not contained in type list");
}; // struct FindSubtype<Template, TargetType, HeadType, TailTypes...>

} // namespace detail
} // namespace gyros

#endif // include guard

