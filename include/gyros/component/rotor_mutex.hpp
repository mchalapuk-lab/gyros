// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_MUTEX_HPP_
#define GYROS_COMPONENT_ROTOR_MUTEX_HPP_

#include <mutex>
#include <functional>
#include <vector>
#include <queue>
#include <stdexcept>

namespace gyros {
namespace component {

class Lock {
 public:
  template <class Deleter>
  Lock(Deleter deleter)
      : deleter_(deleter) {
  }
  Lock(Lock const&) = delete;
  Lock(Lock &&rhs)
      : deleter_(rhs.deleter_) {
  }
  ~Lock() {
    deleter_();
  }
 private:
  std::function<void ()> deleter_;
}; // class Lock

template <size_t n_states__ = 3>
class RotorMutex {
  static_assert(n_states__ >= 3, "state count must be at least 3");
 public:
  RotorMutex()
      : states_(n_states__) {

    std::fill(states_.begin(), states_.end(), StateInfo{0, 0});
    for (auto it = states_.begin(),end = states_.end(); it != end; ++it) {
      free_.push(it);
      it->state_version_ = it - states_.begin();
    }
  }

  Lock acquireReadOnly(size_t *read_index,
                       size_t *state_version);
  Lock acquireReadWrite(size_t *read_index, 
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
  std::queue<StateIterator> free_;

  StateIterator mostFreshReadOnly() {
    if (free_.size() == 0) {
      throw std::logic_error("no free states for writing");
    }
    auto state = free_.back();
    state->reader_count_ += 1;
    return state;
  }
  StateIterator leastFreshReadWrite() {
    if (free_.size() == 0) {
      throw std::logic_error("no free states for writing");
    }
    auto state = free_.front();
    if (state->reader_count_ != 0) {
      throw std::logic_error("no free states for writing");
    }
    free_.pop();
    return state;
  }

  void freeAfterRead(StateIterator state) {
    state->reader_count_ -= 1;
  }
  void freeAfterWrite(StateIterator state) {
    state->state_version_ = free_.back()->state_version_ + 1;
    free_.push(state);
  }
}; // class RotorMutex<n_states__>

template <size_t n_states__>
Lock RotorMutex<n_states__>::acquireReadOnly(size_t *read_index,
                                             size_t *state_version) {
  std::unique_lock<std::mutex> lock(mutex_);
  StateIterator read_state = mostFreshReadOnly();
  lock.unlock();

  *read_index = read_state - states_.begin();
  *state_version = read_state->state_version_;

  return Lock([this, read_state] () {
                std::lock_guard<std::mutex> lock(mutex_);
                freeAfterRead(read_state);
              });
}

template <size_t n_states__>
Lock RotorMutex<n_states__>::acquireReadWrite(size_t *read_index, 
                                              size_t *state_version,
                                              size_t *write_index) {
  std::unique_lock<std::mutex> lock(mutex_);
  StateIterator read_state = mostFreshReadOnly();
  StateIterator write_state = leastFreshReadWrite();
  lock.unlock();

  *read_index = read_state - states_.begin();
  *state_version = read_state->state_version_;
  *write_index = write_state - states_.begin();

  return Lock([this, read_state, write_state] () {
                std::lock_guard<std::mutex> lock(mutex_);
                freeAfterRead(read_state);
                freeAfterWrite(write_state);
              });
}

} // namespace component
} // namespace gyros

#endif // include guard

