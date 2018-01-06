#include "gtest/gtest.h"
#include "CvPlus/CvPlus.h"

TEST(SplitRect, EmptyRect)
{
    cv::Rect emptyRect;
    auto result = cvplus::split(emptyRect);
    AVAR(result);
    EXPECT_EQ(1, result.size());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    APAUSE;
}