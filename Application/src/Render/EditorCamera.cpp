//
// Created by ianpo on 30/01/2025.
//

#include "Render/EditorCamera.hpp"

using namespace TRG::Literal;

namespace TRG::Application {
	void EditorCamera::MoveCamera(const float ts, Vec3 localMovement) {
		if (Math::Magnitude2(localMovement) < REAL_EPSILON) return;

		Math::NormalizeInPlace(localMovement);
		const Vec3 movementXZ = {localMovement.x, 0, localMovement.z};
		const Vec3 movementY = {0,localMovement.y,0};

		InternalCamera.Position += ((InternalCamera.Rotation * movementXZ) + movementY) * (ts * MovementSpeed);
	}

	void EditorCamera::RotateCamera(const Vec2 localMovement) {
		if (Math::Magnitude2(localMovement) < REAL_EPSILON) return;

		const auto rotateX = localMovement.x*SensitivityX;
		const auto rotateY = localMovement.y*SensitivityY;

		const auto axeY = (InverseY ? -1_r : 1_r);
		const auto axeX = (InverseX ? 1_r : -1_r);

		const auto quatX = Math::MakeQuat(rotateX, {0,axeX,0});
		const auto quatY = Math::MakeQuat(rotateY, {axeY,0,0});

		InternalCamera.Rotation = quatX * InternalCamera.Rotation * quatY;
		Math::NormalizeInPlace(InternalCamera.Rotation);

	}

	Camera3D EditorCamera::GetCamera3D() const {
		return InternalCamera.GetCamera3D();
	}
}
