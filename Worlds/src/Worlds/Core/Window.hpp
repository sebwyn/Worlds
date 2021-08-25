#pragma once

#include <glm/glm.hpp>
#include <sstream>
#include <volk.h>

#include "Worlds/Core/Base.hpp"
#include "Worlds/Events/Event.hpp"

#include "Worlds/Core/Module.hpp"

namespace Worlds {

struct WindowProps {
    std::string title;
    uint32_t width, height;
    WindowProps(const std::string &title = "Worlds Engine",
                uint32_t width = 800, uint32_t height = 600)
        : title(title), width(width), height(height) {}
};

class Window {
  public:
    using EventCallbackFn = std::function<void(Event &)>;

    Window() {}
    virtual ~Window() = default;

    virtual void Update() = 0;

    virtual glm::uvec2 GetSize() = 0;

    virtual bool IsVSync() const = 0;
    virtual void SetVSync(bool enabled) = 0;

    virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

    virtual void *GetNativeWindow() = 0;

    virtual std::pair<const char **, uint32_t> GetInstanceExtensions() = 0;
    virtual VkResult CreateSurface(const VkInstance &instance,
                                   const VkAllocationCallbacks *allocator,
                                   VkSurfaceKHR *surface) = 0;
};

class WindowAPI : public Module::Registrar<WindowAPI> {
    inline static const bool Registered = Register(Stage::Pre);

  public:
    WindowAPI();

    void Update() override { window->Update(); }

    glm::uvec2 GetSize() { return window->GetSize(); }
    bool IsVSync() const { return window->IsVSync(); }
    void SetVSync(bool enabled) { window->SetVSync(enabled); }

    void SetEventCallback(const Window::EventCallbackFn &callback) {
        window->SetEventCallback(callback);
    }

    void *GetNativeWindow() { return window->GetNativeWindow(); }

    std::pair<const char **, uint32_t> GetInstanceExtensions() {
        return window->GetInstanceExtensions();
    }
    VkResult CreateSurface(const VkInstance &instance,
                           const VkAllocationCallbacks *allocator,
                           VkSurfaceKHR *surface){
        return window->CreateSurface(instance, allocator, surface);
    }

  private:
    Scope<Window> window;
};

} // namespace Worlds
