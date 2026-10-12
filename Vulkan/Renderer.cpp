#include "Renderer.h"
#include "Debug.h"
#include <iostream>
#include <print>
#include <GLFW/glfw3.h>

void Renderer::CreateInstance()
{
    constexpr vk::ApplicationInfo app_info
    {
        .pApplicationName = "Vulkan triangle",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "Gaming Campus Triangle",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };
    
    
    std::vector requiredExtensions = GetRequiredExtensions();
    auto extensionProperties = context.enumerateInstanceExtensionProperties();


    for (uint32_t i = 0; i < requiredExtensions.size(); ++i)
    {
        if (std::ranges::none_of(extensionProperties,
            [glfwExtension = requiredExtensions[i]](auto const& extensionProperty)
            {
                return strcmp(extensionProperty.extensionName, glfwExtension);
            }
        ))
        {
            throw std::runtime_error("Required GLFW extension not supported: " + std::string(requiredExtensions[i]));
        }
    }

    const std::vector<char const*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::vector<char const*> requiredLayers;

    if (Debug::EnableDebug) {
        requiredLayers.assign_range(validationLayers);
    }

    auto layerProperties = context.enumerateInstanceLayerProperties();

    auto predicate_layer = [&layerProperties](auto const& requiredLayer)
        {
            return std::ranges::none_of(layerProperties, [requiredLayer](auto const& layerProperty)
                {
                    return strcmp(layerProperty.layerName, requiredLayer) == 0;
                });

        };

    auto unsupportedLayerIt = std::ranges::find_if(requiredLayers, predicate_layer);

    if (unsupportedLayerIt != requiredLayers.end())
    {
        throw std::runtime_error("Message layer not suppported: " + std::string(*unsupportedLayerIt));
    }

    vk::InstanceCreateInfo create_info
    {
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t> (requiredLayers.size()),
        .ppEnabledLayerNames = requiredLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    instance = vk::raii::Instance(context, create_info);

}

std::vector<char const*> Renderer::GetRequiredlayers()
{

    const std::vector validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::vector<char const*> requiredLayers = {};

    if (Debug::EnableDebug)
        requiredLayers.append_range(validationLayers);

    return requiredLayers;
}

std::vector<char const*> Renderer::GetRequiredExtensions()
{
    uint32_t glfwExtensionsCount = 0;

    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    
    std::vector<char const* > applicationExtensions = {vk::EXTDebugUtilsExtensionName };
    std::vector requireExtensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

    requireExtensions.append_range(applicationExtensions);

    return requireExtensions;
}

vk::Bool32 Renderer::DebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, const vk::DebugUtilsMessengerCallbackDataEXT* data, void*)
{
    std::cout << " Validation Layer : Type [" << vk::to_string(type) << "] - Message : " << data->pMessage <<  std::endl;  
    
    return vk::False;
}


void Renderer::pickDevice()
{
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    auto const devIter = std::ranges::find_if( physicalDevices, [&]( auto const & physicalDevice ) { return isDeviceSuitable( physicalDevice ); } );
    if ( devIter == physicalDevices.end() )
    {
        throw std::runtime_error( "failed to find a suitable GPU!" );
    }
    physicalDevice = *devIter;
}



bool Renderer::isDeviceSuitable(vk::raii::PhysicalDevice const & physicalDevice)
{
    // Check physical device support Vulkan 1.4 
    bool supportsVulkan = physicalDevice.getProperties().apiVersion >= vk::ApiVersion14;
    
    // return true if the physicalDevice meet the criteria
    return supportsVulkan;
}

void Renderer::SetupDebugMessenger()
{
    
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError); 
    
    vk::DebugUtilsMessageTypeFlagsEXT typeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    
    vk::DebugUtilsMessengerCreateInfoEXT createInfo 
    {
        .messageSeverity = severityFlags,
        .messageType = typeFlags,
        .pfnUserCallback = &DebugCallback,
    };
    
    debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo);
}
