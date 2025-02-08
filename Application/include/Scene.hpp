//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "Render/Renderable.hpp"
#include "Render/EditorCamera.hpp"
#include "TRG/Math/Mesh.hpp"
#include <raylib.h>


namespace TRG::Application {

	enum class Action {
		None,
		AddPoint,
		AddTriangulatePoint,
		FreeCam
	};

	class Scene final : public Renderable {
	public:
		Scene();
		~Scene() override;

		Scene(Scene&& scene) noexcept;
		Scene& operator=(Scene&& scene) noexcept;

		Scene(const Scene& scene) = delete;
		Scene& operator=(const Scene& scene) = delete;
	public:
		void Init();
	public:
		void Update(float ts) override;
		void Render(float ts) override;
		void RenderGui(float ts) override;

		void RenderImGuiPoints();
		void RenderImGuiCamera();
		void RenderImGuiCameraInputs();
		void RenderImGuiJarvisShell();
		void RenderImGuiGrahamScanShell();
		void RenderImGuiMeshGraph();

		void RenderImGui(float ts) override;
		[[nodiscard]] Camera3D GetCamera3D() const;
		[[nodiscard]] const Camera& GetCamera() const;
	public:
		void swap(Scene& other) noexcept;

	private:
		[[nodiscard]] Vec3 GetKeyboardMovement();
		[[nodiscard]] Vec2 GetKeyboardRotation();
		[[nodiscard]] Vec2 GetMouseRotation();
	private:
		void OnEnterFreeCam(float ts);
		void OnFreeCam(float ts);
		void OnExitFreeCam(float ts);

		Vec3 GetMouseToWorldPos() const;
		Vec3 GetScreenToWorldPos(Vec2 screenPos) const;

		Ray3 GetMouseToWorldRay() const;
		Ray3 GetScreenToWorldRay(Vec2 screenPos) const;

		void TryUpdateInvViewProjMatrix(uint32_t newScreenWidth, uint32_t newScreenHeight);

	private:
		void BeginAddPoint(float ts);
		void OnAddingPoint(float ts);
		std::optional<Vec2> EndAddPoint(float ts);

		void UpdatePointToAdd();

	private:
		EditorCamera m_Camera;
		Model m_Model;
		Mat4 InvViewProjMatrix;
		std::optional<Vec3> PointToAdd;
		Real m_ScreenWidth;
		Real m_ScreenHeight;
		bool InvViewProjMatrixDirty = true;

	private:
		std::vector<Vec2> m_2DPoints;
		Math::MeshGraph m_MeshGraph;
	private:
		std::vector<Vec2> m_JarvisShell;
		std::vector<Vec2> m_GrahamScanShell;
	public:
		MouseButton m_AddPoint = MOUSE_BUTTON_MIDDLE;
		MouseButton m_AddTriangulationPoint = MOUSE_BUTTON_SIDE;
		MouseButton m_EnterFpsKey = MOUSE_BUTTON_RIGHT;

		KeyboardKey m_ForwardKey = KeyboardKey::KEY_W;
		KeyboardKey m_BackwardKey = KeyboardKey::KEY_S;
		KeyboardKey m_RightKey = KeyboardKey::KEY_A;
		KeyboardKey m_LeftKey = KeyboardKey::KEY_D;
		KeyboardKey m_UpKey = KeyboardKey::KEY_E;
		KeyboardKey m_DownKey = KeyboardKey::KEY_Q;
		KeyboardKey m_RotateRightKey = KeyboardKey::KEY_RIGHT;
		KeyboardKey m_RotateLeftKey = KeyboardKey::KEY_LEFT;
		KeyboardKey m_RotateUpKey = KeyboardKey::KEY_UP;
		KeyboardKey m_RotateDownKey = KeyboardKey::KEY_DOWN;
	private:
		Action m_Action = Action::None;
		bool m_EditInputs = false;
	};

} // TRG::Application
