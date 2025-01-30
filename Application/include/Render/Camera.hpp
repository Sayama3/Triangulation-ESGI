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
	public:
		Vec3 Position = {0,0,0};
		Quat Rotation = Math::Identity<Quat>();
		Real Fov = 90;
	};
} // Application
// TRG
