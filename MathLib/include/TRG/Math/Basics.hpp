//
// Created by ianpo on 26/01/2025.
//

#pragma once

#include "Types.hpp"

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

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::qua<T,Q> LookAt(const glm::vec<3,T,Q>& forward, const glm::vec<3,T,Q>& up) {
		return glm::quatLookAt(forward, up);
	}

	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	inline static glm::qua<T,Q> MakeQuat(const T radians, const glm::vec<3,T,Q>& axis) {
		return glm::angleAxis(radians, axis);
	}

} // TRG
