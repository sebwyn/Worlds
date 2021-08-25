#pragma once

#include <sstream>

#include "Worlds/Core/Base.hpp"
#include "Worlds/Events/Event.hpp"

#include <glm/glm.hpp>
#include <volk.h>

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

    static Scope<Window> Create(const WindowProps &props = WindowProps());
    static Window &Get() { return *instance; }

    Window() { instance = this; }
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

  private:
    static Window *instance;
};

} // namespace Worlds
