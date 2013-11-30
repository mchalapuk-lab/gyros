// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_POSITION_ITERATOR_HPP_
#define GYROS_COMPONENT_POSITION_ITERATOR_HPP_

#include <cstddef>

#include "gyros/fwd/component/iterator.hpp"

namespace gyros {
namespace component {

template <class IteratorType>
class IteratorFacade {
 public:
  bool operator== (IteratorType const& rhs) const {
    return derived_this().equals(rhs);
  }
  bool operator!= (IteratorType const& rhs) const {
    return !(*this == rhs);
  }

  ptrdiff_t operator- (IteratorType const& rhs) const {
    return derived_this().difference(rhs);
  }

  IteratorType operator+ (ptrdiff_t diff) const {
    IteratorType copy = derived_this();
    copy.increment(diff);
    return copy;
  }
  IteratorType operator- (ptrdiff_t diff) const {
    return *this + (-diff);
  }

  IteratorType operator++ (int) {
    IteratorType copy = derived_this();
    derived_this().increment(1);
    return copy;
  }
  IteratorType operator-- (int) {
    IteratorType copy = derived_this();
    derived_this().increment(-1);
    return copy;
  }

  IteratorType& operator+= (ptrdiff_t diff) {
    derived_this().increment(diff);
    return derived_this();
  }
  IteratorType& operator-= (ptrdiff_t diff) {
    return *this += -diff;
  }

  IteratorType& operator++ () {
    return *this += 1;
  }
  IteratorType& operator-- () {
    return *this -= 1;
  }

 private:
  IteratorType& derived_this() {
    return *static_cast<IteratorType *>(this);
  }
  IteratorType const& derived_this() const {
    return *static_cast<IteratorType const*>(this);
  }
}; // class IteratorFacade

template <class ComponentType>
class PositionIterator : public IteratorFacade<PositionIterator<ComponentType>> {
 public:
  typedef PositionIterator<ComponentType> IteratorType;

  explicit constexpr PositionIterator(ComponentType const* ptr)
    : ptr_(ptr) {
  }

  void increment(ptrdiff_t diff) {
    ptr_ += diff;
  }
  ptrdiff_t difference(IteratorType const& rhs) const {
    return ptr_ - rhs.ptr_;
  }
  bool equals(PositionIterator<ComponentType> const& rhs) const {
    return ptr_ == rhs.ptr_;
  }
 private:
  ComponentType const* ptr_;
}; // class PositionIterator

template <class ComponentType, class LockType>
class ReadingIterator :
    public IteratorFacade<ReadingIterator<ComponentType, LockType>> {
 public:
  typedef ReadingIterator<ComponentType, LockType> IteratorType;

  ReadingIterator(ComponentType const* ptr, LockType lock)
      : ptr_(ptr), lock_(lock) {
  }

  ComponentType const& operator* () const {
    return *(ptr_ + lock_.read_offset());
  }

  void increment(ptrdiff_t diff) {
    ptr_ += diff;
  }
  ptrdiff_t difference(IteratorType const& rhs) const {
    return ptr_ - rhs.ptr_;
  }
  bool equals(IteratorType const& rhs) const {
    return ptr_ == rhs.ptr_;
  }
 private:
  ComponentType const* ptr_;
  LockType lock_;
}; // class ReadingIterator

template <class ComponentType, class LockType>
class WritingIterator :
    public IteratorFacade<WritingIterator<ComponentType, LockType>> {
 public:
  typedef WritingIterator<ComponentType, LockType> IteratorType;

  WritingIterator(ComponentType const* ptr, LockType lock)
      : ptr_(ptr), lock_(lock) {
  }

  ComponentType const& operator* () const {
    return *(ptr_ + lock_.read_offset());
  }
  template <class WritingVisitorType>
  void visit(WritingVisitorType visit) {
    return visit(**this, *(ptr_ + lock_.write_offset()));
  }

  void increment(ptrdiff_t diff) {
    ptr_ += diff;
  }
  ptrdiff_t difference(IteratorType const& rhs) const {
    return ptr_ - rhs.ptr_;
  }
  bool equals(IteratorType const& rhs) const {
    return ptr_ == rhs.ptr_;
  }
 private:
  ComponentType const* ptr_;
  LockType lock_;
}; // class ReadingIterator

} // namespace component
} // namespace gyros

#endif // include guard

