//
// Created by ianpo on 30/01/2025.
//

#include "Scene.hpp"
#include "ImGuiLib.hpp"

#include <imgui.h>
#include <rcamera.h>
#include <rlImGui.h>
#include <rlgl.h>
#include <Core/Reinterpreter.hpp>

#include "Application.hpp"
#include "Core/Singleton.hpp"


namespace TRG::Application {

	using namespace TRG::Literal;

	Scene::Scene() {
	}

	Scene::~Scene() {
		if (IsModelValid(m_Model))
			UnloadModel(m_Model);
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
		m_Camera.InternalCamera.Rotation = Math::MakeQuat(-45*deg2rad, {1,0,0});
		m_2DPoints.reserve(1000);
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
			else if (IsMouseButtonPressed(m_AddTriangulationPoint)) {
				m_Action = Action::AddTriangulatePoint;
				BeginAddPoint(ts);
			}
		}

		if (m_Action == Action::FreeCam && IsMouseButtonReleased(m_EnterFpsKey)) {
			OnExitFreeCam(ts);
			m_Action = Action::None;
		}
		else if (m_Action == Action::AddPoint && IsMouseButtonReleased(m_AddPoint)) {
			if (const auto vec2 = EndAddPoint(ts)) {
				m_2DPoints.push_back(vec2.value());
			}
			m_Action = Action::None;
		}
		else if (m_Action == Action::AddTriangulatePoint && IsMouseButtonReleased(m_AddTriangulationPoint)) {
			if (const auto vec2 = EndAddPoint(ts)) {
				m_MeshGraph.AddPoint(vec2.value());
			}
			m_Action = Action::None;
		}

