// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_BUILDER_HPP_
#define GYROS_BUILDER_HPP_

#include "gyros/fwd/builder.hpp"

#include "gyros/component/rotor.hpp"
#include "gyros/entity/index.hpp"
#include "gyros/scene.hpp"

#include "gyros/detail/make_rotor_type.hpp"

namespace gyros {

template <class ...TupleTypes>
class Builder {
 public:
  typedef typename detail::MakeRotor<TupleTypes...>::Type RotorType;
  typedef entity::Index<TupleTypes...> IndexType;
}; // Builder<TupleTypes...>

} // namespace gyros

#endif // include guard

