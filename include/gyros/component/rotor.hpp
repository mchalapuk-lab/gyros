// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_HPP_
#define GYROS_COMPONENT_ROTOR_HPP_

#include <cstddef>

#include "gyros/component/rotor_builder.hpp"
#include "gyros/component/rotor_mutex.hpp"
#include "gyros/component/rotor_lock.hpp"
#include "gyros/component/rotor_state.hpp"
#include "gyros/component/iterator.hpp"
#include "gyros/util/type_literal.hpp"
#include "gyros/util/type_list/type_list.hpp"
#include "gyros/fwd/component/rotor.hpp"

namespace gyros {
namespace component {
namespace tl = util::type_list;

template <class ...ComponentTypes>
struct Rotor {
}; // struct Rotor<ComponentTypes...>

template <class ...ComponentTypes>
struct RotorBase {
}; // struct RotorBase<ComponentTypes...>

template <class T, class ...L>
struct Rotor<T, L...> : private RotorBase<T, L...> {
  struct PoolGetter;
  struct CapacityGetter;
  struct PositionResolver;
 public:
  typedef ReadOnlyRotorState<
      PoolGetter, CapacityGetter, PositionResolver, RotorLock
      > ReadOnlyState;
  typedef ReadWriteRotorState<
      PoolGetter, CapacityGetter, PositionResolver, RotorLock
      > ReadWriteState;
  typedef Rotor<T, L...> RotorType;
  typedef tl::TypeList<T, L...> TypeList;
  typedef RotorBuilder<T, L...> BuilderType;

  Rotor(Rotor<T, L...> &&rhs) noexcept
      : RotorBase<T, L...>(std::move(rhs)),
      get_pool_{ *this },
      get_capacity_{ *this },
      get_position_{ *this },
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
    return get_capacity_(util::TypeLiteral<ComponentType>());
  }
  template <class ComponentType>
  PositionIterator<ComponentType> const begin() const noexcept {
    auto pool = get_pool_(util::TypeLiteral<ComponentType>());
    return PositionIterator<ComponentType>(pool);
  }
  template <class ComponentType>
  PositionIterator<ComponentType> const end() const noexcept {
    return begin<ComponentType>() + capacity<ComponentType>();
  }

  ReadOnlyState const acquireReadOnly() {
    size_t read_offset, state_version;
    RotorLock lock = mutex_.acquireReadOnly(&read_offset, &state_version);
    return ReadOnlyState(get_pool_,
                         get_capacity_,
                         get_position_,
                         read_offset,
                         std::move(lock));
  }
  ReadWriteState acquireReadWrite() {
    size_t read_offset, state_version, write_offset;
    auto lock = mutex_.acquireReadWrite(&read_offset,
                                        &state_version,
                                        &write_offset);
    return ReadWriteState(get_pool_,
                          get_capacity_,
                          get_position_,
                          read_offset,
                          write_offset,
                          std::move(lock));
  }

 private:
  struct PoolGetter {
    template <class ComponentType>
    ComponentType *const
    operator() (util::TypeLiteral<ComponentType> type) const noexcept {
      return rotor_.poolHack(type);
    }
    RotorType const& rotor_;
  } get_pool_; // struct PoolGetter

  struct CapacityGetter {
    template <class ComponentType>
    size_t operator() (util::TypeLiteral<ComponentType> type) const noexcept {
      return rotor_.capacityHack(type);
    }
    RotorType const& rotor_;
  } get_capacity_; // struct CapacityGetter

  struct PositionResolver {
    template <class ComponentType>
    ptrdiff_t operator() (PositionIterator<ComponentType> it) const noexcept {
      return it - rotor_.begin<ComponentType>();
    }
    RotorType const& rotor_;
  } get_position_; // struct PositionResolver

  RotorMutex<3> mutex_;

  template <class ...ArgTypes>
  Rotor(ArgTypes ...pool_info) noexcept
    : RotorBase<T, L...>(std::forward<ArgTypes>(pool_info)...),
    get_pool_{ *this },
    get_capacity_{ *this },
    get_position_{ *this } {
  }

  friend struct PoolGetter;
  friend struct CapacityGetter;
  friend struct PositionResolver;
  friend struct RotorBuilder<T, L...>;
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
    delete [] reinterpret_cast<RawMemory<T> *>(pool_);
  }

 protected:
  template <class ...ArgTypes>
  RotorBase(RawMemory<T> *pool_start,
            size_t pool_capacity,
            ArgTypes &&...next_pool_info) noexcept
    : RotorBase<L...>(std::forward<ArgTypes>(next_pool_info)...),
    pool_(reinterpret_cast<T*>(pool_start)),
    capacity_(pool_capacity) {
  }

  using RotorBase<L...>::capacityHack;
  using RotorBase<L...>::poolHack;

  size_t capacityHack(util::TypeLiteral<T>) const noexcept {
    return capacity_;
  }
  T *const poolHack(util::TypeLiteral<T>) const noexcept {
    return pool_;
  }

 private:
  T *const pool_;
  size_t capacity_;
}; // class RotorBase<T, L...>

template <>
struct RotorBase<> {
  struct Unused {
  };
  size_t capacityHack(util::TypeLiteral<Unused>) const noexcept {
  }
  Unused *const poolHack(util::TypeLiteral<Unused>) const noexcept {
  }
}; // struct RotorBase<>

} // namespace component
} // namespace gyros

#endif // include guard