		switch (m_Action) {
			case Action::None:
				break;
			case Action::FreeCam : {
				OnFreeCam(ts);
				break;
			}
			case Action::AddTriangulatePoint :
			case Action::AddPoint : {
				OnAddingPoint(ts);
				break;
			}
		}
	}

	void Scene::Render(const float ts) {
		if (IsModelValid(m_Model)) {
			DrawModel(m_Model, Vector3(0,0,0), 1.0, Color(180, 180, 180, 255));
			DrawModelWires(m_Model, Vector3(0,0,0), 1.0, Color(80, 80, 80, 255));
		}
		constexpr auto color = Color{ 50, 180, 40, 255};
		for (uint64_t i = 0; i < m_2DPoints.size(); ++i) {
			const auto& current = m_2DPoints[i];
			const auto& next = m_2DPoints[(i + 1) % m_2DPoints.size()];
			DrawSphere(Vector3(current.x, 0, current.y), 0.01, color);
			DrawLine3D(Vector3(current.x, 0, current.y), Vector3(next.x, 0, next.y), color);
		}

		constexpr auto colorMeshGraph = Color{ 150, 180, 40, 255};
		for (const auto& [vert_id, vert] : m_MeshGraph.m_Vertices) {
			DrawSphere(Vector3(vert.Position.x, 0.001, vert.Position.y), 0.01, { 150, 180, 40, 255});
		}
		for (const auto& [id, tr] : m_MeshGraph.m_Triangles) {
			const auto& edgeAB = m_MeshGraph.m_Edges.at(tr.EdgeAB);
			const auto& edgeBC = m_MeshGraph.m_Edges.at(tr.EdgeBC);
			// const auto& edgeAC = m_MeshGraph.m_Edges.at(tr.EdgeC);

			const auto& vertA = m_MeshGraph.m_Vertices.at(edgeAB.VertexA);
			const auto& vertB = m_MeshGraph.m_Vertices.at(edgeAB.VertexB);
			const auto& vertC = m_MeshGraph.m_Vertices.at((edgeBC.VertexA != edgeAB.VertexA) && (edgeBC.VertexA != edgeAB.VertexB) ? edgeBC.VertexA : edgeBC.VertexB);
			if (edgeAB.VertexB == edgeBC.VertexA) {
				DrawTriangle3D(Vector3(vertA.Position.x, 0.001, vertA.Position.y),Vector3(vertC.Position.x, 0.001, vertC.Position.y), Vector3(vertB.Position.x, 0.001, vertB.Position.y), { 187, 225, 50, 255});
			} else {
				DrawTriangle3D(Vector3(vertA.Position.x, 0.001, vertA.Position.y),Vector3(vertB.Position.x, 0.001, vertB.Position.y),Vector3(vertC.Position.x, 0.001, vertC.Position.y), { 187, 225, 50, 255});
			}
		}
		for (const auto& [id, edge] : m_MeshGraph.m_Edges) {
			const auto& vertA = m_MeshGraph.m_Vertices.at(edge.VertexA);
			const auto& vertB = m_MeshGraph.m_Vertices.at(edge.VertexB);
			// void DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); // Draw a cylinder/cone
			DrawLine3D(Vector3(vertA.Position.x, 0.001, vertA.Position.y), Vector3(vertB.Position.x, 0.001, vertB.Position.y), { 75, 90, 20, 255});
		}

		constexpr auto jarvisColor = Color{ 50, 40, 180, 255};
		for (uint64_t i = 0; i < m_JarvisShell.size(); ++i) {
			const auto& current = m_JarvisShell[i];
			const auto& next = m_JarvisShell[(i + 1) % m_JarvisShell.size()];
			DrawSphere(Vector3(current.x, 0, current.y), 0.01, jarvisColor);
			DrawLine3D(Vector3(current.x, 0, current.y), Vector3(next.x, 0, next.y), jarvisColor);
		}

		constexpr auto GrahamScanColor = Color{ 180, 20, 160, 255};
		for (uint64_t i = 0; i < m_GrahamScanShell.size(); ++i) {
			const auto& current = m_GrahamScanShell[i];
			const auto& next = m_GrahamScanShell[(i + 1) % m_GrahamScanShell.size()];
			DrawSphere(Vector3(current.x, 0, current.y), 0.01, GrahamScanColor);
			DrawLine3D(Vector3(current.x, 0, current.y), Vector3(next.x, 0, next.y), GrahamScanColor);
		}

		if (m_Action == Action::AddPoint && PointToAdd.has_value()) {
			DrawSphere(reinterpret<Vector3>(PointToAdd.value()), 0.01, Color{180, 50, 40, 150});
		}

		if (m_Action == Action::AddTriangulatePoint && PointToAdd.has_value()) {
			DrawSphere(reinterpret<Vector3>(PointToAdd.value()), 0.01, Color{150, 180, 40, 150});
		}

	}

	void Scene::RenderGui(const float ts) {

	}

	void Scene::RenderImGui(const float ts) {
		RenderImGuiPoints();
		RenderImGuiCamera();
		RenderImGuiCameraInputs();
		RenderImGuiJarvisShell();
		RenderImGuiGrahamScanShell();
	}

	void Scene::RenderImGuiPoints() {
		ImGui::SetWindowSize(ImVec2{400, 300}, ImGuiCond_FirstUseEver);
		std::vector<uint64_t> toDelete;
		ImGui::Begin("Points");
		{
			if (ImGui::Button("Delete All")) {
				m_2DPoints.clear();
			}
			ImGui::SameLine(0, 1);
			if (ImGui::Button("Sort")) {
				const Vec2 center = Math::CalculateCenter<std::vector<Vec2>::iterator, 2>(m_2DPoints.begin(), m_2DPoints.end());
				std::sort(m_2DPoints.begin(), m_2DPoints.end(), [center](const Vec2& a, const Vec2& b) {
					const auto cToA = a - center;
					const auto cToB = b - center;
					const auto angleA = Math::SignedAngle({1,0}, cToA);
					const auto angleB = Math::SignedAngle({1,0}, cToB);
					if (angleA == angleB) {
						return Math::Magnitude(cToA) < Math::Magnitude(cToB);
					}
					return angleA < angleB;
				});
			}

			if (ImGui::Button("Make Jarvis Shell")) {
				m_JarvisShell = Math::JarvisConvexShell(m_2DPoints.begin(), m_2DPoints.end());
			}

			if (ImGui::Button("Make Graham Scan Shell")) {
				 auto list = Math::GrahamScanConvexShell(m_2DPoints.begin(), m_2DPoints.end());
				m_GrahamScanShell = std::vector<Vec2>(list.begin(), list.end());
			}

			if (ImGui::Button("Incremental Triangulation")) {
				const auto vertices = Math::IncrementalTriangulation(m_2DPoints.cbegin(), m_2DPoints.cend(), 0.001);
				Mesh mesh{};
				mesh.vertexCount = vertices.size();
				mesh.triangleCount = vertices.size() / 3;
				static_assert(sizeof(float) == sizeof(Real));
				mesh.vertices = static_cast<float *>(malloc(sizeof(float) * 3 * vertices.size()));
				std::memcpy(mesh.vertices, vertices.data(), sizeof(float) * 3 * vertices.size());
				UploadMesh(&mesh, false);
				m_Model = LoadModelFromMesh(mesh);
			}

			if (ImGui::Button("Optimize Pre-Mesh")) {
				m_MeshGraph.DelaunayTriangulation();
			}

			for (uint64_t i = 0; i < m_2DPoints.size(); ++i) {
				ImGui::PushID(i);
				std::string name = "Point " + std::to_string(i);
				ImGuiLib::DragReal2(name.c_str(), &m_2DPoints[i].x, 0.01, 0,0, "%.2f");
				ImGui::SameLine();
				if (ImGui::Button("Delete")) {
					toDelete.push_back(i);
				}
				ImGui::PopID();
			}
		}
		ImGui::End();

		for (const auto to_delete: toDelete) {
			m_2DPoints.erase(m_2DPoints.begin() + to_delete);
		}
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
				ImGuiLib::ComboMouseButton("Add Triangulation Key", &m_AddTriangulationPoint);
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

	void Scene::RenderImGuiJarvisShell() {
		bool open = !m_JarvisShell.empty();
		if (open) {
			ImGui::SetWindowSize(ImVec2{400, 300}, ImGuiCond_FirstUseEver);
			ImGui::Begin("Jarvis Shell", &open);
			{
				for (uint64_t i = 0; i < m_JarvisShell.size(); ++i) {
					ImGui::PushID(i);
					std::string name = "Point " + std::to_string(i);
					ImGuiLib::DragReal2(name.c_str(), &m_JarvisShell[i].x, 0.01, 0,0, "%.2f");
					ImGui::PopID();
				}
			}
			ImGui::End();
			if (!open) m_JarvisShell.clear();
		}
	}
	void Scene::RenderImGuiGrahamScanShell() {
		bool open = !m_GrahamScanShell.empty();
		if (open) {
			ImGui::SetWindowSize(ImVec2{400, 300}, ImGuiCond_FirstUseEver);
			ImGui::Begin("Graham Scan Shell", &open);
			{
				for (uint64_t i = 0; i < m_GrahamScanShell.size(); ++i) {
					ImGui::PushID(i);
					std::string name = "Point " + std::to_string(i);
					ImGuiLib::DragReal2(name.c_str(), &m_GrahamScanShell[i].x, 0.01, 0,0, "%.2f");
					ImGui::PopID();
				}
			}
			ImGui::End();
			if (!open) m_GrahamScanShell.clear();
		}
	}

	Camera3D Scene::GetCamera3D() const {
		return m_Camera.GetCamera3D();
	}

	const Camera& Scene::GetCamera() const {
		return m_Camera.InternalCamera;
	}

	void Scene::swap(Scene &other) noexcept {
		std::swap(m_Camera, other.m_Camera);
	}

	Vec3 Scene::GetKeyboardMovement() {

		Vec3 movement{0};

		if (IsKeyDown(m_ForwardKey)) {
			movement.z -= 1;
		}
		if (IsKeyDown(m_BackwardKey)) {
			movement.z += 1;
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
		const Vec2 normalizeMousePos = Vec2{(screenPos.x  / m_ScreenWidth) * 2_r - 1_r, (1_r-(screenPos.y  / m_ScreenHeight)) * 2_r - 1_r};
		const Vec4 result = InvViewProjMatrix * Vec4{normalizeMousePos.x, normalizeMousePos.y, -1, 1};
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

	std::optional<Vec2> Scene::EndAddPoint(float ts) {
		if (!PointToAdd) return std::nullopt;
		return Vec2{PointToAdd.value().x, PointToAdd.value().z};
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
