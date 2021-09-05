#pragma once

#include "Worlds/Core/Window.hpp"
#include "Worlds/Events/ApplicationEvent.hpp"
#include "Worlds/Events/Event.hpp"

#include "Worlds/Files/Files.hpp"
#include "Worlds/Graphics/Graphics.hpp"

#include "Worlds/Core/Module.hpp"

int main(int argc, char **argv);

namespace Worlds {

class Application {
  public:
    static Application &Get() { return *instance; }

    Application(int argc, char **argv, ModuleFilter &&moduleFilter = {});
    virtual ~Application();

    virtual void OnAppStart() = 0;

    virtual void OnAppEvent(Event &e) = 0;
    virtual void OnAppUpdate() = 0;

    void Close();

    void OnEvent(Event &e);

    char *GetArgv0() { return argv[0]; }
    bool IsMinimized() { return minimized; }

  protected:
    Scope<Graphics> graphics;

  private:
    void Run();
    bool OnWindowResize(WindowResizeEvent &e);
    bool OnWindowClose(WindowCloseEvent &e);

    void UpdateStage(Module::Stage stage);

  private:
    static Application *instance;

    int argc;
    char **argv;

    bool running = true;
    bool minimized = false;

    std::multimap<Module::StageIndex, Scope<Module>> modules;

    friend int ::main(int argc, char **argv);
};

Application *CreateApplication(int argc, char **argv);

} // namespace Worlds
