set(PROJECT_NAME Aria)

################################################################################
# Source groups
################################################################################
set(src
    "src/Aria.h"
    "src/ariapch.cpp"
    "src/ariapch.h"
    )
source_group("src" FILES ${src})

set(src__Aria__Core
    "src/Aria/Core/Application.cpp"
    "src/Aria/Core/Application.h"
    "src/Aria/Core/Base.h"
    "src/Aria/Core/EntryPoint.h"
    "src/Aria/Core/Input.h"
    "src/Aria/Core/KeyCodes.h"
    "src/Aria/Core/Layer.cpp"
    "src/Aria/Core/Layer.h"
    "src/Aria/Core/LayerStack.cpp"
    "src/Aria/Core/LayerStack.h"
    "src/Aria/Core/Log.cpp"
    "src/Aria/Core/Log.h"
    "src/Aria/Core/Timestep.h"
    "src/Aria/Core/Window.cpp"
    "src/Aria/Core/Window.h"
    )
source_group("src\\Aria\\Core" FILES ${src__Aria__Core})

set(src__Aria__Events
    "src/Aria/Events/ApplicationEvent.h"
    "src/Aria/Events/Event.h"
    "src/Aria/Events/KeyEvent.h"
    "src/Aria/Events/MouseEvent.h"
    )
source_group("src\\Aria\\Events" FILES ${src__Aria__Events})

set(src__Aria__ImGui
    "src/Aria/ImGui/ImGuiLayerOpenGL.cpp"
    "src/Aria/ImGui/ImGuiLayerOpenGL.h"
    "src/Aria/ImGui/ImGuiLayer.cpp"
    "src/Aria/ImGui/ImGuiLayer.h"
    "src/Aria/ImGui/ImGuiLayerVulkan.cpp"
    "src/Aria/ImGui/ImGuiLayerVulkan.h"
    )
source_group("src\\Aria\\ImGui" FILES ${src__Aria__ImGui})

set(src__Aria__Renderer
    "src/Aria/Renderer/Buffer.cpp"
    "src/Aria/Renderer/Buffer.h"
    "src/Aria/Renderer/Camera.cpp"
    "src/Aria/Renderer/Camera.h"
    "src/Aria/Renderer/GraphicsContext.h"
    "src/Aria/Renderer/RenderCommand.cpp"
    "src/Aria/Renderer/RenderCommand.h"
    "src/Aria/Renderer/Renderer.cpp"
    "src/Aria/Renderer/Renderer.h"
    "src/Aria/Renderer/RendererApi.cpp"
    "src/Aria/Renderer/RendererApi.h"
    "src/Aria/Renderer/Shader.cpp"
    "src/Aria/Renderer/Shader.h"
    "src/Aria/Renderer/Texture.cpp"
    "src/Aria/Renderer/Texture.h"
    "src/Aria/Renderer/VertexArray.cpp"
    "src/Aria/Renderer/VertexArray.h"
    )
source_group("src\\Aria\\Renderer" FILES ${src__Aria__Renderer})

set(src__Platform__OpenGL
    "src/Platform/OpenGL/OpenGlBuffer.cpp"
    "src/Platform/OpenGL/OpenGlBuffer.h"
    "src/Platform/OpenGL/OpenGlContext.cpp"
    "src/Platform/OpenGL/OpenGlContext.h"
    "src/Platform/OpenGL/OpenGlRendererApi.cpp"
    "src/Platform/OpenGL/OpenGlRendererApi.h"
    "src/Platform/OpenGL/OpenGlShader.cpp"
    "src/Platform/OpenGL/OpenGlShader.h"
    "src/Platform/OpenGL/OpenGlTexture2D.cpp"
    "src/Platform/OpenGL/OpenGlTexture2D.h"
    "src/Platform/OpenGL/OpenGlVertexArray.cpp"
    "src/Platform/OpenGL/OpenGlVertexArray.h"
    )
source_group("src\\Platform\\OpenGL" FILES ${src__Platform__OpenGL})

set(src__Platform__Vulkan
    "src/Platform/Vulkan/VulkanDebugMessenger.cpp"
    "src/Platform/Vulkan/VulkanDebugMessenger.h"
    "src/Platform/Vulkan/VulkanDeviceManager.cpp"
    "src/Platform/Vulkan/VulkanDeviceManager.h"
    "src/Platform/Vulkan/VulkanError.h"
    "src/Platform/Vulkan/VulkanGraphicsPipeline.cpp"
    "src/Platform/Vulkan/VulkanGraphicsPipeline.h"
    "src/Platform/Vulkan/VulkanInstance.cpp"
    "src/Platform/Vulkan/VulkanInstance.h"
    "src/Platform/Vulkan/VulkanLib.cpp"
    "src/Platform/Vulkan/VulkanLib.h"
    "src/Platform/Vulkan/VulkanRendererApi.cpp"
    "src/Platform/Vulkan/VulkanRendererApi.h"
    "src/Platform/Vulkan/VulkanRenderPass.cpp"
    "src/Platform/Vulkan/VulkanRenderPass.h"
    "src/Platform/Vulkan/VulkanShader.cpp"
    "src/Platform/Vulkan/VulkanShader.h"
    )
