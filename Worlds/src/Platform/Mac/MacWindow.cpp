#include "Platform/Mac/MacWindow.hpp"
#include "wpch.hpp"

#include "Worlds/Events/ApplicationEvent.hpp"

namespace Worlds {

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char *description) {
    W_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

MacWindow::MacWindow(const WindowProps &props) { init(props); }

MacWindow::~MacWindow() { shutdown(); }

void MacWindow::init(const WindowProps &props) {
    data.title = props.title;
    data.width = props.width;
    data.height = props.height;

    W_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width,
                props.height);

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        W_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        // for vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(data.width, data.height, data.title.c_str(),
                                  nullptr, nullptr);
        ++s_GLFWWindowCount;
    }

    glfwSetWindowUserPointer(window, &data);
    SetVSync(true);

    glfwSetWindowSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

    glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    // TODO: get key and mouse events
}

void MacWindow::shutdown() {
    glfwDestroyWindow(window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void MacWindow::Update() { 
    glfwPollEvents(); 
}

void MacWindow::SetVSync(bool enabled) {
    // TODO: get vsyn working
    data.vsync = enabled;
}

bool MacWindow::IsVSync() const { return data.vsync; }

glm::uvec2 MacWindow::GetSize() { return glm::uvec2(data.width, data.height); }

std::pair<const char **, uint32_t> MacWindow::GetInstanceExtensions() {
    uint32_t count;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);
    return std::make_pair(extensions, count);
}

VkResult MacWindow::CreateSurface(const VkInstance &instance,
                                  const VkAllocationCallbacks *allocator,
                                  VkSurfaceKHR *surface) {
    return glfwCreateWindowSurface(instance, window, allocator, surface);
}

}; // namespace Worlds
