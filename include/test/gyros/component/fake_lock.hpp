// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_COMPONENT_FAKE_LOCK_HPP_
#define TEST_GYROS_COMPONENT_FAKE_LOCK_HPP_

namespace test {
namespace gyros {
namespace component {

template <ptrdiff_t read_offset__ = 0, ptrdiff_t write_offset__ = 0>
struct FakeLock {
  ptrdiff_t read_offset() const {
    return read_offset__;
  }
  ptrdiff_t write_offset() const {
    return write_offset__;
  }
}; // struct FakeLock

} // namespace component
} // namespace gyros
} // namespace test

#endif // include guard

