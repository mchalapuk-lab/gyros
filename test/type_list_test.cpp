// author: Maciej Chałapuk
// license: Apache2
// vim: ts=2 sw=2 expandtab
#include "gyros/util/type_list.hpp"

#include <type_traits>

#include "test/gyros/components.hpp"
#include "test/static_assert.hpp"

using Simple = test::gyros::component::EmptyComponent;
template <class Type>
using Member = test::gyros::component::OneMemberComponent<Type>;
using Mock = test::gyros::component::MockComponent;

using namespace gyros::util::type_list;
using namespace test;

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

// back

static_assert(
    std::is_same<typename Back<TypeList<Simple>>::Type, Simple>::value,
    "test getting back type of singleton list"
    );
static_assert(
    std::is_same<typename Back<TypeList<Simple, Mock>>::Type, Mock>::value,
    "test getting back type of 2-element list"
    );
void testGettingBackTypeOfEightElementList() {
  typedef TypeList<Mock, Mock, Mock, Mock, Mock, Mock, Mock, Simple> List;
  AssertIsSame<typename Back<List>::Type, Simple>();
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

// pop back

static void testPoppingBackSimngletonList() {
  typedef typename PopBack<TypeList<Simple>>::Type AfterPop;
  AssertIsSame<AfterPop, TypeList<>>();
}
static void testPoppingBack2ElementList() {
  typedef typename PopBack<TypeList<Simple, Mock>>::Type AfterPop;
  AssertIsSame<AfterPop, TypeList<Simple>>();
}

// pop front

static void testPoppingFrontSimngletonList() {
  typedef typename PopFront<TypeList<Simple>>::Type AfterPop;
  AssertIsSame<AfterPop, TypeList<>>();
}
static void testPoppingFront2ElementList() {
  typedef typename PopFront<TypeList<Simple, Mock>>::Type AfterPop;
  AssertIsSame<AfterPop, TypeList<Mock>>();
}

// index of

static_assert(IndexOf<TypeList<>, Simple>::value == -1,
              "test index of in empty list equals -1");
static_assert(IndexOf<TypeList<Mock>, Simple>::value == -1,
              "test index of in singleton list equals -1");
static_assert(IndexOf<TypeList<Mock, Mock>, Simple>::value == -1,
              "test index of in 2-element list equals -1");
static_assert(IndexOf<TypeList<Simple>, Simple>::value == 0,
              "test index of in singleton list equals 0");
static_assert(IndexOf<TypeList<Simple, Mock>, Simple>::value == 0,
              "test index of in a 2-element list equals 0");
static_assert(IndexOf<TypeList<Simple, Mock>, Mock>::value == 1,
              "test index of in a 2-element list equals 1");
static_assert(IndexOf<TypeList<Simple, Mock, Simple>, Mock>::value == 1,
              "test index of in a 3-element list equals 1");
// contains

static_assert(!Contains<TypeList<>, Simple>::value,
              "test empty list doesnt contain any type");
static_assert(!Contains<TypeList<Mock>, Simple>::value,
              "test singleton list doesnt contain a type");
static_assert(!Contains<TypeList<Mock, Mock>, Simple>::value,
              "test 2-element list doesnt contain a type");
static_assert(Contains<TypeList<Simple>, Simple>::value,
              "test singleton list contains simple type");
static_assert(Contains<TypeList<Simple, Mock>, Simple>::value,
              "test list contains its first type");
static_assert(Contains<TypeList<Simple, Mock>, Mock>::value,
              "test list contains its last type");
static_assert(Contains<TypeList<Simple, Mock, Simple>, Mock>::value,
              "test list contains its middle type");

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

// cast

static void testCastingEmptyListToTuple() {
  typedef std::tuple<> ExpectedType;
  typedef typename Cast<std::tuple, TypeList<>>::Type ActualType;
  AssertIsSame<ExpectedType, ActualType>();
}
static void testCastingSingletonListToTuple() {
  typedef std::tuple<Simple> ExpectedType;
  typedef typename Cast<std::tuple, TypeList<Simple>>::Type ActualType;
  AssertIsSame<ExpectedType, ActualType>();
}
static void testCasting3ElementListToTuple() {
  typedef std::tuple<Simple, Mock, Member<int>>
      ExpectedType;
  typedef typename Cast<std::tuple, TypeList<Simple, Mock, Member<int>>>::Type
      ActualType;
  AssertIsSame<ExpectedType, ActualType>();
}

