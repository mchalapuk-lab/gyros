// author: Maciej Chałapuk
// license: MIT
// vim: ts=2 sw=2 expandtab
#ifndef TEST_GYROS_VISITOR_WRAPPER_HPP_
#define TEST_GYROS_VISITOR_WRAPPER_HPP_

namespace test {
namespace gyros {

template <class VisitorType>
class MockVisitorWrapper {
 public:
  MockVisitorWrapper(VisitorType &visitor) : delegate_(visitor) {
  }
  template <class Type>
  void operator() (Type &obj) {
    delegate_.call(obj);
  }
 private:
  VisitorType &delegate_;
}; // struct MockVisitorWrapper

template <class VisitorType>
MockVisitorWrapper<VisitorType> wrap(VisitorType &visitor) {
  return MockVisitorWrapper<VisitorType>(visitor);
}

} // namespace gyros
} // namespace test

#endif // include guard

