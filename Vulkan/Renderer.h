#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS

#define GLFW_INCLUDE_VULKAN // <--- Indispensable pour débloquer les fonctions Vulkan de GLFW
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>
#include <algorithm>
#include <limits>
#include <cstdint>


class Renderer
{
public:
    void CreateInstance();

    void pickPhysicDevice();
    bool isDeviceSuitable(vk::raii::PhysicalDevice const & physicalDevice);
    
    void createSurface(GLFWwindow* window);
    
    void createLogicalDevice();
    void createSwapChain(GLFWwindow* window);
    
    void createImageViews();

private:
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
    
    vk::raii::SurfaceKHR surface = nullptr;
    
    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::raii::Device         device         = nullptr;
    
    vk::raii::Queue graphicsQueue = nullptr;
    std::vector<const char*> requiredDeviceExtensions = {
        vk::KHRSwapchainExtensionName};
    
    
    // Surface KHR
    vk::raii::SwapchainKHR           swapChain      = nullptr;
    std::vector<vk::Image>           swapChainImages;
    vk::SurfaceFormatKHR             swapChainSurfaceFormat;
    vk::Extent2D                     swapChainExtent;
    std::vector<vk::raii::ImageView> swapChainImageViews;
    
    static uint32_t chooseSwapMinImageCount(vk::SurfaceCapabilitiesKHR const &surfaceCapabilities)
    {
        auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
        if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount))
        {
            minImageCount = surfaceCapabilities.maxImageCount;
        }
        return minImageCount;
    }

    static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const &availableFormats)
    {
        assert(!availableFormats.empty());
        const auto formatIt = std::ranges::find_if(
            availableFormats,
            [](const auto &format) { return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; });
        return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
    }

    static vk::PresentModeKHR chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const &availablePresentModes)
    {
        assert(std::ranges::any_of(availablePresentModes, [](auto presentMode) { return presentMode == vk::PresentModeKHR::eFifo; }));
        return std::ranges::any_of(availablePresentModes,
                                   [](const vk::PresentModeKHR value) { return vk::PresentModeKHR::eMailbox == value; }) ?
                   vk::PresentModeKHR::eMailbox :
                   vk::PresentModeKHR::eFifo;
    }
    vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilitiesKHR const &capabilities,GLFWwindow* window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        return {
            std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)};
    }
    
    
    
    //Image Views

    
    
    std::vector<char const*> GetRequiredlayers();
    std::vector<char const*> GetRequiredExtensions();
    
    void SetupDebugMessenger();
    
    
    static  VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity, 
        vk::DebugUtilsMessageTypeFlagsEXT type, 
        const vk::DebugUtilsMessengerCallbackDataEXT* data,
        void*);
    
    

    
    
};