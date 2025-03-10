//
// Created by ianpo on 26/01/2025.
//

#include <TRG/Math.hpp>
#include <gtest/gtest.h>

using namespace TRG::Literal;
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
	typedef std::array<TRG::Vec2, 4> MathArray;
	typedef std::vector<TRG::Vec3> MathVector;

	constexpr MathArray arr {
		TRG::Vec2{-1_r,-1_r},
		TRG::Vec2{-1_r,+1_r},
		TRG::Vec2{+1_r,+1_r},
		TRG::Vec2{+1_r,-1_r},
	};

	const TRG::Vec2 center = TRG::Math::CalculateCenter<MathArray::const_iterator, 2>(arr.cbegin(),arr.cend());

	EXPECT_REAL_EQ(center.x, 0_r);
	EXPECT_REAL_EQ(center.y, 0_r);

	// ============================================================== //

	const MathVector arr2 {
		TRG::Vec3{+0_r,+0_r, +3_r},
		TRG::Vec3{+0_r,+3_r, +3_r},
		TRG::Vec3{+4_r,+3_r, -1_r},
		TRG::Vec3{+4_r,+0_r, -1_r},
	};
	const TRG::Vec3 center2 = TRG::Math::CalculateCenter<MathVector::const_iterator, 3>(arr2.cbegin(),arr2.cend());

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

	// EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,1,0}, Vec3(1,1,0), {0,0,1}) * rad2deg, 315_r);
	// EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,1,0}, Vec3(-1,1,0), {0,0,1}) * rad2deg, 45_r);
	//
	// EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(1,0,1), {0,-1,0}) * rad2deg, 315_r);
	// EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(-1,0,1), {0,-1,0}) * rad2deg, 45_r);
	//
	// EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(0,1,1),{1,0,0}) * rad2deg, 315_r);
	// EXPECT_REAL_EQ(TRG::Math::SignedAngle(Vec3{0,0,1}, Vec3(0,-1,1),{1,0,0}) * rad2deg, 45_r);
}

TEST(MathTest, OrientationTests) {
	const Vec2 A{0,0};
	const Vec2 B{0,1};
	const Vec2 C{-1,0};
	EXPECT_TRUE(TRG::Math::IsTriangleOriented(A, B, C));
	EXPECT_FALSE(TRG::Math::IsTriangleOriented(A, C, B));
}

TEST(MathTest, TriangleTests) {
	const Vec2 A{-1,-1};
	const Vec2 B{1,-1};
	const Vec2 C{0, 1};

	const Vec2 P_Inside = {0,0};
	const Vec2 P_Outside = {1,1};
	const Vec2 P_Outside2 = {-0.25,0.75};

	EXPECT_TRUE(TRG::Math::PointIsInsideTriangle(A, B, C, P_Inside));
	EXPECT_TRUE(TRG::Math::PointIsInsideTriangle(A, C, B, P_Inside));

	EXPECT_TRUE(TRG::Math::PointIsInsideTriangle(A, B, C, A));
	EXPECT_TRUE(TRG::Math::PointIsInsideTriangle(A, B, C, B));
	EXPECT_TRUE(TRG::Math::PointIsInsideTriangle(A, B, C, C));

	EXPECT_FALSE(TRG::Math::PointIsInsideTriangle(A, B, C, P_Outside));
	EXPECT_FALSE(TRG::Math::PointIsInsideTriangle(A, C, B, P_Outside));

	EXPECT_FALSE(TRG::Math::PointIsInsideTriangle(A, B, C, P_Outside2));
	EXPECT_FALSE(TRG::Math::PointIsInsideTriangle(A, C, B, P_Outside2));

	const Vec2 p1{-6,3};
	const Vec2 p2{-3,2};
	const Vec2 p3{0,3};
	const Vec2 center{-3, 7};
	const Real radius{5};

	const auto circle = Math::GetCircle(p1, p2, p3);
	const Vec2 calc_center = Math::GetCircleCenter(p1, p2, p3);
	const Real calc_radius = Math::GetCircleRadius(p1, p2, p3);

	EXPECT_REAL_EQ(calc_center.x, center.x);
	EXPECT_REAL_EQ(calc_center.y, center.y);
	EXPECT_REAL_EQ(calc_radius, radius);

	EXPECT_REAL_EQ(circle.Center.x, center.x);
	EXPECT_REAL_EQ(circle.Center.y, center.y);
	EXPECT_REAL_EQ(circle.Radius, radius);

	const Vec3 p1_3d{-6,3,0};
	const Vec3 p2_3d{-3,2,0};
	const Vec3 p3_3d{0,3,0};
	const Vec3 center_3d{-3, 7,0};
	const Real radius_3d{5};

	const auto sphere = Math::GetSphere(p1_3d, p2_3d, p3_3d);
	const Vec2 calc_center_3d = Math::GetSphereCenter(p1_3d, p2_3d, p3_3d);
	const Real calc_radius_3d = Math::GetSphereRadius(p1_3d, p2_3d, p3_3d);

	EXPECT_REAL_EQ(calc_center_3d.x, center_3d.x);
	EXPECT_REAL_EQ(calc_center_3d.y, center_3d.y);
	EXPECT_REAL_EQ(calc_radius_3d, radius_3d);

	EXPECT_REAL_EQ(sphere.Center.x, center_3d.x);
	EXPECT_REAL_EQ(sphere.Center.y, center_3d.y);
	EXPECT_REAL_EQ(sphere.Center.z, center_3d.z);
	EXPECT_REAL_EQ(sphere.Radius, radius_3d);
}

