// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_HPP_
#define GYROS_COMPONENT_ROTOR_HPP_

#include <cstddef>

#include "gyros/component/rotor_builder.hpp"
#include "gyros/component/rotor_mutex.hpp"
#include "gyros/component/rotor_lock.hpp"
#include "gyros/component/iterator.hpp"

namespace gyros {
namespace component {

template <class ...ComponentTypes>
struct Rotor {
}; // struct Rotor<ComponentTypes...>

template <class ...ComponentTypes>
struct RotorBase {
}; // struct RotorBase<ComponentTypes...>

template <class T, class ...L>
struct Rotor<T, L...> : private RotorBase<T, L...> {
 public:
  template <class ComponentType>
  using ReadOnlyIterator = ReadingIterator<ComponentType, RotorLock>;
  template <class ComponentType>
  using ReadWriteIterator = WritingIterator<ComponentType, RotorLock>;

  Rotor(Rotor<T, L...> &&rhs) noexcept
      : RotorBase<T, L...>(std::move(rhs)),
      mutex_(std::move(rhs.mutex_)) {
  }
  
  size_t capacity() const noexcept {
    return capacity<T>();
  }
  PositionIterator<T> begin() const noexcept {
    return begin<T>();
  }
  PositionIterator<T> const end() const noexcept {
    return end<T>();
  }

  template <class ComponentType>
  size_t capacity() const noexcept {
    return this->capacityHack(static_cast<ComponentType *>(nullptr));
  }
  template <class ComponentType>
  PositionIterator<ComponentType> const begin() const noexcept {
    return PositionIterator<ComponentType>(pool<ComponentType>());
  }
  template <class ComponentType>
  PositionIterator<ComponentType> const end() const noexcept {
    return begin<ComponentType>() + capacity<ComponentType>();
  }

  template <class ComponentType>
  ReadOnlyIterator<ComponentType> const
  upgradeReadOnly(PositionIterator<ComponentType> it) {
    size_t read_offset, state_version;
    RotorLock lock = mutex_.acquireReadOnly(&read_offset, &state_version);

    return ReadOnlyIterator<ComponentType>(
        pool<ComponentType>() + (it - begin()),
        capacity<ComponentType>() * read_offset,
        std::move(lock)
        );
  }
  template <class ComponentType>
  ReadWriteIterator<ComponentType>
  upgradeReadWrite(PositionIterator<ComponentType> it) {
    size_t read_offset, state_version, write_offset;
    auto lock = mutex_.acquireReadWrite(&read_offset,
                                        &state_version,
                                        &write_offset);
    return ReadWriteIterator<ComponentType>(
        pool<ComponentType>() + (it - begin()),
        capacity<ComponentType>() * read_offset,
        capacity<ComponentType>() * write_offset,
        std::move(lock)
        );
  }

 private:
  RotorMutex<detail::N_COPIES> mutex_;

  template <class ...ArgTypes>
  Rotor(ArgTypes ...pool_info) noexcept
    : RotorBase<T, L...>(std::forward<ArgTypes>(pool_info)...) {
  }
  template <class ComponentType>
  ComponentType *const pool() const noexcept {
    return this->poolHack(static_cast<ComponentType *>(nullptr));
  }

  friend class detail::RotorCreator<Rotor<T, L...>>;
}; // struct Rotor<T, L...>

template <class T, class ...L>
class RotorBase<T, L...> : public RotorBase<L...> {
 public:
  RotorBase(RotorBase<T, L...> &&rhs) noexcept
      : RotorBase<L...>(std::move(rhs)),
      pool_(rhs.pool_),
      capacity_(rhs.capacity_) {

    rhs.capacity_ = 0;
  }
  ~RotorBase() {
    if (!capacity_) {
      return; // in case if moved
    }
    for (auto it = pool_, end = it + capacity_; it != end; ++it) {
      it->~T();
    }
    delete [] reinterpret_cast<detail::RawMemory<T> *>(pool_);
  }

 protected:
  template <class ...ArgTypes>
  RotorBase(detail::RawMemory<T> *pool_start,
            size_t pool_capacity,
            ArgTypes ...next_pool_info) noexcept
    : RotorBase<L...>(std::forward<ArgTypes>(next_pool_info)...),
    pool_(reinterpret_cast<T*>(pool_start)),
    capacity_(pool_capacity) {
  }

  size_t capacityHack(T *) const noexcept {
    return capacity_;
  }
  T *const poolHack(T *) const noexcept {
    return pool_;
  }

 private:
  T *const pool_;
  size_t capacity_;
}; // class RotorBase<T, L...>

} // namespace component
} // namespace gyros

#endif // include guard

