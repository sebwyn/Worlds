#pragma once

#include "Worlds/Models/Model.hpp"

namespace Worlds {

class Mesh {
  public:
    Mesh(Scope<Model> model) : model(std::move(model)) {}

    void CmdRender(const CommandBuffer &commandBuffer) {
        model->CmdRender(commandBuffer, 1);
    }

  private:
    Scope<Model> model;
};

}; // namespace Worlds
