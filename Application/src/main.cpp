//
// Created by ianpo on 26/01/2025.
//

#include "Application.hpp"

int main(void)
{

	auto* app = new TRG::Application::Application(800, 450, "Application");
	app->Run();
	delete app;
}