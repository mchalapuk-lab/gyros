// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_POSITION_ITERATOR_HPP_
#define GYROS_COMPONENT_POSITION_ITERATOR_HPP_

#include <cstddef>

#include "gyros/fwd/component/rotor.hpp"

namespace gyros {
namespace component {

template <class ComponentType>
class PositionIterator {
 public:
  typedef PositionIterator<ComponentType> IteratorType;

  explicit constexpr PositionIterator(ComponentType const* ptr)
    : ptr_(ptr) {
  }

  bool operator== (IteratorType const& rhs) const {
    return ptr_ == rhs.ptr_;
  }
  bool operator!= (IteratorType const& rhs) const {
    return ptr_ != rhs.ptr_;
  }

  ptrdiff_t operator- (IteratorType const& rhs) const {
    return ptr_ - rhs.ptr_;
  }

  IteratorType operator+ (ptrdiff_t diff) const {
    return IteratorType(ptr_ + diff);
  }
  IteratorType operator- (ptrdiff_t diff) const {
    return IteratorType(ptr_ - diff);
  }

  IteratorType operator++ (int) {
    return IteratorType(ptr_++);
  }
  IteratorType operator-- (int) {
    return IteratorType(ptr_--);
  }

  IteratorType& operator+= (ptrdiff_t diff) {
    ptr_ += diff;
    return *this;
  }
  IteratorType& operator-= (ptrdiff_t diff) {
    ptr_ -= diff;
    return *this;
  }

  IteratorType& operator++ () {
    return *this += 1;
  }
  IteratorType& operator-- () {
    return *this -= 1;
  }

 private:
  ComponentType const* ptr_;

  template <class ...ComponentTypes>
  friend class Rotor;
}; // class PositionIterator

} // namespace component
} // namespace gyros

#endif // include guard

