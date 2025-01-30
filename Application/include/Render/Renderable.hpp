//
// Created by ianpo on 30/01/2025.
//

#pragma once

namespace TRG::Application {
	class Renderable {
	public:
		virtual ~Renderable() = default;
	public:
		virtual void Update(float ts) = 0;
		virtual void Render(float ts) = 0;
		virtual void RenderGui(float ts) = 0;
		virtual void RenderImGui(float ts) = 0;
	};
}
