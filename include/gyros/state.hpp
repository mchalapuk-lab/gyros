// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_STATE_HPP_
#define GYROS_STATE_HPP_

#include <tuple>

#include "gyros/detail/findsubtype.hpp"
#include "gyros/fwd/state.hpp"

namespace gyros {

template <class SceneType, class ...ComponentTypes>
class State {
}; // class State

// specialization for one component
template <class ComponentType, class ...L>
class State<Scene<L...>, ComponentType> {
 public:
  typedef typename detail::FindSubtype<Scene, ComponentType, L...>::type
      SceneType;

  State(SceneType &scene) :
      scene_(scene) {
  }

  ComponentType* begin() {
    return reinterpret_cast<ComponentType *>(scene_.pool_);
  }
  ComponentType* end() {
    return begin() + scene_.capacity_;
  }
  ComponentType const* begin() const {
    return reinterpret_cast<ComponentType *>(scene_.pool_);
  }
  ComponentType const* end() const {
    return begin() + scene_.capacity_;
  }
 private:
  SceneType &scene_;
}; // class State<SceneType, ComponentType>

// in case of multiple components, we operate on tuples
template <class ...ComponentTypes, class ...L>
class State<Scene<L...>, ComponentTypes...> {
 public:
  typedef StateIterator<ComponentTypes...> IteratorType;
  typedef Scene<L...> SceneType;

  State(SceneType &scene) :
      scene_(scene) {
  }

  IteratorType begin();
  IteratorType end();
  IteratorType const begin() const;
  IteratorType const end() const;
 private:
  SceneType &scene_;
}; // class State<SceneType, ComponentTypes...>

template <class ...ComponentTypes>
class StateIterator {
 public:
  typedef StateIterator<ComponentTypes...> Type;
  typedef std::tuple<ComponentTypes &...> TupleType;

  StateIterator(ComponentTypes *...ptrs) :
      tuple_(*ptrs...),
      pointers_(ptrs...) {
  }

  bool operator==(Type const& rhs) const {
    return std::get<0>(pointers_) == std::get<0>(rhs.pointers_);
  }
  bool operator!=(Type const& rhs) const {
    return !operator==(rhs);
  }
  Type operator++ () {
    return *this; // TODO
  }
  TupleType& operator* () {
    return tuple_;
  }
 private:
  TupleType tuple_;
  std::tuple<ComponentTypes *...> pointers_;
}; // class StateIterator

} // namespace gyros

#endif // include guard

