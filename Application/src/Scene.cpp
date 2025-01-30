//
// Created by ianpo on 30/01/2025.
//

#include "Scene.hpp"
#include "ImGuiLib.hpp"

#include <imgui.h>
#include <rlImGui.h>
#include <Core/Reinterpreter.hpp>


namespace TRG::Application {

	using namespace TRG::Literal;

	Scene::Scene() {
	}

	Scene::~Scene() {
	}

	Scene::Scene(Scene&& scene) noexcept {
		swap(scene);
	}
	Scene& Scene::operator=(Scene&& scene) noexcept {
		swap(scene);
		return *this;
	}

	void Scene::Init() {
		m_Camera.InternalCamera.Position = {0,2,0};
		m_Camera.InternalCamera.Rotation = Math::LookAt(Math::Normalize(Vec3{0,0, 1}), Vec3{0,1,0});
	}

	void Scene::Update(const float ts) {

		m_IsInFps = IsMouseButtonDown(m_EnterFpsKey);

		if (IsMouseButtonPressed(m_EnterFpsKey)) {
			DisableCursor();
		} else if (IsMouseButtonReleased(m_EnterFpsKey)) {
			EnableCursor();
		}

		if (m_IsInFps) {
			m_Camera.MoveCamera(ts, GetKeyboardMovement());
			m_Camera.RotateCamera(GetMouseRotation());
		}
	}

	void Scene::Render(const float ts) {

	}

	void Scene::RenderGui(const float ts) {

	}

	void Scene::RenderImGuiCamera() {
		ImGui::SetWindowSize(ImVec2{450, 250}, ImGuiCond_FirstUseEver);
		ImGui::Begin("Camera");
		{
			ImGuiLib::DragReal3("Position", &m_Camera.InternalCamera.Position.x, 0.01, 0,0,"%.2f");
			ImGuiLib::DragQuat("Rotation", &m_Camera.InternalCamera.Rotation, 0.001, 0,0,"%.3f");
			ImGuiLib::DragReal("FOV Y", &m_Camera.InternalCamera.Fov, 0.1, 0,0,"%.1f");
			ImGuiLib::DragReal("Sensitivity X", &m_Camera.SensitivityX, 0.0001, 0, REAL_MAX, "%.4f");
			ImGuiLib::DragReal("Sensitivity Y", &m_Camera.SensitivityY, 0.0001, 0, REAL_MAX, "%.4f");
			ImGui::Checkbox("Inverse X", &m_Camera.InverseX);
			ImGui::Checkbox("Inverse Y", &m_Camera.InverseY);
			ImGui::Spacing();
			if (ImGui::Button("Edit Inputs")) {
				m_EditInputs = true;
			}
		}
		ImGui::End();
	}

	void Scene::RenderImGuiCameraInputs() {
		if (m_EditInputs) {
			ImGui::SetWindowSize(ImVec2{400, 300}, ImGuiCond_FirstUseEver);
			ImGui::Begin("Inputs", &m_EditInputs);
			{
				ImGui::BeginDisabled(m_IsInFps);
				ImGuiLib::ComboMouseButton("Enter FpsKey", &m_EnterFpsKey);
				ImGui::Spacing();
				ImGuiLib::ComboKeyboardKey("Forward Key", &m_ForwardKey);
				ImGuiLib::ComboKeyboardKey("Backward Key", &m_BackwardKey);
				ImGuiLib::ComboKeyboardKey("Right Key", &m_RightKey);
				ImGuiLib::ComboKeyboardKey("Left Key", &m_LeftKey);
				ImGuiLib::ComboKeyboardKey("Up Key", &m_UpKey);
				ImGuiLib::ComboKeyboardKey("Down Key", &m_DownKey);
				ImGui::Spacing();
				ImGuiLib::ComboKeyboardKey("Rotate Right Key", &m_RotateRightKey);
				ImGuiLib::ComboKeyboardKey("Rotate Left Key", &m_RotateLeftKey);
				ImGuiLib::ComboKeyboardKey("Rotate Up Key", &m_RotateUpKey);
				ImGuiLib::ComboKeyboardKey("Rotate Down Key", &m_RotateDownKey);
				ImGui::EndDisabled();
			}
			ImGui::End();
		}
	}

	void Scene::RenderImGui(const float ts) {

		RenderImGuiCamera();
		RenderImGuiCameraInputs();
	}

	Camera3D Scene::GetCamera3D() const {
		return m_Camera.GetCamera3D();
	}

	void Scene::swap(Scene &other) noexcept {
		std::swap(m_Camera, other.m_Camera);
	}

	Vec3 Scene::GetKeyboardMovement() {

		Vec3 movement{0};

		if (IsKeyDown(m_ForwardKey)) {
			movement.z += 1;
		}
		if (IsKeyDown(m_BackwardKey)) {
			movement.z -= 1;
		}

		if (IsKeyDown(m_RightKey)) {
			movement.x += 1;
		}
		if (IsKeyDown(m_LeftKey)) {
			movement.x -= 1;
		}

		if (IsKeyDown(m_UpKey)) {
			movement.y += 1;
		}
		if (IsKeyDown(m_DownKey)) {
			movement.y -= 1;
		}

		return movement;
	}

	Vec2 Scene::GetKeyboardRotation() {
		Vec2 rotation{0};

		if (IsKeyDown(m_RotateRightKey)) {
			rotation.x += 1;
		}
		if (IsKeyDown(m_RotateLeftKey)) {
			rotation.x -= 1;
		}

		if (IsKeyDown(m_RotateUpKey)) {
			rotation.y += 1;
		}
		if (IsKeyDown(m_RotateDownKey)) {
			rotation.y -= 1;
		}

		return rotation;
	}

	Vec2 Scene::GetMouseRotation() {
		return reinterpret<Vec2>(GetMouseDelta());
	}
} // TRG::Application
