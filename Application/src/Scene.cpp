//
// Created by ianpo on 30/01/2025.
//

#include "Scene.hpp"
#include "ImGuiLib.hpp"

#include <imgui.h>
#include <rcamera.h>
#include <rlImGui.h>
#include <Core/Reinterpreter.hpp>

#include "Application.hpp"
#include "Core/Singleton.hpp"


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
		m_Camera.InternalCamera.Rotation = Math::MakeQuat(45*deg2rad, {1,0,0});
	}

	void Scene::Update(const float ts) {

		if (m_Action == Action::None) {
			if (IsMouseButtonPressed(m_EnterFpsKey)) {
				m_Action = Action::FreeCam;
				OnEnterFreeCam(ts);
			}
			else if (IsMouseButtonPressed(m_AddPoint)) {
				m_Action = Action::AddPoint;
				BeginAddPoint(ts);
			}
		}

		if (m_Action == Action::FreeCam && IsMouseButtonReleased(m_EnterFpsKey)) {
			OnExitFreeCam(ts);
			m_Action = Action::None;
		}
		else if (m_Action == Action::AddPoint && IsMouseButtonReleased(m_AddPoint)) {
			EndAddPoint(ts);
			m_Action = Action::None;
		}

		switch (m_Action) {
			case Action::None:
				break;
			case Action::FreeCam : {
				OnFreeCam(ts);
				break;
			}
			case Action::AddPoint : {
				OnAddingPoint(ts);
				break;
			}
		}
	}

	void Scene::Render(const float ts) {
		if (m_Action == Action::AddPoint && PointToAdd.has_value()) {
			DrawSphere(reinterpret<Vector3>(PointToAdd.value()), 0.1, Color{180, 50, 40, 150});
			TraceLog(TraceLogLevel::LOG_INFO, "Position (%f, %f, %f)", PointToAdd.value().x, PointToAdd.value().y, PointToAdd.value().z);
		}

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
				ImGui::BeginDisabled(m_Action != Action::None);
				ImGuiLib::ComboMouseButton("Add Point Key", &m_AddPoint);
				ImGuiLib::ComboMouseButton("FreeCam Key", &m_EnterFpsKey);
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

	void Scene::OnEnterFreeCam(float ts) {
		DisableCursor();
		InvViewProjMatrixDirty = true;
	}

	void Scene::OnFreeCam(float ts) {
		m_Camera.MoveCamera(ts, GetKeyboardMovement());
		m_Camera.RotateCamera(GetMouseRotation());
	}

	void Scene::OnExitFreeCam(float ts) {
		EnableCursor();
	}

	Vec3 Scene::GetMouseToWorldPos() const {
		const auto mousePos = reinterpret<Vec2>(GetMousePosition());
		return GetScreenToWorldPos(mousePos);
	}

	Ray3 Scene::GetMouseToWorldRay() const {
		const auto mousePos = reinterpret<Vec2>(GetMousePosition());
		return GetScreenToWorldRay(mousePos);
	}

	Vec3 Scene::GetScreenToWorldPos(const Vec2 screenPos) const {
		const auto normalizeMousePos = Vec2{(screenPos.x  / m_ScreenWidth) * 2_r - 1_r, (screenPos.y  / m_ScreenHeight) * 2_r - 1_r};
		const Vec4 result = InvViewProjMatrix * Vec4{normalizeMousePos.x, normalizeMousePos.y, 6, 1};
		const Vec4 resultNormalize = result / result.w;
		return resultNormalize;
	}

	Ray3 Scene::GetScreenToWorldRay(const Vec2 screenPos) const {
		const auto worldScreenPos = GetScreenToWorldPos(screenPos);
		const auto camPos = m_Camera.InternalCamera.Position;
		return Ray3{camPos, Math::Normalize(worldScreenPos - camPos)};
	}

	void Scene::TryUpdateInvViewProjMatrix(const uint32_t newScreenWidth, const uint32_t newScreenHeight) {
		if (InvViewProjMatrixDirty || newScreenWidth != m_ScreenWidth || newScreenHeight != m_ScreenHeight) {
			m_ScreenWidth = newScreenWidth;
			m_ScreenHeight = newScreenHeight;
			InvViewProjMatrixDirty = false;

			const Mat4 viewMat  = m_Camera.InternalCamera.GetViewMatrix();
			const Mat4 projMat  = m_Camera.InternalCamera.GetProjectionMatrix(m_ScreenWidth, m_ScreenHeight);

			const auto viewProj = projMat * viewMat;
			InvViewProjMatrix = glm::inverse(viewProj);
		}
	}

	void Scene::BeginAddPoint(float ts) {
		UpdatePointToAdd();
	}

	void Scene::OnAddingPoint(float ts) {
		UpdatePointToAdd();
	}

	void Scene::EndAddPoint(float ts) {
		PointToAdd.reset();
	}

	void Scene::UpdatePointToAdd() {
		const auto& app = SingletonApp::Get();

		TryUpdateInvViewProjMatrix(app.GetWidth(), app.GetHeight());
		const auto ray = GetMouseToWorldRay();
		const Plane ground {{0,0,0}, {0,1,0}};
		const auto t = Math::Raycast(ground, ray);
		if (t && t.value() > REAL_EPSILON) {
			PointToAdd = ray.GetPoint(t.value());
		} else {
			PointToAdd = std::nullopt;
		}
	}

} // TRG::Application
