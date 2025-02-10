//
// Created by ianpo on 30/01/2025.
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

	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	struct Ray {
		Ray() = default;
		~Ray() = default;
		Ray(glm::vec<L,T,Q> origin, glm::vec<L,T,Q> direction) : Origin(origin), Direction(direction) {}

		glm::vec<L,T,Q> GetPoint(T t) const {
			return Origin + Direction * t;
		}
		glm::vec<L,T,Q> Origin;
		glm::vec<L,T,Q> Direction;
	};

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	struct Plane {
		Plane() = default;
		~Plane() = default;
		Plane(glm::vec<3,T,Q> origin, glm::vec<3,T,Q> normal) : Origin(origin), Normal(normal) {}
		glm::vec<3,T,Q> Origin;
		glm::vec<3,T,Q> Normal;
	};

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	struct Circle {
		using Vector = glm::vec<2,T,Q>;
		Circle() = default;
		~Circle() = default;
		Circle(const glm::vec<2,T,Q>& center, T radius) : Center{center}, Radius{std::abs(radius)} {}

		glm::vec<2,T,Q> Center;
		T Radius;
	};

	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	struct Sphere {
		using Vector = glm::vec<3,T,Q>;
		Sphere() = default;
		~Sphere() = default;
		Sphere(const glm::vec<3,T,Q>& center, T radius) : Center{center}, Radius{std::abs(radius)} {}

		glm::vec<3,T,Q> Center;
		T Radius;
	};

}

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

	typedef Ray<3, Real> Ray3;
	typedef Ray<2, Real> Ray2;

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
