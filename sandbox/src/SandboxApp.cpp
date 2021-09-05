#include "Worlds.hpp"
#include "Worlds/Core/EntryPoint.hpp"
#include "Worlds/Scenes/Scenes.hpp"

#include <iostream>

#include "SandboxRenderer.hpp"
#include "SandboxScene.hpp"

#include "Worlds/Events/KeyboardEvents.hpp"

class Sandbox : public Worlds::Application {
  public:
    Sandbox(int argc, char **argv) : Worlds::Application(argc, argv) {}

    void OnAppStart() override {
        Graphics::Get()->SetRenderer(std::make_unique<SandboxRenderer>());

        Scenes::Get()->SetScene(CreateScope<SandboxScene>());
    }

    void OnAppEvent(Event &e) override {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<KeyPressedEvent>(
            W_BIND_EVENT_FN(Sandbox::OnKeyPressed));
    }

    void OnAppUpdate() override {}

  private:
    bool OnKeyPressed(KeyPressedEvent &e) {
        if (e.getKey() == GLFW_KEY_SPACE) {
            auto renderer =
                static_cast<SandboxRenderer *>(Graphics::Get()->GetRenderer());
            renderer->SetColor(colors[++color % colors.size()]);
        } else if (e.getKey() == GLFW_KEY_W) {
            glm::vec3 oldPos = Scenes::Get()->GetCamera()->GetPosition();
            Scenes::Get()->GetCamera()->SetPosition(oldPos +
                                                    glm::vec3(0, 0, -0.1));
        } else if (e.getKey() == GLFW_KEY_A) {
            glm::vec3 oldPos = Scenes::Get()->GetCamera()->GetPosition();
            Scenes::Get()->GetCamera()->SetPosition(oldPos +
                                                    glm::vec3(-0.1, 0, 0));
        } else if (e.getKey() == GLFW_KEY_S) {
            glm::vec3 oldPos = Scenes::Get()->GetCamera()->GetPosition();
            Scenes::Get()->GetCamera()->SetPosition(oldPos +
                                                    glm::vec3(0, 0, 0.1));
        } else if (e.getKey() == GLFW_KEY_D) {
            glm::vec3 oldPos = Scenes::Get()->GetCamera()->GetPosition();
            Scenes::Get()->GetCamera()->SetPosition(oldPos +
                                                    glm::vec3(0.1, 0, 0));
        }
        return false;
    }

    const std::vector<glm::vec4> colors{
        {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
        {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1},
    };
    uint32_t color = 0;
};

Worlds::Application *Worlds::CreateApplication(int argc, char **argv) {
    return new Sandbox(argc, argv);
}
