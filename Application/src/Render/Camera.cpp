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
