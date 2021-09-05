#pragma once

#include <unordered_map>
#include <vector>
#include <bitset>

#include "Worlds/Scenes/Scenes.hpp"
#include "Worlds/Scenes/Camera.hpp"

class SandboxScene : public Worlds::Scene {
  public:
    SandboxScene(){
        SetCamera(std::make_unique<Worlds::Camera>());
        GetCamera()->SetPosition(glm::vec3(0, 0, 10));
    } 
  private:

};
