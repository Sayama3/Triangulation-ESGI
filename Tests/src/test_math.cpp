//
// Created by ianpo on 26/01/2025.
//

#include <TRG/Math.hpp>
#include <gtest/gtest.h>

using namespace TRG::Math::Literal;
using namespace TRG::Math;
using namespace TRG;

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
	typedef std::vector<TRG::Math::Vec3> MathVector;

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
		TRG::Math::Vec3{+0_r,+0_r, +3_r},
		TRG::Math::Vec3{+0_r,+3_r, +3_r},
		TRG::Math::Vec3{+4_r,+3_r, -1_r},
		TRG::Math::Vec3{+4_r,+0_r, -1_r},
	};
	const TRG::Math::Vec3 center2 = TRG::Math::CalculateCenter<MathVector::const_iterator, 3>(arr2.cbegin(),arr2.cend());

	EXPECT_REAL_EQ(center2.x, 2_r);
	EXPECT_REAL_EQ(center2.y, 3_r/2_r);
	EXPECT_REAL_EQ(center2.z, 1_r);

	EXPECT_REAL_EQ(TRG::Math::Dot(Vec2{0,1}, Vec2(1,0)), 0);
	EXPECT_REAL_EQ(TRG::Math::Dot(Vec2{0,1}, Vec2(0,1)), 1);
	EXPECT_REAL_EQ(TRG::Math::Dot(Vec2{1,0}, Vec2(2,0)), 2);
	EXPECT_REAL_EQ(TRG::Math::Dot(Vec2{4,0}, Vec2(2,0)), 8);

	EXPECT_REAL_EQ(TRG::Math::Magnitude(Vec2{4,0}), 4);
	EXPECT_REAL_EQ(TRG::Math::Magnitude2(Vec2{4,0}), 16);
	EXPECT_REAL_EQ(TRG::Math::Magnitude2(Vec2{4,2}), 20);

	EXPECT_REAL_EQ(TRG::Math::Angle(Vec2{0,1}, Vec2(1,1)) * rad2deg, 45_r);
	EXPECT_REAL_EQ(TRG::Math::Angle(Vec2{0,1}, Vec2(1,-1)), pi * 0.75_r);
	EXPECT_REAL_EQ(TRG::Math::Angle(Vec2{0,1}, Vec2(1,-1)), pi * 0.75_r);
	EXPECT_REAL_EQ(TRG::Math::Angle(Vec2{0,1}, Vec2(0,1)), 0);
	EXPECT_REAL_EQ(TRG::Math::Angle(Vec2{0,1}, Vec2(0,-1)), pi);

	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec2{0,1}, Vec2(1,1)) * rad2deg, 315_r);
	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec2{0,1}, Vec2(-1,1)) * rad2deg, 45_r);

	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,1,0}, Vec3(1,1,0), {0,0,1}) * rad2deg, 315_r);
	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,1,0}, Vec3(-1,1,0), {0,0,1}) * rad2deg, 45_r);

	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(1,0,1), {0,-1,0}) * rad2deg, 315_r);
	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(-1,0,1), {0,-1,0}) * rad2deg, 45_r);

	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(0,1,1),{1,0,0}) * rad2deg, 315_r);
	EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(0,-1,1),{1,0,0}) * rad2deg, 45_r);
}
