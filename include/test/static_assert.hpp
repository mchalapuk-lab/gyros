// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef TEST_STATIC_ASSERT_HPP_
#define TEST_STATIC_ASSERT_HPP_

namespace test {

template <class Expected, class Actual>
struct AssertIsSame {
  static_assert(std::is_same<Expected, Actual>::value, "types are not same");
}; // struct AssertIsSame<Expected, Actual>

}; // namespace test

#endif // include guard

