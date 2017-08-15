#include <gtest/gtest.h>
#include "../flow.hpp"


struct ALinearBFlow : public ::testing::Test
{
    static constexpr auto nine = [](int event) { return event == 9 ? bflow::result::completed : bflow::result::rejected; };
    static constexpr auto seven = [](int event) { return event == 7 ? bflow::result::completed : bflow::result::rejected; };
    static constexpr auto twos_till_nine = [](int event) {
        return event == 9 ? bflow::result::completed : event == 2 ? bflow::result::accepted : bflow::result::rejected;
    };
};

TEST_F(ALinearBFlow, withNoStepsAcceptsNothing) {
    bflow::linear<int> flow;

    ASSERT_EQ(bflow::result::rejected, flow.process(7));
}

TEST_F(ALinearBFlow, withOneStepAcceptsItsStepInput) {
    auto flow = bflow::linear<int>::of(nine);

    ASSERT_EQ(bflow::result::rejected, flow.process(7));
    ASSERT_EQ(bflow::result::completed, flow.process(9));
}

TEST_F(ALinearBFlow, withOneStepAcceptsItsStepInputTillItCompletes) {
    auto flow = bflow::linear<int>::of(twos_till_nine);

    ASSERT_EQ(bflow::result::accepted, flow.process(2));
    ASSERT_EQ(bflow::result::accepted, flow.process(2));
    ASSERT_EQ(bflow::result::accepted, flow.process(2));
    ASSERT_EQ(bflow::result::completed, flow.process(9));
}

TEST_F(ALinearBFlow, withMultipleStepsAcceptsItsStepsInputsOneByOne) {
    auto flow = bflow::linear<int>::of(nine, seven);

    ASSERT_EQ(bflow::result::rejected, flow.process(7));
    ASSERT_EQ(bflow::result::accepted, flow.process(9));
    ASSERT_EQ(bflow::result::rejected, flow.process(9));
    ASSERT_EQ(bflow::result::completed, flow.process(7));
}

TEST_F(ALinearBFlow, rejectsAnyInputOnceCompleted) {
    auto flow = bflow::linear<int>::of(twos_till_nine, seven);

    ASSERT_EQ(bflow::result::accepted, flow.process(2));
    ASSERT_EQ(bflow::result::accepted, flow.process(9));
    ASSERT_EQ(bflow::result::completed, flow.process(7));

    ASSERT_EQ(bflow::result::rejected, flow.process(7));
    ASSERT_EQ(bflow::result::rejected, flow.process(2));
    ASSERT_EQ(bflow::result::rejected, flow.process(9));
}

/*
struct ALoopedBFlow : public ALinearBFlow
{
};

TEST_F(ALoopedBFlow, onceCompletedRestartsItself) {
    auto flow = bflow::Looped<int>::of(nine, seven);
    ASSERT_EQ(bflow::result::accepted, flow.process(9));
    ASSERT_EQ(bflow::result::rejected, flow.process(9));
    ASSERT_EQ(bflow::result::completed, flow.process(7));
    ASSERT_EQ(bflow::result::rejected, flow.process(7));

    ASSERT_EQ(bflow::result::accepted, flow.process(9));
    ASSERT_EQ(bflow::result::rejected, flow.process(9));
    ASSERT_EQ(bflow::result::completed, flow.process(7));
    ASSERT_EQ(bflow::result::rejected, flow.process(7));
}

//TEST_F(ALinearBFlow, supportsSubflows) {
//    auto flow = bflow::linear<int>::of(nine, bflow::linear<int>::of(seven, nine), seven);
//
//    ASSERT_EQ(bflow::result::accepted, flow.process(9));
//    ASSERT_EQ(bflow::result::accepted, flow.process(7));
//    ASSERT_EQ(bflow::result::rejected, flow.process(7));
//    ASSERT_EQ(bflow::result::accepted, flow.process(9));
//    ASSERT_EQ(bflow::result::completed, flow.process(7));
//}

//TEST_F(ALoopedBFlow, supportsSubflowAlternatives) {
//    bflow::Looped<int> flow{bflow::AnyOf<int> {seven, nine}, nine};
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
*/
