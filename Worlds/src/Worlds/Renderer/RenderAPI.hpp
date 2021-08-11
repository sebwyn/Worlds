#pragma once

#include "Worlds/Core/Base.hpp"
#include "Worlds/Events/ApplicationEvent.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Worlds {

class RenderAPI {
  public:
    enum class API { None = 0, Vulkan = 1 };

  public:
    virtual ~RenderAPI() = default;

    virtual void onWindowResize(WindowResizeEvent& e) = 0;
    virtual void onUpdate() = 0;

    API getAPI() { return s_API; }
    static Scope<RenderAPI> create(void *window);

  private:
    static API s_API;
};

} // namespace Worlds
