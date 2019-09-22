#include <gtest/gtest.h>
#include "flow.hpp"


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
  bflow::one_shot<int> a_subflow_of_seven_and_nine = bflow::one_shot<int>::of(seven, nine);
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

TEST_F(AOneShotFlow, supportsInPlaceDefinedSubflows) {
  auto flow = bflow::one_shot<int>::of(nine, bflow::one_shot<int>::of(seven, nine), seven);

  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::accepted, flow.process(7));
  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::completed, flow.process(7));
}

TEST_F(AOneShotFlow, supportsSubflowsDefinedElsewhere) {
  auto flow = bflow::one_shot<int>::of(nine, a_subflow_of_seven_and_nine, seven);

  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::accepted, flow.process(7));
  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::completed, flow.process(7));
}

TEST_F(ALoopedFlow, supportsSubflowAlternatives) {
  auto flow = bflow::looped<int>::of(bflow::any<int>::of(seven, nine), nine);

  ASSERT_EQ(bflow::result::accepted, flow.process(7));
  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::completed, flow.process(9));

  ASSERT_EQ(bflow::result::accepted, flow.process(9));
  ASSERT_EQ(bflow::result::rejected, flow.process(7));
  ASSERT_EQ(bflow::result::completed, flow.process(9));
}

TEST_F(AOneShotFlow, notifiesRegisteredListenerOnCompletion) {
  int notified = 0;

  auto flow = bflow::one_shot<int>::of(seven, nine);
  flow.on_completion([&notified](){ notified++; });
  ASSERT_EQ(0, notified) << "Notified listener too soon";

  flow.process(9);
  ASSERT_EQ(0, notified) << "Notified listener too soon";

  flow.process(7);
  ASSERT_EQ(0, notified) << "Notified listener too soon";

  flow.process(9);
  ASSERT_EQ(1, notified) << "Failed to notify the listener once";

  flow.process(7);
  flow.process(9);
  ASSERT_EQ(1, notified) << "Notified listener too many times";
}
