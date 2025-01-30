//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "Render/Renderable.hpp"
#include "Render/EditorCamera.hpp"
#include <raylib.h>


namespace TRG::Application {

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

		void RenderImGuiCamera();

		void RenderImGuiCameraInputs();

		void RenderImGui(float ts) override;
		[[nodiscard]] Camera3D GetCamera3D() const;
	public:
		void swap(Scene& other) noexcept;

	private:
		[[nodiscard]] Vec3 GetKeyboardMovement();
		[[nodiscard]] Vec2 GetKeyboardRotation();
		[[nodiscard]] Vec2 GetMouseRotation();
	private:
		EditorCamera m_Camera;
	public:
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
		bool m_IsInFps = false;
		bool m_EditInputs = false;
	};

} // TRG::Application
