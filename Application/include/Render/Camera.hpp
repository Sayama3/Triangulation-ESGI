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
