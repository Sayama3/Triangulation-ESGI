//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "TRG/Math.hpp"
#include <raylib.h>

namespace TRG::Application {
	struct Camera {
	public:
		[[nodiscard]] Camera3D GetCamera3D() const;

		[[nodiscard]] Mat4 GetViewMatrix() const;
		[[nodiscard]] Mat4 GetProjectionMatrix(const Real width, const Real height, const Real near = 0.1, const Real far = 1000) const;
		[[nodiscard]] Mat4 GetViewProjectionMatrix(const Real width, const Real height, const Real near = 0.1, const Real far = 1000) const {return GetProjectionMatrix(width, height, near, far) * GetViewMatrix();}

		[[nodiscard]] Vec3 CalculateForward() const;
		[[nodiscard]] Vec3 CalculateUp() const;
		[[nodiscard]] Vec3 CalculateRight() const;
	public:
		Vec3 Position = {0,0,0};
		Quat Rotation = Math::Identity<Quat>();
		Real Fov = 90;
	};
} // Application
// TRG
