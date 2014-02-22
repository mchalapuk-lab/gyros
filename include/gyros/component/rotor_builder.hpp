// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#ifndef GYROS_COMPONENT_ROTOR_BUILDER_HPP_
#define GYROS_COMPONENT_ROTOR_BUILDER_HPP_

#include <vector>
#include <type_traits>
#include <functional>

#include "gyros/util/type_literal.hpp"
#include "gyros/util/type_list.hpp"
#include "gyros/type_traits.hpp"
#include "gyros/fwd/component/rotor.hpp"
#include "gyros/fwd/component/rotor_builder.hpp"

namespace gyros {
namespace tl = util::type_list;
namespace component {

template <class ComponentType, class ...ArgTypes>
ComponentType* allocateComponent(void *placement, ArgTypes &&...args) {
  return new (placement) ComponentType(std::forward<ArgTypes>(args)...);
}

template <class T>
using RawMemory = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

template <class ...ComponentTypes>
struct RotorBuilder {
}; // struct Builder

template <class T, class ...L>
class RotorBuilder<T, L...> : protected RotorBuilder<L...> {
 public:
  typedef RotorBuilder<T, L...> Type;
  typedef RotorBuilder<L...> SuperType;
  typedef Rotor<T, L...> RotorType;
  typedef T FirstComponentType;
  typedef std::function<FirstComponentType *(void *)> FactoryType;

  template <class EmplacedComponentType, class ...ArgTypes>
  RotorBuilder<T, L...>& emplace(ArgTypes &&...args) {
    auto factory = std::bind(
        &allocateComponent<EmplacedComponentType, ArgTypes...>,
        std::placeholders::_1,
        std::forward<ArgTypes>(args)...
        );
    return addFactory<EmplacedComponentType>(std::move(factory));
  }
  template <class EmplacedComponentType, class FactoryType>
  RotorBuilder<T, L...>& addFactory(FactoryType factory) {
    addFactory0(
        std::forward<FactoryType>(factory),
        util::TypeLiteral<EmplacedComponentType>()
        );
    return *this;
  }

  RotorType build() const {
    RecursiveForwardType build0 {
      ComponentAllocator { *this },
      BuildFinisher()
    };
    return build0();
  }

  std::vector<FactoryType> const& factories() const {
    return factories_;
  }
 protected:
  template <class Type>
  using GetSuperType = GetAncestor<Type, 1>;

  struct ComponentAllocator {
    template <class CurrentType, class ForwarderType, class ...ArgTypes>
    RotorType operator() (ForwarderType const& forward,
                          ArgTypes &&...args) const {
      typedef typename CurrentType::FirstComponentType ComponentType;
      auto & factories = builder_.CurrentType::factories();
      size_t capacity = factories.size();

      auto *pool = new RawMemory<ComponentType>[capacity * 3];

      auto it = pool;
      for (auto &factory : factories) {
        factory(it++);
      }
      std::copy(pool, it, it);
      it += capacity;
      std::copy(it, it, it + capacity);

      return forward(std::forward<ArgTypes>(args)..., pool, capacity);
    }
    Type builder_;
  }; // struct ComponentAllocator

  struct BuildFinisher {
    template <class ...ArgTypes>
    RotorType operator() (ArgTypes &&...args) const {
      return RotorType(std::forward<ArgTypes>(args)...);
    }
  }; // struct BuildFinisher

  typedef RecursiveForward<
      Type,
      RotorBuilder<typename tl::Back<tl::TypeList<T, L...>>::Type>,
      GetSuperType,
      ComponentAllocator const,
      BuildFinisher const,
      RotorType> RecursiveForwardType;

  std::vector<FactoryType> factories_;

  using SuperType::addFactory0;
  template <class FactoryType>
  void addFactory0(FactoryType factory, util::TypeLiteral<T>) {
    factories_.push_back(std::forward<FactoryType>(factory));
  }
}; // class RotorBuilder<T, L...>

template <>
struct RotorBuilder<> {
  void addFactory0() {
  }
}; // struct RotorBuilder<>

} // namespace component
} // namespace gyros

#endif // include guard

