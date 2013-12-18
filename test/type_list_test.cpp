// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#include "gyros/util/type_list.hpp"

#include <type_traits>

#include "test/gyros/components.hpp"

using Simple = test::gyros::component::EmptyComponent;
template <class Type>
using Member = test::gyros::component::OneMemberComponent<Type>;
using Mock = test::gyros::component::MockComponent;

using namespace gyros::util::type_list;

template <class Expected, class Actual>
struct AssertIsSame {
  static_assert(std::is_same<Expected, Actual>::value, "type are not same");
}; // struct AssertIsSame<Expected, Actual>

/*
 * All tests in this file are executed during compile time ^^
 * All functions are static (not visible outside this file) and never called.
 */

// size

static_assert(Size<TypeList<>>::value == 0,
              "test size of empty type list equals 0");
static_assert(Size<TypeList<Simple>>::value == 1,
              "test size of singleton type list equals 1");
static_assert(Size<TypeList<Member<int>, Member<short>>>::value == 2,
              "test size of list with 3 types");
static_assert(Size<TypeList<Simple, Mock, Member<bool>>>::value == 3,
              "test size of list with 3 types");
static_assert(Size<TypeList<Simple, Simple, Simple, Simple>>::value == 4,
              "test size of list with 4 same types");

// get

static_assert(
    std::is_same<typename Get<TypeList<Simple>, 0>::Type, Simple>::value,
    "test getting first type"
    );
static_assert(
    std::is_same<typename Get<TypeList<Simple, Mock>, 1>::Type, Mock>::value,
    "test getting second type"
    );
void testGettingEightTest() {
  typedef TypeList<Mock, Mock, Mock, Mock, Mock, Mock, Mock, Simple> List;
  AssertIsSame<typename Get<List, 7>::Type, Simple>();
}

// push back

static void testPushingBackToEmptyList() {
  typedef typename PushBack<TypeList<>, Simple>::Type AfterPush;
  AssertIsSame<AfterPush, TypeList<Simple>>();
}
static void testPushingBackToNotEmptyList() {
  typedef typename PushBack<TypeList<Simple>, Simple>::Type AfterPush;
  AssertIsSame<AfterPush, TypeList<Simple, Simple>>();
}

// cat

static void testConcatenatingTwoEmptyLists() {
  typedef typename Cat<TypeList<>, TypeList<>>::Type Concatenated;
  AssertIsSame<Concatenated, TypeList<>>();
}
static void testConcatenatingNotEmptyListWithEmptyList() {
  typedef typename Cat<TypeList<Simple>, TypeList<>>::Type Concatenated;
  AssertIsSame<Concatenated, TypeList<Simple>>();
}
static void testConcatenatingEmptyListWithNotEmptyList() {
  typedef typename Cat<TypeList<>, TypeList<Simple>>::Type Concatenated;
  AssertIsSame<Concatenated, TypeList<Simple>>();
}
static void testConcatenatingTwoSingletonLists() {
  typedef typename Cat<TypeList<Simple>, TypeList<Simple>>::Type Concatenated;
  AssertIsSame<Concatenated, TypeList<Simple, Simple>>();
}
static void testConcatenatingTwoListsWithTwoTypes() {
  typedef TypeList<Mock, Mock> FirstList;
  typedef TypeList<Simple, Simple> SecondList;
  typedef TypeList<Mock, Mock, Simple, Simple> ExpectedList;
  typedef typename Cat<FirstList, SecondList>::Type Concatenated;
  AssertIsSame<Concatenated, ExpectedList>();
}
static void testConcatenatingThreeSingletonLists() {
  typedef TypeList<Mock> FirstList;
  typedef TypeList<Simple> SecondList;
  typedef TypeList<Member<int>> ThirdList;
  typedef TypeList<Mock, Simple, Member<int>> ExpectedList;
  typedef typename Cat<FirstList, SecondList, ThirdList>::Type Concatenated;
  AssertIsSame<Concatenated, ExpectedList>();
}

// permute

static_assert(Size<typename Permute<TypeList<>>::Type>::value == 0,
              "test permutations of emptylist is empty list");
static_assert(Size<typename Permute<TypeList<Simple>>::Type>::value == 1,
              "test permutations of singleton is list is list of size 1");

static void testCreatingPermutationsOfOneTypeList() {
  typedef TypeList<TypeList<Simple>, TypeList<Member<int>>> ExpectedList;
  typedef typename Permute<TypeList<Simple, Member<int>>>::Type ActualList;
  AssertIsSame<ExpectedList, ActualList>();
}
static void testCreatingCartesianProductOfTypeListWithItself() {
  typedef TypeList<
      TypeList<Simple, Simple>,
      TypeList<Simple, Member<int>>,
      TypeList<Member<int>, Simple>,
      TypeList<Member<int>, Member<int>>
          > ExpectedList;
  typedef typename Permute<TypeList<Simple, Member<int>>>::Type PermutedList;
  typedef typename Permute<PermutedList, PermutedList>::Type ActualList;
  AssertIsSame<ExpectedList, ActualList>();
}
static void testCreatingCartesianProductOfTwoTypeListsOfSameSize() {
  typedef TypeList<
      TypeList<Mock, Member<char>>,
      TypeList<Mock, Member<int>>,
      TypeList<Simple, Member<char>>,
      TypeList<Simple, Member<int>>
          > ExpectedList;
  typedef typename Permute<TypeList<Mock, Simple>>::Type
      FirstPermutedList;
  typedef typename Permute<TypeList<Member<char>, Member<int>>>::Type
      SecondPermutedList;
  typedef typename Permute<FirstPermutedList, SecondPermutedList>::Type
      ActualList;
  AssertIsSame<ExpectedList, ActualList>();
}
static void testCreatingCartesianProductOfTwoTypeListsOfDifferentSizes() {
  typedef TypeList<TypeList<Mock, Member<char>>, TypeList<Mock, Member<int>>>
      ExpectedList;
  typedef typename Permute<TypeList<Mock>>::Type
      FirstPermutedList;
  typedef typename Permute<TypeList<Member<char>, Member<int>>>::Type
      SecondPermutedList;
  typedef typename Permute<FirstPermutedList, SecondPermutedList>::Type
      ActualList;
  AssertIsSame<ExpectedList, ActualList>();
}
static void testCreatingCartesianProductOfThreeTypeLists() {
  typedef TypeList<TypeList<Mock, Member<char>, Member<int>>> ExpectedList;
  typedef typename Permute<TypeList<Mock>>::Type FirstPermutedList;
  typedef typename Permute<TypeList<Member<char>>>::Type SecondPermutedList;
  typedef typename Permute<TypeList<Member<int>>>::Type ThirdPermutedList;
  typedef typename Permute<FirstPermutedList,
                           SecondPermutedList,
                           ThirdPermutedList>::Type ActualList;
  AssertIsSame<ExpectedList, ActualList>();
}

// power set

static void testCreatingPowerSetFromEmptyTypeList() {
  typedef TypeList<TypeList<>> ExpectedSets;
  typedef typename PowerSet<TypeList<>>::Type ActualSets;
  AssertIsSame<ActualSets, ExpectedSets>();
}
static void testCreatingPowerSetFromSingletonTypeList() {
  typedef TypeList<TypeList<>, TypeList<Simple>> ExpectedSets;
  typedef typename PowerSet<TypeList<Simple>>::Type ActualSets;
  AssertIsSame<ActualSets, ExpectedSets>();
}

