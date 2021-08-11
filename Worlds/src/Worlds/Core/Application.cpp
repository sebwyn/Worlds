#include "Worlds/Core/Application.hpp"
#include "wpch.hpp"

namespace Worlds {

Application *Application::s_instance = nullptr;

Application::Application() {
    s_instance = this;
    m_window = Window::create(WindowProps());
    m_window->setEventCallback(W_BIND_EVENT_FN(Application::onEvent));

    m_renderAPI = RenderAPI::create(m_window->getNativeWindow());
}

void Application::onEvent(Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(
        W_BIND_EVENT_FN(Application::onWindowResize));
    dispatcher.dispatch<WindowCloseEvent>(
        W_BIND_EVENT_FN(Application::onWindowClose));
}

void Application::close() { m_running = false; }

void Application::run() {
    while (m_running) {
        m_renderAPI->onUpdate();
        m_window->onUpdate();
    }
}

bool Application::onWindowResize(WindowResizeEvent &e) {
    if (e.getWidth() == 0 || e.getHeight() == 0) {
        m_minimized = true;
        return false;
    }

    m_minimized = false;
    m_renderAPI->onWindowResize(e);

    return false;
}

bool Application::onWindowClose(WindowCloseEvent &e) {
    m_running = false;
    return true;
}

Application::~Application() {}
}; // namespace Worlds
