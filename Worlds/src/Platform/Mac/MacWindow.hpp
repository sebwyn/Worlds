#pragma once

// TODO: move this include to make glfw be able to use either
#include <volk.h>
#include <GLFW/glfw3.h>

#include "Worlds/Core/Window.hpp"

namespace Worlds {

class MacWindow : public Window {
  public:
    MacWindow(const WindowProps &props);
    virtual ~MacWindow();

    void Update() override;

    glm::uvec2 GetSize() override;

    void SetEventCallback(const EventCallbackFn &callback) override {
        data.eventCallback = callback;
    }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    void EnableCursor();
    void DisableCursor();

    void *GetNativeWindow() override { return window; }

    std::pair<const char **, uint32_t> GetInstanceExtensions() override;
    VkResult CreateSurface(const VkInstance &instance,
                           const VkAllocationCallbacks *allocator,
                           VkSurfaceKHR *surface) override;

  private:
    void init(const WindowProps &props);
    void shutdown();

  private:
    GLFWwindow *window;

    struct WindowData {
        std::string title;
        uint32_t width, height;
        bool vsync;
        bool cursorEnabled;

        EventCallbackFn eventCallback;
    };

    WindowData data;
};

} // namespace Worlds
