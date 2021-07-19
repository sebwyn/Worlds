#pragma once

int main(int argc, char** argv);

namespace Worlds {

class Application {
  public:
    Application();
    virtual ~Application();
  private:
    void run();

    bool m_running = true;
    bool m_minimized = true;
    
    static Application* s_instance;
    friend int ::main(int argc, char** argv);
};

Application* CreateApplication();

} // namespace Worlds
