//
// Created by ianpo on 26/01/2025.
//

#include "Application.hpp"
#include "Core/Singleton.hpp"

int main(void)
{
	TRG::Application::SingletonApp::Register(1600, 900, "Application");
	TRG::Application::SingletonApp::Get().Run();
	TRG::Application::SingletonApp::Unregister();
}
