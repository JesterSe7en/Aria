# Aria


Ongoing personal side project - Based off The Cherno's game engine series.  Branched off from OpenGL 4.6 to Vulkan API 1.3.

Learning tool to understand the game engine architecture and jargon.

Swapped over to CMake for more cross-platform compatibility.
 - Currently supports MSVC and Clang compilers, untested with GCC
 - Utilizing Ninja's build system for speed and cross-platform support

Utilizes the following third-party libraries
- Glad: to manage and load OpenGL and OpenGL-related extensions
- GLFW: cross-platform windowing system
- glm: for all the nessecary matrix math stuff
- imgui: for easy ui creation for debugging purposes
- spdlog: provides logging
- stb_image: image processor
