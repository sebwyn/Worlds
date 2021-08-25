#include "Worlds.hpp"
#include "Worlds/Core/EntryPoint.hpp"

#include <iostream>

#include "SandboxRenderer.hpp"

class Sandbox : public Worlds::Application {
  public:
    Sandbox(int argc, char **argv) : Worlds::Application(argc, argv) {
        Graphics::Get()->SetRenderer(std::make_unique<SandboxRenderer>());
    }

  private:
};

Worlds::Application *Worlds::CreateApplication(int argc, char **argv) {
    return new Sandbox(argc, argv);
}
