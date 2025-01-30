//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "Basics.hpp"

namespace TRG::Math {

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
		if (begin == end) return {};
		glm::vec<L,T,Q> center{static_cast<T>(0)};
		uint64_t count{0ull};
		for (auto it = begin; it != end; ++it) {
			center += *it;
			++count;
		}
		center /= static_cast<T>(count);
		return center;
	}
}
