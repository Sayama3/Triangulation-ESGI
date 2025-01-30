//
// Created by ianpo on 26/01/2025.
//

#pragma once
#include "Scene.hpp"

namespace TRG::Application {

	class Application {
	public:
		Application(int width, int height, const std::string& name);
		~Application();

		Application(Application&&) noexcept;
		Application& operator=(Application&&) noexcept;

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	public:
		void Run();
	private:
		void Update(float ts);
		void RenderScene(float ts);
		void RenderGui(float ts);
		void RenderImGui(float ts);
	public:
		void swap(Application& application) noexcept;
	public:
		[[nodiscard]] uint32_t GetWidth() const {return m_Width;}
		[[nodiscard]] uint32_t GetHeight() const {return m_Height;}
	private:
		uint32_t m_Width;
		uint32_t m_Height;
		Scene m_Scene;
	};
} // TRG::Application
