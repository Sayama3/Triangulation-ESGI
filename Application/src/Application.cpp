//
// Created by ianpo on 26/01/2025.
//

#include "Application.hpp"
#include "TRG/Math.hpp"

#include <raylib.h>
#include <rlgl.h>
#include <imgui.h>
#include <rlImGui.h>

#include "Core/raylibMathHelper.hpp"

using namespace TRG::Literal;

namespace TRG::Application {
	Application::Application(int width, int height, const std::string &name): m_Width(width), m_Height(height) {
		SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Window configuration flags
		InitWindow(width, height, name.c_str());
		rlImGuiSetup(true); // sets up ImGui with either a dark or light default theme
		m_Scene.Init();
	}

	Application::~Application() {
		rlImGuiShutdown(); // cleans up ImGui
		CloseWindow(); // Close window and OpenGL context
	}

	Application::Application(Application && other) noexcept {
		swap(other);
	}

	Application & Application::operator=(Application && other) noexcept {
		swap(other);
		return *this;
	}

	void Application::Run() {

		SetTargetFPS(60); // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------
		while (!WindowShouldClose()) // Detect window close button or ESC key
		{
			m_Width = GetScreenWidth();
			m_Height = GetScreenHeight();

			const float ts = GetFrameTime();

			Update(ts);

			BeginDrawing();
			{
				ClearBackground(GetColor(0x052c46ff));
				CustomBegin3D(m_Scene.GetCamera());
				{
					DrawGrid(20, 1);
					RenderScene(ts);
				}
				EndMode3D();

				RenderGui(ts);

				rlImGuiBegin(); // starts the ImGui content mode. Make all ImGui calls after this
				{
					static bool showDemo = true;
					if (showDemo) ImGui::ShowDemoWindow(&showDemo);

					RenderImGui(ts);
				}
				rlImGuiEnd(); // ends the ImGui content mode. Make all ImGui calls before this
			}
			EndDrawing();
		}
	}

	void Application::Update(const float ts) {
		m_Scene.Update(ts);
	}

	void Application::RenderScene(const float ts) {
		m_Scene.Render(ts);
	}

	void Application::RenderGui(const float ts) {
		m_Scene.RenderGui(ts);
	}

	void Application::RenderImGui(const float ts) {
		m_Scene.RenderImGui(ts);
	}

	void Application::CustomBegin3D(const TRG::Application::Camera& camera) {
		// Initializes 3D mode with custom camera (3D)
		rlDrawRenderBatchActive();      // Update and draw internal render batch

		rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
		rlPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
		rlLoadIdentity();               // Reset current matrix (projection)

		const Mat4 projMatrix = camera.GetProjectionMatrix(m_Width, m_Height, 0.1, 100);
		rlSetMatrixProjection(Mat4ToRaylib(projMatrix));

		rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
		rlLoadIdentity();               // Reset current matrix (modelview)
		const Matrix rlViewMat = Mat4ToRaylib(camera.GetViewMatrix());
		rlMultMatrixf(MatrixToFloat(rlViewMat));      // Multiply modelview matrix by view matrix (camera)

		rlEnableDepthTest();            // Enable DEPTH_TEST for 3D

	}

	void Application::swap(Application &application) noexcept {
		std::swap(m_Width, application.m_Width);
		std::swap(m_Height, application.m_Height);
		std::swap(m_Scene, application.m_Scene);
	}
} // TRG::Application
