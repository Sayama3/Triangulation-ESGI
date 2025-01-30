//
// Created by ianpo on 30/01/2025.
//

#include "Render/Camera.hpp"

#include "Core/Reinterpreter.hpp"


namespace TRG::Application {
	Camera3D Camera::GetCamera3D() const {
		return Camera3D {
			reinterpret<Vector3>(Position),
			reinterpret<Vector3>(Position + CalculateForward()),
			reinterpret<Vector3>(CalculateUp()),
			Fov,
			CAMERA_PERSPECTIVE,
		};
	}

	Mat4 Camera::GetViewMatrix() const {
		return glm::inverse(glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(Rotation));
	}

	Mat4 Camera::GetProjectionMatrix(const Real width, const Real height, const Real near, const Real far) const {
		return glm::perspectiveFov(Fov * deg2rad, width, height, near, far);
	}

	Vec3 Camera::CalculateForward() const {
		return Rotation * Vec3(0,0,1);
	}

	Vec3 Camera::CalculateUp() const {
		return Rotation * Vec3(0,1,0);
	}

	Vec3 Camera::CalculateRight() const {
		return Rotation * Vec3(1,0,0);
	}
} // Application
// TRG
