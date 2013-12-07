// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_MUTEX_HPP_
#define GYROS_COMPONENT_ROTOR_MUTEX_HPP_

#include "gyros/component/rotor_lock.hpp"

#include <mutex>
#include <vector>
#include <deque>
#include <stdexcept>

namespace gyros {
namespace component {

/**
 * State locking and unlocking operations are constant time.
 */
template <size_t n_states__ = 3>
class RotorMutex {
  static_assert(n_states__ >= 3, "state count must be at least 3");
 public:
  RotorMutex()
      : states_(n_states__) {

    std::fill(states_.begin(), states_.end(), StateInfo{0, 0});
    for (auto it = states_.begin(),end = states_.end(); it != end; ++it) {
      free_.push_back(it);
      it->state_version_ = it - states_.begin();
    }
    most_fresh_ = free_.front();
  }

  RotorLock acquireReadOnly(size_t *read_index,
                            size_t *state_version);
  RotorLock acquireReadWrite(size_t *read_index, 
                             size_t *state_version,
                             size_t *write_index);
 private:
  struct StateInfo {
    size_t state_version_;
    size_t reader_count_;
  }; // struct StateInfo

  typedef typename std::vector<StateInfo>::iterator StateIterator;

  std::mutex mutex_;
  std::vector<StateInfo> states_;
  std::deque<StateIterator> free_;
  StateIterator most_fresh_;

  StateIterator mostFreshReadOnly() {
    if (most_fresh_ == free_.front()) {
      free_.pop_front();
    }
    most_fresh_->reader_count_ += 1;
    return most_fresh_;
  }
  StateIterator leastFreshReadWrite() {
    if (free_.size() == 0) {
      throw std::logic_error("no free states");
    }
    auto state = free_.back();
    free_.pop_back();
    return state;
  }

  void freeAfterRead(StateIterator state) {
    if (--state->reader_count_ == 0) {
      if (state == most_fresh_) {
        free_.push_front(state);
      } else {
        // not most fresh so can be downgraded
        free_.push_back(state);
      }
    }
  }
  void freeAfterWrite(StateIterator state) {
    state->state_version_ = most_fresh_->state_version_ + 1;
    free_.push_front(most_fresh_ = state);
  }
}; // class RotorMutex<n_states__>

template <size_t n_states__>
RotorLock RotorMutex<n_states__>::acquireReadOnly(size_t *read_index,
                                                  size_t *state_version) {
  std::unique_lock<std::mutex> lock(mutex_);
  StateIterator read_state = mostFreshReadOnly();
  lock.unlock();

  *read_index = read_state - states_.begin();
  *state_version = read_state->state_version_;

  return RotorLock([this, read_state] () {
                     std::lock_guard<std::mutex> lock(mutex_);
                     freeAfterRead(read_state);
                   });
}

template <size_t n_states__>
RotorLock RotorMutex<n_states__>::acquireReadWrite(size_t *read_index, 
                                                   size_t *state_version,
                                                   size_t *write_index) {
  std::unique_lock<std::mutex> lock(mutex_);
  StateIterator read_state = mostFreshReadOnly();
  StateIterator write_state = leastFreshReadWrite();
  lock.unlock();

  *read_index = read_state - states_.begin();
  *state_version = read_state->state_version_;
  *write_index = write_state - states_.begin();

  return RotorLock([this, read_state, write_state] () {
                     std::lock_guard<std::mutex> lock(mutex_);
                     freeAfterRead(read_state);
                     freeAfterWrite(write_state);
                   });
}

} // namespace component
} // namespace gyros

#endif // include guard

