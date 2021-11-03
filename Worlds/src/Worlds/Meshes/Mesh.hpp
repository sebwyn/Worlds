#pragma once

#include "Worlds/Models/Model.hpp"

#include "Worlds/Graphics/Buffers/UniformHandler.hpp"
#include "Worlds/Graphics/Descriptors/DescriptorsHandler.hpp"

namespace Worlds {

class Mesh {
  public:
    Mesh(Scope<Model> model) : model(std::move(model)) {}

    void CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline,
                   UniformHandler &uniformScene, const glm::mat4 &modelMatrix) {

        descriptorSet.Push("UniformScene", uniformScene);

        uniformObject.Push("transform", modelMatrix);
        uniformObject.Push("color", glm::vec4(1.0));
        descriptorSet.Push("UniformObject", uniformObject);

        if (!descriptorSet.Update(pipeline))
            return;

        descriptorSet.BindDescriptor(commandBuffer, pipeline);
        model->CmdRender(commandBuffer, 1);
    }

  private:
    Scope<Model> model;

    DescriptorsHandler descriptorSet;
    UniformHandler uniformObject;
};

}; // namespace Worlds
