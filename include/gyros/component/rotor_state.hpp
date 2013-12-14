// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_STATE_HPP_
#define GYROS_COMPONENT_ROTOR_STATE_HPP_

#include "gyros/component/iterator.hpp"
#include "gyros/util/type_literal.hpp"

#include <memory>

namespace gyros {
namespace component {

template <class PoolGetterType,
          class CapacityGetterType,
          class PositionResolverType,
          class LockType>
class ReadOnlyRotorState {
 public:
  template <class ComponentType>
  using Iterator = ReadingIterator<ComponentType>;

  ReadOnlyRotorState(PoolGetterType const& get_pool,
                     CapacityGetterType const& get_capacity,
                     PositionResolverType const& get_position,
                     size_t read_offset,
                     LockType &&lock)
      : shared_(new Data{ get_pool,
                          get_capacity,
                          get_position,
                          read_offset,
                          std::move(lock) }) {
  }
  
  template <class ComponentType>
  Iterator<ComponentType> const upgrade(PositionIterator<ComponentType> it) {
    util::TypeLiteral<ComponentType> type;

    return Iterator<ComponentType>(
        shared_->get_pool_(type) + shared_->get_position_(it),
        shared_->get_capacity_(type) * shared_->read_offset_
        );
  }
 private:
  struct Data {
    PoolGetterType const& get_pool_;
    CapacityGetterType const& get_capacity_;
    PositionResolverType const& get_position_;
    size_t read_offset_;
    LockType lock_;
  }; // struct Data

  std::shared_ptr<Data const> shared_;
}; // class ReadOnlyRotorState<ComponentTypes...>

template <class PoolGetterType,
          class CapacityGetterType,
          class PositionResolverType,
          class LockType>
class ReadWriteRotorState {
 public:
  template <class ComponentType>
  using Iterator = WritingIterator<ComponentType>;

  ReadWriteRotorState(PoolGetterType const& get_pool,
                      CapacityGetterType const& get_capacity,
                      PositionResolverType const& get_position,
                      size_t read_offset,
                      size_t write_offset,
                      LockType &&lock)
      : shared_(new Data{ get_pool,
                          get_capacity,
                          get_position,
                          read_offset,
                          write_offset,
                          std::move(lock) }) {
  }
  
  template <class ComponentType>
  Iterator<ComponentType> upgrade(PositionIterator<ComponentType> it) const {
    util::TypeLiteral<ComponentType> type;

    return Iterator<ComponentType>(
        shared_->get_pool_(type) + shared_->get_position_(it),
        shared_->get_capacity_(type) * shared_->read_offset_,
        shared_->get_capacity_(type) * shared_->write_offset_
        );
  }
 private:
  struct Data {
    PoolGetterType const& get_pool_;
    CapacityGetterType const& get_capacity_;
    PositionResolverType const& get_position_;
    size_t read_offset_;
    size_t write_offset_;
    LockType lock_;
  }; // struct Data

  std::shared_ptr<Data const> shared_;
}; // class ReadWriteRotorState<ComponentTypes...>

} // namespace component
} // namespace gyros

#endif // include guard

