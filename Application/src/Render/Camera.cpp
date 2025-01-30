//
// Created by ianpo on 30/01/2025.
//

#include "Render/Camera.hpp"
#include "Core/Reinterpreter.hpp"


namespace TRG::Application {
	Camera3D Camera::GetCamera3D() const {
		return Camera3D {
			reinterpret<Vector3>(Position),
			reinterpret<Vector3>(Position + Rotation * Vec3(0,0,1)),
			reinterpret<Vector3>(Rotation * Vec3(0,1,0)),
			Fov,
			CAMERA_PERSPECTIVE,
		};
	}
} // Application
// TRG
