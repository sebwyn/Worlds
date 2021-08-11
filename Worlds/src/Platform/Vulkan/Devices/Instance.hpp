#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Worlds {

class Instance {
  public:
    Instance();
    ~Instance();

    const VkInstance &getInstance() const { return m_instance; }

  private:
    void createInstance();
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void *pUserData);
    void setupDebugMessenger();

  private:
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
};

} // namespace Worlds
