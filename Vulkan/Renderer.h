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
    void createSwapChain();

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

    
    std::vector<char const*> GetRequiredlayers();
    std::vector<char const*> GetRequiredExtensions();
    
    void SetupDebugMessenger();
    
    
    static  VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity, 
        vk::DebugUtilsMessageTypeFlagsEXT type, 
        const vk::DebugUtilsMessengerCallbackDataEXT* data,
        void*);
    
    

    
    
};