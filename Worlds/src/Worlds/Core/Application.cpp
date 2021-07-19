#include "Worlds/Core/Application.hpp"
#include "wpch.hpp"

namespace Worlds {

Application* Application::s_instance = nullptr;

Application::Application() {}
Application::~Application() {}
void Application::run() {}

}; // namespace Worlds
