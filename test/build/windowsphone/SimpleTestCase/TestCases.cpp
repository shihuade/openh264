#include <gtest/gtest.h>

int add(int a, int b)
{

	return a + b;
}

TEST(AddTest, PositiveTest)
{
	EXPECT_EQ(3,add(1,2));

}

TEST(AddTest, NegativeTest)
{
	EXPECT_EQ(-3, add(-1, -2));

}