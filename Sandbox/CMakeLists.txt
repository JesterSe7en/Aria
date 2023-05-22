set(PROJECT_NAME Sandbox)

################################################################################
# Source groups
################################################################################
set(Headers
    "src/ExampleLayer.h"
    "src/VulkanLayer.h"
    )
source_group("Headers" FILES ${Headers})

set(Sources
    "src/ExampleLayer.cpp"
    "src/SandboxApp.cpp"
    "src/VulkanLayer.cpp"
    )
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
    )

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Sandbox)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
    )
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG "Sandbox"
    TARGET_NAME_DIST "Sandbox"
    TARGET_NAME_RELEASE "Sandbox"
    )
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-windows-x86_64/Sandbox/"
    OUTPUT_DIRECTORY_DIST "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Dist-windows-x86_64/Sandbox/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-windows-x86_64/Sandbox/"
    )
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
    MultiThreadedDebug
    >
    $<$<CONFIG:Dist>:
    MultiThreaded
    >
    $<$<CONFIG:Release>:
    MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Dist>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Aria/vendor/spdlog/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Aria/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Aria/vendor/glm;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Aria/vendor/Glad/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Aria/vendor/imgui;"
    "$ENV{VK_SDK_PATH}/Include"
    )

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
    "ARIA_DEBUG"
    ">"
    "$<$<CONFIG:Dist>:"
    "ARIA_DIST"
    ">"
    "$<$<CONFIG:Release>:"
    "ARIA_RELEASE"
    ">"
    "ARIA_PLATFORM_WINDOWS;"
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
      /W3;
      ${DEFAULT_CXX_EXCEPTION_HANDLING};
      /Y-
      )
  target_link_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>:
      /DEBUG;
      /INCREMENTAL
      >
      $<$<CONFIG:Dist>:
      /DEBUG:FULL;
      /OPT:REF;
      /OPT:ICF;
      /INCREMENTAL:NO
      >
      $<$<CONFIG:Release>:
      /DEBUG:FULL;
      /OPT:REF;
      /OPT:ICF;
      /INCREMENTAL:NO
      >
      /SUBSYSTEM:CONSOLE
      )
else ()
  target_compile_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>:
      -O0;
      -g;
      -Wall;
      -Wextra;
      -pedantic;
      -fno-inline-functions;
      -fno-omit-frame-pointer
      >
      $<$<CONFIG:Dist>:
      -O2;
      -DNDEBUG;
      -fstack-protector-strong;
      -pie;
      -fvisibility=hidden;
      -fdata-sections;
      -ffunction-sections;
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      >
      $<$<CONFIG:Release>:
      -O2;
      -g;
      -DNDEBUG;
      -fvisibility=hidden;
      -fdata-sections;
      -ffunction-sections;
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      >
      -std=c++17;
      ${DEFAULT_CXX_EXCEPTION_HANDLING}
      )
endif ()

################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Aria
    )

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "vulkan-1;"
    "$(CoreLibraryDependencies)"
    )
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "$ENV{VK_SDK_PATH}/Lib"
    )

