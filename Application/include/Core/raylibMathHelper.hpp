//
// Created by ianpo on 08/02/2025.
//

#pragma once

#include "TRG/Math/Types.hpp"
#include <raymath.h>

template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
inline static Matrix Mat4ToRaylib(const glm::mat<4,4,T,Q>& mat4) {
	return Matrix {
		mat4[0][0],mat4[1][0],mat4[2][0],mat4[3][0],
		mat4[0][1],mat4[1][1],mat4[2][1],mat4[3][1],
		mat4[0][2],mat4[1][2],mat4[2][2],mat4[3][2],
		mat4[0][3],mat4[1][3],mat4[2][3],mat4[3][3],
	};
}