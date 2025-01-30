//
// Created by ianpo on 26/01/2025.
//

#include "Application.hpp"
#include "Core/Singleton.hpp"

int main(void)
{
	using SingletonApp = TRG::Application::Singleton<TRG::Application::Application>;
	SingletonApp::Register(800, 450, "Application");
	SingletonApp::Get().Run();
	SingletonApp::Unregister();
}
