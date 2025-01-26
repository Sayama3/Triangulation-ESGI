//
// Created by ianpo on 26/01/2025.
//

#pragma once

namespace TRG::Math {

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

	using Real = double;
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

	using Real = float;
#endif

	namespace Literal
	{
		constexpr Real operator ""_r(const unsigned long long int value) {return static_cast<Real>(value);}
		constexpr Real operator ""_r(const long double value) {return static_cast<Real>(value);}
	}

} // TRG
