#pragma once

#include "Worlds/Core/Window.hpp"
#include "Worlds/Events/ApplicationEvent.hpp"
#include "Worlds/Events/Event.hpp"

#include "Worlds/Graphics/Graphics.hpp"
#include "Worlds/Files/Files.hpp"

int main(int argc, char **argv);

namespace Worlds {

class Application {
  public:
    Application(int argc, char **argv);
    virtual ~Application();

    void OnEvent(Event &e);

    void Close();

    Window &GetWindow() { return *window; }
    static Application &Get() { return *instance; }

    char *GetArgv0() { return argv[0]; }

    bool IsMinimized() { return minimized; }

  protected:
    Scope<Graphics> graphics;

  private:
    void Run();
    bool OnWindowResize(WindowResizeEvent &e);
    bool OnWindowClose(WindowCloseEvent &e);

  private:
    int argc;
    char **argv;

    bool running = true;
    bool minimized = false;

    Scope<Window> window;
    Scope<Files> files;

    static Application *instance;
    friend int ::main(int argc, char **argv);
};

Application *CreateApplication(int argc, char **argv);

} // namespace Worlds
