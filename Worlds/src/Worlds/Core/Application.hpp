#pragma once

#include "Worlds/Core/Window.hpp"
#include "Worlds/Events/ApplicationEvent.hpp"
#include "Worlds/Events/Event.hpp"
#include "Worlds/Renderer/RenderAPI.hpp"

int main(int argc, char **argv);

namespace Worlds {

class Application {
  public:
    Application();
    virtual ~Application();

    void onEvent(Event &e);

    void close();

    Window &getWindow() { return *m_window; }
    static Application &get() { return *s_instance; }

  private:
    void run();
    bool onWindowResize(WindowResizeEvent &e);
    bool onWindowClose(WindowCloseEvent &e);

  private:

    bool m_running = true;
    bool m_minimized = false;

    Scope<Window> m_window;
    Scope<RenderAPI> m_renderAPI;

    static Application *s_instance;
    friend int ::main(int argc, char **argv);
};

Application *CreateApplication();

} // namespace Worlds
