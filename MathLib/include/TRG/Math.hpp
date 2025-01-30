//
// Created by ianpo on 26/01/2025.
//

#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#define TRG_SQR(a) (a * a)
#define GLM_VEC_T(L,T,Q) glm::vec<L,T,Q>

#ifdef TRG_DOUBLE
	#define REAL_DECIMAL_DIG	DBL_DECIMAL_DIG		// # of decimal digits of rounding precision
	#define REAL_DIG			DBL_DIG				// # of decimal digits of precision
	#define REAL_EPSILON		DBL_EPSILON			// smallest such that 1.0+DBL_EPSILON != 1.0
	#define REAL_HAS_SUBNORM	DBL_HAS_SUBNORM		// type does support subnormal numbers
	#define REAL_GUARD			FLT_GUARD
	#define REAL_MANT_DIG		DBL_MANT_DIG		// # of bits in mantissa
	#define REAL_MAX			DBL_MAX				// max value
	#define REAL_MAX_10_EXP		DBL_MAX_10_EXP		// max decimal exponent
	#define REAL_MAX_EXP		DBL_MAX_EXP			// max binary exponent
	#define REAL_MIN			DBL_MIN				// min normalized positive value
	#define REAL_MIN_10_EXP		DBL_MIN_10_EXP		// min decimal exponent
	#define REAL_MIN_EXP		DBL_MIN_EXP			// min binary exponent
	#define REAL_NORMALIZE		FLT_NORMALIZE
	#define REAL_RADIX			DBL_RADIX			// exponent radix
	#define REAL_TRUE_MIN		DBL_TRUE_MIN		// min positive value

namespace TRG {
	using Real = double;
}
#else
#define REAL_DECIMAL_DIG	FLT_DECIMAL_DIG		// # of decimal digits of rounding precision
#define REAL_DIG			FLT_DIG				// # of decimal digits of precision
#define REAL_EPSILON		FLT_EPSILON			// smallest such that 1.0+FLT_EPSILON != 1.0
#define REAL_HAS_SUBNORM	FLT_HAS_SUBNORM		// type does support subnormal numbers
#define REAL_GUARD			FLT_GUARD
#define REAL_MANT_DIG		FLT_MANT_DIG		// # of bits in mantissa
#define REAL_MAX			FLT_MAX				// max value
#define REAL_MAX_10_EXP		FLT_MAX_10_EXP		// max decimal exponent
#define REAL_MAX_EXP		FLT_MAX_EXP			// max binary exponent
#define REAL_MIN			FLT_MIN				// min normalized positive value
#define REAL_MIN_10_EXP		FLT_MIN_10_EXP		// min decimal exponent
#define REAL_MIN_EXP		FLT_MIN_EXP			// min binary exponent
#define REAL_NORMALIZE		FLT_NORMALIZE
#define REAL_RADIX			FLT_RADIX			// exponent radix
#define REAL_TRUE_MIN		FLT_TRUE_MIN		// min positive value

namespace TRG {
	using Real = float;
}
#endif


namespace TRG {

	typedef glm::qua<Real> Quat;
	typedef glm::vec<4, Real> Vec4;
	typedef glm::vec<3, Real> Vec3;
	typedef glm::vec<2, Real> Vec2;
	typedef glm::vec<1, Real> Vec1;

	typedef glm::mat<4, 4, Real> Mat4x4; typedef Mat4x4 Mat4;
	typedef glm::mat<3, 3, Real> Mat3x3; typedef Mat3x3 Mat3;
	typedef glm::mat<2, 2, Real> Mat2x2; typedef Mat2x2 Mat2;
	typedef glm::mat<3, 2, Real> Mat3x2;

	inline static constexpr Real tau {6.28318530717958647692};
	inline static constexpr Real pi  {3.14159265358979323846};
	inline static constexpr Real phi {1.61803398874989484820};
	inline static constexpr Real deg2rad {pi / static_cast<Real>(180)};
	inline static constexpr Real rad2deg {static_cast<Real>(180) / pi};

	namespace Literal
	{
		constexpr Real operator ""_r(const unsigned long long int value) {return static_cast<Real>(value);}
		constexpr Real operator ""_r(const long double value) {return static_cast<Real>(value);}
	}

}

