#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS

#include <iostream>
#include <vulkan/vulkan_raii.hpp>



class Renderer
{
public:
    void CreateInstance();

private:
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
    vk::raii::PhysicalDevice physicalDevice = nullptr;

    std::vector<char const*> GetRequiredlayers();
    std::vector<char const*> GetRequiredExtensions();
    
    void SetupDebugMessenger();
    
    
    static  VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity, 
        vk::DebugUtilsMessageTypeFlagsEXT type, 
        const vk::DebugUtilsMessengerCallbackDataEXT* data,
        void*);
    
    
    void pickDevice();
    bool isDeviceSuitable(vk::raii::PhysicalDevice const & physicalDevice);
    
    
};