// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENT_FAKE_LOCK_HPP_
#define TEST_GYROS_COMPONENT_FAKE_LOCK_HPP_

namespace test {
namespace gyros {
namespace component {

struct FakeLock {
  FakeLock(ptrdiff_t read_offset = 0, ptrdiff_t write_offset = 0)
      : read_offset_(read_offset),
      write_offset_(write_offset) {
      }

  ptrdiff_t read_offset() const {
    return read_offset_;
  }
  ptrdiff_t write_offset() const {
    return write_offset_;
  }
 private:
  ptrdiff_t read_offset_;
  ptrdiff_t write_offset_;
}; // struct FakeLock

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

