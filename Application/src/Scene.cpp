//
// Created by ianpo on 30/01/2025.
//

#include "Scene.hpp"

#include <imgui.h>
#include <rlImGui.h>


namespace TRG::Application {

	Scene::Scene() {
	}

	Scene::~Scene() {
		// De-Initialization
		//--------------------------------------------------------------------------------------
		UnloadTexture(background); // Unload background texture
		UnloadTexture(midground); // Unload midground texture
		UnloadTexture(foreground); // Unload foreground texture
	}

	Scene::Scene(Scene&& scene) noexcept {
		swap(scene);
	}
	Scene& Scene::operator=(Scene&& scene) noexcept {
		swap(scene);
		return *this;
	}

	void Scene::Init() {
		background = LoadTexture("Assets/cyberpunk_street_background.png");
		midground = LoadTexture("Assets/cyberpunk_street_midground.png");
		foreground = LoadTexture("Assets/cyberpunk_street_foreground.png");
	}

	void Scene::Update(const float ts) {
		// Update
		//----------------------------------------------------------------------------------
		scrollingBack -= 0.1f;
		scrollingMid -= 0.5f;
		scrollingFore -= 1.0f;

		// NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
		if (scrollingBack <= -background.width * 2) scrollingBack = 0;
		if (scrollingMid <= -midground.width * 2) scrollingMid = 0;
		if (scrollingFore <= -foreground.width * 2) scrollingFore = 0;
		//----------------------------------------------------------------------------------
	}

	void Scene::Render(const float ts) {

			// Draw background image twice
			// NOTE: Texture is scaled twice its size
			DrawTextureEx(background, Vector2{scrollingBack, 20}, 0.0f, 2.0f, WHITE);
			DrawTextureEx(background, Vector2{background.width * 2 + scrollingBack, 20}, 0.0f, 2.0f, WHITE);

			// Draw midground image twice
			DrawTextureEx(midground, Vector2{scrollingMid, 20}, 0.0f, 2.0f, WHITE);
			DrawTextureEx(midground, Vector2{midground.width * 2 + scrollingMid, 20}, 0.0f, 2.0f, WHITE);

			// Draw foreground image twice
			DrawTextureEx(foreground, Vector2{scrollingFore, 70}, 0.0f, 2.0f, WHITE);
			DrawTextureEx(foreground, Vector2{foreground.width * 2 + scrollingFore, 70}, 0.0f, 2.0f, WHITE);


			{
			}
		//----------------------------------------------------------------------------------
	}

	void Scene::RenderGui(const float ts) {

		DrawText("BACKGROUND SCROLLING & PARALLAX", 10, 10, 20, RED);
		DrawText("(c) Cyberpunk Street Environment by Luis Zuno (@ansimuz)", m_Width - 330, m_Height - 20, 10,
				 RAYWHITE);
	}

	void Scene::RenderImGui(const float ts) {
		static bool showDemo = true;
		if (showDemo) ImGui::ShowDemoWindow(&showDemo);
	}

	Camera3D Scene::GetCamera3D() const {
		return Camera3D {
			Vector3 {0,0,0},
			Vector3 {0,0,1},
			Vector3 {0,1,0},
			float{60},
			CAMERA_PERSPECTIVE,
		};
	}

	void Scene::SetWindow(const uint32_t width, const uint32_t height) {
		m_Width = width;
		m_Height = height;
	}

	void Scene::swap(Scene &other) noexcept {
		std::swap(background, other.background);
		std::swap(midground, other.midground);
		std::swap(foreground, other.foreground);

		std::swap(m_Width, other.m_Width);
		std::swap(m_Height, other.m_Height);

		std::swap(scrollingBack, other.scrollingBack);
		std::swap(scrollingMid, other.scrollingMid);
		std::swap(scrollingFore, other.scrollingFore);
	}
} // TRG::Application
