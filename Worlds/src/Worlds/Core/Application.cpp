#include "Worlds/Core/Application.hpp"
#include "wpch.hpp"

namespace Worlds {

Application *Application::instance = nullptr;

Application::Application(int argc, char **argv, ModuleFilter &&moduleFilter)
    : argc(argc), argv(argv) {
    instance = this;
    Worlds::Log::init();

    W_INFO("Argv 0:" + std::string(GetArgv0())); 

    // TODO: Optimize and clean up!
    std::vector<TypeId> created;
    for (;;) {
        bool postponed = false;
        for (const auto &[moduleId, moduleTest] : Module::Registry()) {
            if (std::find(created.begin(), created.end(), moduleId) !=
                created.end())
                continue;
            if (!moduleFilter.Check(moduleId))
                continue;
            bool this_postponed = false;
            for (const auto &requireId : moduleTest.requires) {
                if (!moduleFilter.Check(moduleId))
                    break;
                if (std::find(created.begin(), created.end(), requireId) ==
                    created.end()) {
                    this_postponed = true;
                    break;
                }
            }
            if (this_postponed) {
                postponed = true;
                continue;
            }
            auto &&module = moduleTest.create();
            modules.emplace(Module::StageIndex(moduleTest.stage, moduleId),
                            std::move(module));
            created.emplace_back(moduleId);
        }
        if (!postponed)
            break;
    }

    WindowAPI::Get()->SetEventCallback(W_BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() { Module::Registry().clear(); }

void Application::OnEvent(Event &e) {
    this->OnAppEvent(e);

    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(
        W_BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.dispatch<WindowCloseEvent>(
        W_BIND_EVENT_FN(Application::OnWindowClose));

    for (auto &[stage, module] : modules) {
        module->OnEvent(e);
    }
}

void Application::Close() { running = false; }

void Application::Run() {

    this->OnAppStart();

    while (running) {
        this->OnAppUpdate();

        // TODO: create some structure and don't always update all module stages
        UpdateStage(Module::Stage::Always);

        UpdateStage(Module::Stage::Pre);

        UpdateStage(Module::Stage::Normal);

        UpdateStage(Module::Stage::Post);

        UpdateStage(Module::Stage::Render);
    }
}

void Application::UpdateStage(Module::Stage stage) {
    for (auto &[stageIndex, module] : modules) {
        if (stageIndex.first == stage)
            module->Update();
    }
}

bool Application::OnWindowResize(WindowResizeEvent &e) {
    if (e.getWidth() == 0 || e.getHeight() == 0) {
        minimized = true;
        return false;
    }

    minimized = false;
    return false;
}

bool Application::OnWindowClose(WindowCloseEvent &e) {
    running = false;
    return true;
}

}; // namespace Worlds
