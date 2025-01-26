//
// Created by ianpo on 26/01/2025.
//

#pragma once

namespace TRG::Application {

	class Application {
	public:
		Application(int width, int height, const std::string& name);
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	public:
		void Run();
	private:
		uint32_t m_Width;
		uint32_t m_Heigh;
	};

} // TRG::Application
