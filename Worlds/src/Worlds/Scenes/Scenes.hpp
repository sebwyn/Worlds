#pragma once

#include "Worlds/Scenes/Scene.hpp"

#include "Worlds/Core/Module.hpp"

namespace Worlds {

class Scenes : Module::Registrar<Scenes> {
    inline static const bool Registered = Register(Stage::Normal);

  public:
    Scenes();
    ~Scenes();

    void Update() override;

    Scene *GetScene() { return scene.get(); }
    void SetScene(Scope<Scene> &&scene){ this->scene = std::move(scene); }
    
  private:
    Scope<Scene> scene;
};

} // namespace Worlds
