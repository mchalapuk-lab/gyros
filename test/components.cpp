#include "test/gyros/components.hpp"

namespace test {
namespace gyros {
namespace component {

size_t CountingComponent::constructor_calls = 0;
size_t CountingComponent::destructor_calls = 0;

} // namespace component
} // namespace gyros
} // namespace test

