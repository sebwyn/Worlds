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

    void onUpdate() override;

    uint32_t getWidth() const override { return m_data.width; }
    uint32_t getHeight() const override { return m_data.height; }

    // Window attributes
    void setEventCallback(const EventCallbackFn &callback) override {
        m_data.eventCallback = callback;
    }
    void setVSync(bool enabled) override;
    bool isVSync() const override;

    glm::uvec2 GetSize() override;

    void *getNativeWindow() override { return m_window; }
    std::pair<const char**, uint32_t> GetInstanceExtensions() override;
    VkResult CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) override;

  private:
    void init(const WindowProps &props);
    void shutdown();

  private:
    GLFWwindow *m_window;

    struct WindowData {
        std::string title;
        uint32_t width, height;
        bool vsync;

        EventCallbackFn eventCallback;
    };

    WindowData m_data;
};

} // namespace Worlds
