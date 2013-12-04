// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_MUTEX_HPP_
#define GYROS_COMPONENT_ROTOR_MUTEX_HPP_

#include <mutex>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>

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

struct DereferencingComparator {
  template <class IteratorType>
  bool operator() (IteratorType lhs, IteratorType rhs) const {
    return *lhs - *rhs;
  }
}; // struct DereferencingComparator

template <size_t n_states__ = 3>
class RotorMutex {
 public:
  RotorMutex() : state_versions_(n_states__), last_version_(0) {
    free_states_.reserve(n_states__);

    std::fill(state_versions_.begin(), state_versions_.end(), 0);
    for (auto it = state_versions_.begin(),
         end = state_versions_.end(); it != end; ++it) {
      pushHeap(free_states_, it);
      reader_count_.insert(std::make_pair(it, 0));
    }
  }

  Lock acquireReadOnly(size_t *read_index, size_t *state_version) {
    std::unique_lock<std::mutex> lock(mutex_);
    StatePointer read_state = mostFreshReadOnly();
    lock.unlock();

    *read_index = read_state - state_versions_.begin();
    *state_version = *read_state;

    return Lock([this, read_state] () {
                  std::lock_guard<std::mutex> lock(mutex_);
                  freeAfterRead(read_state);
                });
  }
  Lock acquireReadWrite(size_t *read_index, 
                        size_t *state_version,
                        size_t *write_index) {
    std::unique_lock<std::mutex> lock(mutex_);
    StatePointer write_state = leastFreshReadWrite();
    StatePointer read_state = mostFreshReadOnly();
    lock.unlock();

    *read_index = read_state - state_versions_.begin();
    *state_version = *read_state;
    *write_index = write_state - state_versions_.begin();
  
    return Lock([this, read_state, write_state] () {
                  std::lock_guard<std::mutex> lock(mutex_);
                  freeAfterRead(read_state);
                  freeAfterWrite(write_state);
                });
  }
 private:
  typedef std::vector<size_t>::iterator StatePointer;

  std::mutex mutex_;
  std::vector<size_t> state_versions_;
  std::vector<StatePointer> free_states_;
  std::map<StatePointer, size_t> reader_count_;
  size_t last_version_;

  StatePointer mostFreshReadOnly() {
    auto state = peekHeap(free_states_);
    // TODO check if one of already read states is more up to date
    popHeap(free_states_);
    reader_count_.at(state) += 1;
    return state;
  }
  StatePointer leastFreshReadWrite() {
    auto state = peekHeap(free_states_);
    popHeap(free_states_);
    return state;
  }

  void freeAfterRead(StatePointer state) {
    if (--reader_count_.at(state) == 0) {
      pushHeap(free_states_, state);
    }
  }
  void freeAfterWrite(StatePointer state) {
    *state = ++last_version_;
    pushHeap(free_states_, state);
  }

  static void pushHeap(std::vector<StatePointer> heap, StatePointer state) {
    heap.push_back(state);
    std::push_heap(heap.begin(), heap.end(), DereferencingComparator());
  }
  static StatePointer peekHeap(std::vector<StatePointer> heap) {
    return heap.front();
  }
  static void popHeap(std::vector<StatePointer> heap) {
    std::pop_heap(heap.begin(), heap.end(), DereferencingComparator());
    heap.pop_back();
  }
}; // class RotorMutex<n_states__>

} // namespace component
} // namespace gyros

#endif // include guard

