#include <gtest/gtest.h>
#include "../flow.hpp"


struct AOneShotFlow : public ::testing::Test
{
  static constexpr auto nine = [](int event) {
    return event == 9 ? bflow::result::completed : bflow::result::rejected;
  };
  static constexpr auto seven = [](int event) {
    return event == 7 ? bflow::result::completed : bflow::result::rejected;
  };
  static constexpr auto twos_till_nine = [](int event) {
    return event == 9 ? bflow::result::completed : event == 2 ? bflow::result::accepted : bflow::result::rejected;
  };
};

TEST_F(AOneShotFlow, withNoStepsAcceptsNothing) {
  bflow::one_shot<int> flow;

  ASSERT_EQ(bflow::result::rejected, flow.process(7));
}

TEST_F(AOneShotFlow, withOneStepAcceptsItsStepInput) {
  auto flow = bflow::one_shot<int>::of(nine);

  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::completed, flow.process(9));
}

TEST_F(AOneShotFlow, withOneStepAcceptsItsStepInputTillItCompletes) {
  auto flow = bflow::one_shot<int>::of(twos_till_nine);

  ASSERT_EQ(bflow::result::accepted, flow.process(2));
  ASSERT_EQ(bflow::result::accepted, flow.process(2));
  ASSERT_EQ(bflow::result::accepted, flow.process(2));
  ASSERT_EQ(bflow::result::completed, flow.process(9));
}

TEST_F(AOneShotFlow, withMultipleStepsAcceptsItsStepsInputsOneByOne) {
  auto flow = bflow::one_shot<int>::of(nine, seven);

  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::rejected, flow.process(9));
  ASSERT_EQ(bflow::result::completed, flow.process(7));
}

TEST_F(AOneShotFlow, rejectsAnyInputOnceCompleted) {
  auto flow = bflow::one_shot<int>::of(twos_till_nine, seven);

  ASSERT_EQ(bflow::result::accepted, flow.process(2));
  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::completed, flow.process(7));

  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::rejected, flow.process(2));
  ASSERT_EQ(bflow::result::rejected, flow.process(9));
}

struct ALoopedFlow : public AOneShotFlow
{
};

TEST_F(ALoopedFlow, onceCompletedRestartsItself) {
  auto flow = bflow::looped<int>::of(nine, seven);

  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::rejected, flow.process(9));
  ASSERT_EQ(bflow::result::completed, flow.process(7));
  ASSERT_EQ(bflow::result::rejected, flow.process(7));

  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::rejected, flow.process(9));
  ASSERT_EQ(bflow::result::completed, flow.process(7));
  ASSERT_EQ(bflow::result::rejected, flow.process(7));
}

// TEST_F(AOneShotFlow, supportsSubflows) {
//    auto flow = bflow::one_shot<int>::of(nine, bflow::one_shot<int>::of(seven, nine), seven);
//
//    ASSERT_EQ(bflow::result::accepted, flow.process(9));
//    ASSERT_EQ(bflow::result::accepted, flow.process(7));
//    ASSERT_EQ(bflow::result::rejected, flow.process(7));
//    ASSERT_EQ(bflow::result::accepted, flow.process(9));
//    ASSERT_EQ(bflow::result::completed, flow.process(7));
//}

// TEST_F(ALoopedFlow, supportsSubflowAlternatives) {
//    bflow::looped<int> flow{bflow::AnyOf<int> {seven, nine}, nine};
//
//    ASSERT_EQ(bflow::result::accepted, flow.process(7));
//    ASSERT_EQ(bflow::result::rejected, flow.process(7));
//    ASSERT_EQ(bflow::result::completed, flow.process(9));
//
//    ASSERT_EQ(bflow::result::accepted, flow.process(9));
//    ASSERT_EQ(bflow::result::rejected, flow.process(7));
//    ASSERT_EQ(bflow::result::completed, flow.process(9));
//}
//

