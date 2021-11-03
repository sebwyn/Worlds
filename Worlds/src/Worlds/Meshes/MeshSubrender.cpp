#include "Worlds/Meshes/MeshSubrender.hpp"

#include "Worlds/Models/Vertex3d.hpp"
#include "Worlds/Scenes/Scenes.hpp"

#include "Worlds/Maths/Transform.hpp"
#include "Worlds/Meshes/Mesh.hpp"
#include "Worlds/Models/Model.hpp"
#include "Worlds/Models/Shapes/Cube.hpp"

namespace Worlds {

MeshSubrender::MeshSubrender(Pipeline::Stage stage)
    : Subrender(stage),
      coloredPipeline(stage,
                      {"../Sandbox/assets/shaders/scene.frag",
                       "../Sandbox/assets/shaders/scene.vert"},
                      {Vertex3d::GetVertexInput()}, {},
                      PipelineGraphics::Mode::Polygon,
                      PipelineGraphics::Depth::None) {}

void MeshSubrender::Render(const CommandBuffer &commandBuffer) {
    auto camera = Scenes::Get()->GetCamera();

    coloredPipeline.BindPipeline(commandBuffer);

    uniformScene.Push("projection", camera->GetProjectionMatrix());
    uniformScene.Push("view", camera->GetViewMatrix());

    auto view = Scenes::Get()->GetRegistry()->view<Transform, Mesh>();
    for (auto entity : view) {
        auto &transform = view.get<Transform>(entity);
        auto &mesh = view.get<Mesh>(entity);

        transform.UpdateMatrix();
        mesh.CmdRender(commandBuffer, coloredPipeline, uniformScene, transform.GetMatrix());
    }
}

} // namespace Worlds