TEST(MathTest, RayCastTests) {
	const Plane<Real> plane0{{0,0,0},{0,1,0}};
	const Ray<3, Real> ray0{{0,1,0},{0,-1,0}};
	const auto hit0 = Math::Raycast(plane0, ray0);
	ASSERT_TRUE(hit0.has_value());
	EXPECT_REAL_EQ(hit0.value(), 1);

	const Plane<Real> plane1{{0,0,0},{0,1,0}};
	const Ray<3, Real> ray1{{0,1,0},{0,1,0}};
	const auto hit1 = Math::Raycast(plane1, ray1);
	ASSERT_TRUE(hit1.has_value());
	EXPECT_REAL_EQ(hit1.value(), -1);

	const Plane<Real> plane2{{10,0,10},{0,1,0}};
	const Ray<3, Real> ray2{{0,1,0},{0,-1,0}};
	const auto hit2 = Math::Raycast(plane2, ray2);
	ASSERT_TRUE(hit2.has_value());
	EXPECT_REAL_EQ(hit2.value(), 1);


	const Plane<Real> plane3{{0,.5,0},{0,1,0}};
	const Ray<3, Real> ray3{{0,1,0},{0,-1,0}};
	const auto hit3 = Math::Raycast(plane3, ray3);
	ASSERT_TRUE(hit3.has_value());
	EXPECT_REAL_EQ(hit3.value(), 0.5);


	const Plane<Real> plane4{{0,0,0},{0,1,0}};
	const Ray<3, Real> ray4{{0,1,0},{1,0,0}};
	const auto hit4 = Math::Raycast(plane4, ray4);
	ASSERT_FALSE(hit4.has_value());


	const Plane<Real> plane5{{-1,5,0},{1,0,0}};
	const Ray<3, Real> ray5{{0,1,0},{1,0,0}};
	const auto hit5 = Math::Raycast(plane5, ray5);
	ASSERT_TRUE(hit5.has_value());
	EXPECT_REAL_EQ(hit5.value(), -1);


	const Plane<Real> plane6{{1,1,1},Math::Normalize(Vec3{1,1,1})};
	const Ray<3, Real> ray6{{0,-1,0},Math::Normalize(Vec3{1,1,1})};
	const auto hit6 = Math::Raycast(plane6, ray6);
	ASSERT_TRUE(hit6.has_value());
	EXPECT_REAL_EQ(hit6.value(), Math::Magnitude(Vec3{4_r/3_r}));


	const Plane<Real> plane7{{1,1,0},Math::Normalize(Vec3{1,1,0})};
	const Ray<3, Real> ray7{{0,-1,0},Math::Normalize(Vec3{1,1,0})};
	const auto hit7 = Math::Raycast(plane7, ray7);
	ASSERT_TRUE(hit7.has_value());
	EXPECT_REAL_EQ(hit7.value(), std::sqrt(4.5_r));
}