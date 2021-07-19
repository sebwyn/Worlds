#include "Worlds.hpp"
#include "Worlds/Core/EntryPoint.hpp"

#include <iostream>

class Sandbox : public Worlds::Application {
  public:
    Sandbox() {
        std::cout << "Hello Sandbox!" << std::endl; 
    }
  private:
};

Worlds::Application *Worlds::CreateApplication() { return new Sandbox(); }
