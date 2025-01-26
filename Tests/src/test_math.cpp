//
// Created by ianpo on 26/01/2025.
//

#include <TRG/Math.hpp>
#include <gtest/gtest.h>

using namespace TRG::Math::Literal;

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7_r * 6_r, 42.0_r);
}