namespace TRG::Math {
	template<typename T>
	inline static constexpr T Identity() {
		return glm::identity<T>();
	}
	template<typename T>
	inline static T Sign(T value) {
		return value < static_cast<T>(0) ? static_cast<T>(-1) : static_cast<T>(1);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::mat<L,L,T,Q> Inverse(const glm::mat<L,L,T,Q>& mat) {
		return glm::inverse(mat);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Determinant(const glm::mat<L,L,T,Q>& mat) {
		return glm::determinant(mat);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Dot(const glm::vec<L,T,Q>& x, const glm::vec<L,T,Q>& y) {
		return glm::dot(x,y);
	}

	/**
	 * Calculate the angle in radians between the vector 1 & 2.
	 * @tparam L Length of the vector.
	 * @tparam T Type of the vector
	 * @tparam Q Alignment of the vector.
	 * @param x Vector 1.
	 * @param y Vector 2.
	 * @return Angle in radians between the vector 1 & 2.
	 */
	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Angle(glm::vec<L,T,Q> x, glm::vec<L,T,Q> y) {
		x = glm::normalize(x);
		y = glm::normalize(y);
		return glm::acos(Dot(x,y));
	}

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::vec<3,T,Q> Cross(const glm::vec<3,T,Q>& x, const glm::vec<3,T,Q>& y) {
		return glm::cross(x,y);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Magnitude2(const glm::vec<L,T,Q>& x) {
		return glm::length2(x);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Magnitude(const glm::vec<L,T,Q>& x) {
		return glm::length(x);
	}

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Magnitude2(const glm::qua<T,Q>& x) {
		return glm::length2(x);
	}

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T Magnitude(const glm::qua<T,Q>& x) {
		return glm::length(x);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static GLM_VEC_T(L,T,Q) Normalize(const glm::vec<L,T,Q>& x) {
		return glm::normalize(x);
	}

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static void NormalizeInPlace(glm::vec<L,T,Q>& x) {
		const T len = Magnitude(x);
		x /= len;
	}

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::qua<T,Q> Normalize(const glm::qua<T,Q>& x) {
		return glm::normalize(x);
	}

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static void NormalizeInPlace(glm::qua<T,Q>& x) {
		const T len = Magnitude(x);
		x /= len;
	}

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static GLM_VEC_T(3,T,Q) CalculateTriangleNormal(const GLM_VEC_T(3,T,Q)& a, const GLM_VEC_T(3,T,Q)& b, const GLM_VEC_T(3,T,Q)& c) {
		const GLM_VEC_T(3,T,Q) AB = b - a;
		const GLM_VEC_T(3,T,Q) AC = c - a;

		return Normalize(Cross(AB,AC));
	}

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static GLM_VEC_T(3,T,Q) CalculateTriangleNormalNonNormalized(const GLM_VEC_T(3,T,Q)& a, const GLM_VEC_T(3,T,Q)& b, const GLM_VEC_T(3,T,Q)& c) {
		const GLM_VEC_T(3,T,Q) AB = b - a;
		const GLM_VEC_T(3,T,Q) AC = c - a;

		return Cross(AB,AC);
	}

	// template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	// inline static bool IsTriangleOriented(const glm::vec<3,T,Q>& AB, const glm::vec<3,T,Q>& AC, const glm::vec<3,T,Q>& normal) {
	// 	return Dot(Cross(AB, AC), normal) > 0;
	// }

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static bool IsTriangleOriented(const glm::vec<2,T,Q>& AB, const glm::vec<2,T,Q>& AC) {
		return Determinant(glm::mat<2,2,T,Q>{
			AB,
			AC,
		}) > 0;
	}

	// template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	// inline static bool IsTriangleOriented(const glm::vec<3,T,Q>& a, const glm::vec<3,T,Q>& b, const glm::vec<3,T,Q>& c, const glm::vec<3,T,Q>& normal) {
	// 	const auto AB = b - a;
	// 	const auto AC = c - a;
	// 	return IsTriangleOriented(AB, AC, normal);
	// }

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static bool IsTriangleOriented(const glm::vec<2,T,Q>& a, const glm::vec<2,T,Q>& b, const glm::vec<2,T,Q>& c) {
		const auto AB = b - a;
		const auto AC = c - a;
		return IsTriangleOriented(AB, AC);
	}

	/**
	 * Calculate the signed angle in radians between the vector 1 & 2.
	 * @tparam T Type of the vector
	 * @tparam Q Alignment of the vector.
	 * @param x Vector 1.
	 * @param y Vector 2.
	 * @return Signed angle in radians between 0 & 2*PI.
	 */
	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static T SignedAngle(glm::vec<2,T,Q> x, glm::vec<2,T,Q> y) {
		const auto angle = Angle(x,y);
		if (IsTriangleOriented(x,y)) {
			return angle;
		} else {
			return static_cast<T>(2*pi) - angle;
		}
	}

	// /**
	//  * Calculate the signed angle in radians between the vector 1 & 2.
	//  * @tparam T Type of the vector
	//  * @tparam Q Alignment of the vector.
	//  * @param x Vector 1.
	//  * @param y Vector 2.
	//  * @return Signed angle in radians between 0 & 2*PI.
	//  */
	// template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	// inline static T SignedAngle(glm::vec<3,T,Q> x, glm::vec<3,T,Q> y,  const glm::vec<3,T,Q>& normal) {
	// 	const auto angle = Angle(x,y);
	// 	if (IsTriangleOriented(x,y, normal)) {
	// 		return angle;
	// 	} else {
	// 		return static_cast<T>(2*pi) - angle;
	// 	}
	// }

	template<class fwd_iterator, glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::vec<L,T,Q> CalculateCenter(const fwd_iterator begin, const fwd_iterator end) {
		glm::vec<L,T,Q> center{static_cast<T>(0)};
		uint64_t count{0ull};
		for (auto it = begin; it != end; ++it) {
			center += *it;
			++count;
		}
		center /= static_cast<T>(count);
		return center;
	}

	template<class fwd_iterator, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::vec<3,T,Q> CalculateSurfaceNormal(const fwd_iterator begin, const fwd_iterator end) {
		glm::vec<3,T,Q> normal{static_cast<T>(0)};
		for (auto it = begin; it != end; ++it) {
			glm::vec<3,T,Q> next;
			auto nextIt = it; ++nextIt;
			if (nextIt == end) next = *begin;
			else next = *nextIt;
			const glm::vec<3,T,Q> current = *it;

			normal.x += (current.y - nextIt.y) * (current.z + next.z);
			normal.y += (current.z - nextIt.z) * (current.x + next.x);
			normal.z += (current.x - nextIt.x) * (current.y + next.y);
		}
		return Normalize(normal);
	}


	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::qua<T,Q> LookAt(const glm::vec<3,T,Q>& forward, const glm::vec<3,T,Q>& up) {
		return glm::quatLookAt(forward, up);
	}

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::qua<T,Q> MakeQuat(const T radians, const glm::vec<3,T,Q>& axis) {
		return glm::angleAxis(radians, axis);
	}

} // TRG
