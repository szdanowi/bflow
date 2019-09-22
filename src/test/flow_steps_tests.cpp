#include <gtest/gtest.h>
#include "../flow.hpp"

using namespace bflow;
using namespace bflow::detail;

TEST(InStepsWithNoElements, beginIsAnEnd) {
  auto s = make_steps<int>();
  ASSERT_EQ(s.begin(), s.end());
}

TEST(AnEndOfSteps, convertsToFalse) {
  ASSERT_FALSE(static_cast<bool>(make_steps<int>().end()));
}

TEST(NextToEndOfSteps, isAlsoAnEnd) {
  auto s = make_steps<int>();
  ASSERT_EQ(s.end().next(), s.end());
}

struct InStepsWithSingleElement : public ::testing::Test
{
  InStepsWithSingleElement() : s(make_steps<int>(element_1)) {}

  static constexpr auto element_1 = [](int p) { return p == 7 ? result::accepted : result::rejected; };
  steps<int> s;
};

TEST_F(InStepsWithSingleElement, beginDoesNotEqualToEnd) {
  ASSERT_NE(s.begin(), s.end());
}

TEST_F(InStepsWithSingleElement, beginConvertsToTrue) {
  ASSERT_TRUE(static_cast<bool>(s.begin()));
}

TEST_F(InStepsWithSingleElement, beginGivesAccessToFirstElement) {
  ASSERT_EQ(s.begin().process(7), element_1(7));
  ASSERT_EQ(s.begin().process(1), element_1(1));
}

TEST_F(InStepsWithSingleElement, nextToBeginIsEnd) {
  ASSERT_EQ(s.begin().next(), s.end());
}

struct InStepsWithTwoElements : public InStepsWithSingleElement
{
  InStepsWithTwoElements() : s(make_steps<int>(element_1, element_2)) {}

  static constexpr auto element_2 = [](int p) { return p == 1 ? result::accepted : result::rejected; };
  steps<int> s;
};

TEST_F(InStepsWithTwoElements, nextToBeginIsNotAnEnd) {
  ASSERT_NE(s.begin().next(), s.end());
}

TEST_F(InStepsWithTwoElements, nextToBeginGivesAccessToSecondElement) {
  ASSERT_EQ(s.begin().next().process(7), element_2(7));
  ASSERT_EQ(s.begin().next().process(1), element_2(1));
}

TEST_F(InStepsWithTwoElements, setpsMayBeIteratedUsingPreincrementation) {
  auto it = s.begin();
  ASSERT_EQ(s.begin().next(), ++it);
  ASSERT_EQ(s.begin().next(), it);
}

TEST_F(InStepsWithTwoElements, setpsMayBeIteratedUsingPostincrementation) {
  auto it = s.begin();
  ASSERT_EQ(s.begin(), it++);
  ASSERT_EQ(s.begin().next(), it);
}

TEST_F(InStepsWithTwoElements, nextToSecondIsEnd) {
  ASSERT_EQ(s.begin().next().next(), s.end());
}