source_group("src\\Platform\\Vulkan" FILES ${src__Platform__Vulkan})

set(src__Platform__Windows
    "src/Platform/Windows/VulkanWindow.cpp"
    "src/Platform/Windows/VulkanWindow.h"
    "src/Platform/Windows/WindowsInput.cpp"
    "src/Platform/Windows/WindowsInput.h"
    "src/Platform/Windows/WindowsWindow.cpp"
    "src/Platform/Windows/WindowsWindow.h"
    )
source_group("src\\Platform\\Windows" FILES ${src__Platform__Windows})

set(ALL_FILES
    ${src}
    ${src__Aria__Core}
    ${src__Aria__Events}
    ${src__Aria__ImGui}
    ${src__Aria__Renderer}
    ${src__Platform__OpenGL}
    ${src__Platform__Vulkan}
    ${src__Platform__Windows}
    )


################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/ariapch.h>"
    )

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Aria)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
    )
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG "Aria"
    TARGET_NAME_DIST "Aria"
    TARGET_NAME_RELEASE "Aria"
    )
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-windows-x86_64/Aria/"
    OUTPUT_DIRECTORY_DIST "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Dist-windows-x86_64/Aria/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-windows-x86_64/Aria/"
    )
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
    MultiThreadedDebugDLL
    >
    $<$<CONFIG:Dist>:
    MultiThreadedDLL
    >
    $<$<CONFIG:Release>:
    MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Dist>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/AriaImgui;"
    )

if (DEFINED ENV{VULKAN_SDK})
  target_include_directories(${PROJECT_NAME} PUBLIC "$ENV{VULKAN_SDK}/Include")
else ()
  target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../ProgramData/scoop/apps/vulkan/current/Include")
endif ()

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
    "ARIA_DEBUG;"
    "ARIA_ENABLE_ASSERTS"
    ">"
    "$<$<CONFIG:Dist>:"
    "ARIA_DIST"
    ">"
    "$<$<CONFIG:Release>:"
    "ARIA_RELEASE"
    ">"
    "GLM_FORCE_SILENT_WARNINGS;"
    "ARIA_PLATFORM_WINDOWS;"
    "ARIA_BUILD_DLL;"
    "UNICODE;"
    "_UNICODE"
    )

################################################################################
# Compile and link options
################################################################################
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>:
      /Od;
      /ZI
      >
      $<$<CONFIG:Dist>:
      /Gm-;
      /Ox;
      /Oi;
      /GF;
      /Gy;
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      >
      $<$<CONFIG:Release>:
      /Gm-;
      /Ox;
      /Oi;
      /GF;
      /Gy;
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      >
      /std:c++17;
      /W4;
      ${DEFAULT_CXX_EXCEPTION_HANDLING}
      )
  target_link_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Dist>:
      /OPT:REF;
      /OPT:ICF
      >
      $<$<CONFIG:Release>:
      /OPT:REF;
      /OPT:ICF
      >
      /SUBSYSTEM:WINDOWS
      )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>:
      -g
      -O0
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      >

      $<$<CONFIG:Dist>:
      -O2
      -ffunction-sections
      -fdata-sections
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      >

      $<$<CONFIG:Release>:
      -O2
      -ffunction-sections
      -fdata-sections
      ${DEFAULT_CXX_EXCEPTION_HANDLING}
      >

      -Wall
      -Wextra
      -Wpedantic
      -std=c++17
      )
  target_link_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Release>:
      -flto
      -Wl,--gc-sections
      -Wl,--icf=safe
      -Wl,--strip-all
      -Wl,--plugin-opt=eliminate-unused-dwarf-types
      >
      $<$<CONFIG:Dist>:
      -flto
      -Wl,--gc-sections
      -Wl,--icf=safe
      -Wl,--strip-all
      >
      )
endif ()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    glm::glm
    glad::glad
    vk-bootstrap::vk-bootstrap
    spdlog::spdlog
    imgui
    )

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "opengl32"
    )
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(vk-bootstrap CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory("vendor/AriaImgui")