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

    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;

    W_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width,
                props.height);

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        W_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        // for vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(m_data.width, m_data.height,
                                    m_data.title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;
    }

    glfwSetWindowUserPointer(m_window, &m_data);
    setVSync(true);

    glfwSetWindowSizeCallback(
        m_window, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    // TODO: get key and mouse events
}

void MacWindow::shutdown() {
    glfwDestroyWindow(m_window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void MacWindow::onUpdate() { glfwPollEvents(); }

void MacWindow::setVSync(bool enabled) {
    // TODO: get vsyn working
    m_data.vsync = enabled;
}

bool MacWindow::isVSync() const { return m_data.vsync; }

glm::uvec2 MacWindow::GetSize() {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return glm::uvec2(width, height);
}

std::pair<const char **, uint32_t> MacWindow::GetInstanceExtensions() {
    uint32_t count;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);
    return std::make_pair(extensions, count);
}

VkResult MacWindow::CreateSurface(const VkInstance &instance,
                                  const VkAllocationCallbacks *allocator,
                                  VkSurfaceKHR *surface) {
    return glfwCreateWindowSurface(instance, m_window, allocator, surface);
}

}; // namespace Worlds
