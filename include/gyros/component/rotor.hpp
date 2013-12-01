// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_HPP_
#define GYROS_COMPONENT_ROTOR_HPP_

#include <cstddef>

#include "gyros/component/rotor_builder.hpp"
#include "gyros/component/rotor_state.hpp"
#include "gyros/component/iterator.hpp"

namespace gyros {
namespace component {

template <class ...ComponentTypes>
struct Rotor {
}; // struct Rotor

template <class T, class ...L>
class Rotor<T, L...> : private Rotor<L...> {
 public:
  typedef std::shared_ptr<std::nullptr_t> Unlocker;
  typedef ReadingIterator<T, Unlocker> ReadOnlyIterator;
  typedef WritingIterator<T, Unlocker> ReadWriteIterator;

  Rotor(Rotor<T, L...> &&rhs) noexcept
    : Rotor<L...>(std::move(rhs)),
    pool_(rhs.pool_),
    capacity_(rhs.capacity_) {
  }
  ~Rotor() {
    if (!capacity_) {
      return; // in case if moved
    }
    for (auto it = pool_, end = it + capacity_; it != end; ++it) {
      it->~T();
    }
    delete [] reinterpret_cast<detail::RawMemory<T> *>(pool_);
  }

  PositionIterator<T> begin() const noexcept {
    return begin<T>();
  }
  PositionIterator<T> const end() const noexcept {
    return end<T>();
  }

  template <class ComponentType>
  PositionIterator<ComponentType> begin() const noexcept {
    return begin(static_cast<ComponentType *>(nullptr));
  }
  template <class ComponentType>
  PositionIterator<ComponentType> const end() const noexcept {
    return end(static_cast<ComponentType *>(nullptr));
  }

  ReadOnlyIterator const upgradeReadOnly(PositionIterator<T> it) const {
    ptrdiff_t index = it - begin();
    return ReadOnlyIterator(pool_ + index, unlocker([] () {}));
  }
  ReadWriteIterator upgradeReadWrite(PositionIterator<T> it) const {
    ptrdiff_t index = it - begin();
    return ReadWriteIterator(pool_ + index, 0, unlocker([] () {}));
  }

 protected:
  template <class ...ArgTypes>
  Rotor(detail::RawMemory<T> *pool_start,
        size_t pool_capacity,
        ArgTypes &&...next_pool_info)
    : Rotor<L...>(next_pool_info...),
    pool_(reinterpret_cast<T*>(pool_start)),
    capacity_(pool_capacity) {
  }

  PositionIterator<T> begin(T*) const {
    return PositionIterator<T>(pool_);
  }
  PositionIterator<T> const end(T*) const {
    return PositionIterator<T>(pool_) + capacity_;
  }

  template <class Functor>
  Unlocker unlocker(Functor func) const {
    return Unlocker(nullptr, [func] (std::nullptr_t) { func(); });
  }
 private:
  T *const pool_;
  size_t const capacity_;

  friend class detail::RotorCreator<Rotor<T, L...>>;
}; // class Rotor<T, L...>

} // namespace component
} // namespace gyros

#endif // include guard

