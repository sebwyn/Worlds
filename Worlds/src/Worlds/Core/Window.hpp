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

    Window() { instance = this; }
    virtual ~Window() = default;

    virtual void onUpdate() = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    // Window attributes
    virtual void setEventCallback(const EventCallbackFn &callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;

    virtual void *getNativeWindow() = 0;
    virtual std::pair<const char**, uint32_t> GetInstanceExtensions() = 0;
    virtual VkResult CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) = 0;

    static Scope<Window> create(const WindowProps &props = WindowProps());

    virtual glm::uvec2 GetSize() = 0;

    static Window &Get() { return *instance; }

  private:
    static Window *instance;
};

} // namespace Worlds
