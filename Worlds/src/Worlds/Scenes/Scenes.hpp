#pragma once

#include "Worlds/Core/Module.hpp"
#include "Worlds/Scenes/Camera.hpp"

#include <entt/entt.hpp>

namespace Worlds {

class Scene {
  public:
    Scene() = default;
    Scene(Scope<Camera> &&camera) { SetCamera(std::move(camera)); }

    Camera *GetCamera() { return camera.get(); }
    void SetCamera(Scope<Camera> &&camera) { this->camera = std::move(camera); }

    entt::registry *GetRegistry() { return &registry; }

  private:
    Scope<Camera> camera;

    entt::registry registry;
};

class Scenes : public Module::Registrar<Scenes> {
    inline static const bool Registered = Register(Stage::Normal);

  public:
    Scenes() = default;

    void Update() override {
        if(scene){
            GetCamera()->Update();
        } 
    }

    Scene *GetScene() { return scene.get(); }
    void SetScene(Scope<Scene> &&scene) { this->scene = std::move(scene); }

    Camera *GetCamera() { return (scene ? scene->GetCamera() : nullptr); }
    entt::registry *GetRegistry() {
        return (scene ? scene->GetRegistry() : nullptr);
    }

  private:
    Scope<Scene> scene;
};

} // namespace Worlds
