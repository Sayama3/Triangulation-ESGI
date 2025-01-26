//
// Created by ianpo on 26/01/2025.
//

#include <TRG/Math.hpp>
#include <gtest/gtest.h>

using namespace TRG::Math::Literal;

#ifdef TRG_DOUBLE
	#define EXPECT_REAL_EQ(val1, val2) EXPECT_DOUBLE_EQ(val1, val2)
	#define ASSERT_REAL_EQ(val1, val2) ASSERT_DOUBLE_EQ(val1, val2)
#else
	#define EXPECT_REAL_EQ(val1, val2) EXPECT_FLOAT_EQ(val1, val2)
	#define ASSERT_REAL_EQ(val1, val2) ASSERT_FLOAT_EQ(val1, val2)
#endif

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_REAL_EQ(7_r * 6_r, 42.0_r);
}

// Demonstrate some basic assertions.
TEST(MathTest, BasicVectorTests) {
	typedef std::array<TRG::Math::Vec2, 4> MathArray;
	typedef std::vector<TRG::Math::Vec2> MathVector;

	constexpr MathArray arr {
		TRG::Math::Vec2{-1_r,-1_r},
		TRG::Math::Vec2{-1_r,+1_r},
		TRG::Math::Vec2{+1_r,+1_r},
		TRG::Math::Vec2{+1_r,-1_r},
	};

	const TRG::Math::Vec2 center = TRG::Math::CalculateCenter<MathArray::const_iterator, 2>(arr.cbegin(),arr.cend());

	EXPECT_REAL_EQ(center.x, 0_r);
	EXPECT_REAL_EQ(center.y, 0_r);

	// ============================================================== //

	const MathVector arr2 {
		TRG::Math::Vec2{+0_r,+0_r},
		TRG::Math::Vec2{+0_r,+3_r},
		TRG::Math::Vec2{+4_r,+3_r},
		TRG::Math::Vec2{+4_r,+0_r},
	};
	const TRG::Math::Vec2 center2 = TRG::Math::CalculateCenter<MathVector::const_iterator, 2>(arr2.cbegin(),arr2.cend());

	EXPECT_REAL_EQ(center2.x, 2_r);
	EXPECT_REAL_EQ(center2.y, 3_r/2_r);
}
