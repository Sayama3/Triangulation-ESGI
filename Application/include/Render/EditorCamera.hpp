//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "Camera.hpp"

namespace TRG::Application {

	class EditorCamera {
	public:
		EditorCamera() = default;
		~EditorCamera() = default;
	public:
		void MoveCamera(float ts, Vec3 localMovement);
		void RotateCamera(Vec2 localMovement);

		[[nodiscard]] Camera3D GetCamera3D() const;
	public:
		TRG::Application::Camera InternalCamera;
		Real MovementSpeed = 5;
		Real SensitivityX = 0.001;
		Real SensitivityY = 0.001;
		bool InverseX = false;
		bool InverseY = false;
	};
}
