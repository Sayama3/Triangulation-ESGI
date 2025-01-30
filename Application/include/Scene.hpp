//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "Render/Renderable.hpp"

#include <raylib.h>

namespace TRG::Application {

	class Scene : public Renderable {
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
		void RenderImGui(float ts) override;
		Camera3D GetCamera3D() const;
	public:
		void SetWindow(uint32_t width, uint32_t height);
	public:
		void swap(Scene& other) noexcept;
	private:
		// NOTE: Be careful, background width must be equal or bigger than screen width
		// if not, texture should be draw more than two times for scrolling effect
		Texture2D background;
		Texture2D midground;
		Texture2D foreground;

		uint32_t m_Width;
		uint32_t m_Height;

		float scrollingBack = 0.0f;
		float scrollingMid = 0.0f;
		float scrollingFore = 0.0f;
	};

} // TRG::Application
