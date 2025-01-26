//
// Created by ianpo on 26/01/2025.
//

#include "Application.hpp"
#include "TRG/Math.hpp"

#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>

using namespace TRG::Math::Literal;

namespace TRG::Application {
	Application::Application(int width, int height, const std::string &name) {
		SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Window configuration flags
		InitWindow(width, height, name.c_str());
		rlImGuiSetup(true); // sets up ImGui with either a dark or light default theme
	}

	Application::~Application() {
		rlImGuiShutdown(); // cleans up ImGui
		CloseWindow(); // Close window and OpenGL context
	}

	void Application::Run() {
		// NOTE: Be careful, background width must be equal or bigger than screen width
		// if not, texture should be draw more than two times for scrolling effect
		Texture2D background = LoadTexture("Assets/cyberpunk_street_background.png");
		Texture2D midground = LoadTexture("Assets/cyberpunk_street_midground.png");
		Texture2D foreground = LoadTexture("Assets/cyberpunk_street_foreground.png");

		float scrollingBack = 0.0f;
		float scrollingMid = 0.0f;
		float scrollingFore = 0.0f;

		SetTargetFPS(60); // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------
		while (!WindowShouldClose()) // Detect window close button or ESC key
		{
			m_Width = GetScreenWidth();
			m_Heigh = GetScreenHeight();

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

			// Draw
			//----------------------------------------------------------------------------------
			BeginDrawing();

			{
				ClearBackground(GetColor(0x052c46ff));

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

				DrawText("BACKGROUND SCROLLING & PARALLAX", 10, 10, 20, RED);
				DrawText("(c) Cyberpunk Street Environment by Luis Zuno (@ansimuz)", m_Width - 330, m_Heigh - 20, 10,
				         RAYWHITE);


				rlImGuiBegin(); // starts the ImGui content mode. Make all ImGui calls after this
				{
					static bool showDemo = true;
					if (showDemo) ImGui::ShowDemoWindow(&showDemo);
				}
				rlImGuiEnd(); // ends the ImGui content mode. Make all ImGui calls before this}
			}

			EndDrawing();
			//----------------------------------------------------------------------------------
		}


		// De-Initialization
		//--------------------------------------------------------------------------------------
		UnloadTexture(background); // Unload background texture
		UnloadTexture(midground); // Unload midground texture
		UnloadTexture(foreground); // Unload foreground texture
	}
} // TRG::Application
