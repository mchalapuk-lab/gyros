// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_MUTEX_HPP_
#define GYROS_COMPONENT_ROTOR_MUTEX_HPP_

#include <mutex>
#include <functional>
#include <algorithm>

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
  RotorMutex()
      : state_versions_(n_states__) {

    std::fill(state_versions_.begin(), state_versions_.end(), 0);
    for (auto it = state_versions_.begin(), end = state_versions_.end();
         it != end;
         ++it) {
      free_states_.push_back(it);
    }
    std::make_heap(free_states_.begin(),
                   free_states_.end(),
                   DereferencingComparator());
  }
  Lock acquireReadOnly(size_t *read_index) {
    std::lock_guard<std::mutex> guard(mutex_);
  
    // TODO  
  }
  Lock acquireReadWrite(size_t *read_index, size_t *write_index) {
    std::lock_guard<std::mutex> guard(mutex_);
  
    // TODO  
  }
 private:
  typedef std::vector<size_t>::iterator StatePointer;

  std::mutex mutex_;
  std::vector<size_t> state_versions_;
  std::vector<StatePointer> free_states_;
  std::vector<StatePointer> ro_states_;
}; // class RotorMutex<m_states__>

} // namespace component
} // namespace gyros

#endif // include guard

