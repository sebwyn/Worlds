#include "Worlds/Core/Application.hpp"
#include "wpch.hpp"

namespace Worlds {

Application *Application::instance = nullptr;

Application::Application(int argc, char **argv) : argc(argc), argv(argv) {
    instance = this;
    window = Window::Create(WindowProps());
    window->SetEventCallback(W_BIND_EVENT_FN(Application::OnEvent));

    graphics = CreateScope<Graphics>();
    files = CreateScope<Files>();
}

void Application::OnEvent(Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(
        W_BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.dispatch<WindowCloseEvent>(
        W_BIND_EVENT_FN(Application::OnWindowClose));
}

void Application::Close() { running = false; }

void Application::Run() {
    while (running) {
        window->Update();
        graphics->Update();
    }
}

bool Application::OnWindowResize(WindowResizeEvent &e) {
    if (e.getWidth() == 0 || e.getHeight() == 0) {
        minimized = true;
        return false;
    }

    minimized = false;
    return false;
}

bool Application::OnWindowClose(WindowCloseEvent &e) {
    running = false;
    return true;
}

Application::~Application() {}
}; // namespace Worlds
