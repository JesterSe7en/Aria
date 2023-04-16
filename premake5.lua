workspace "Aria"
  architecture "x64"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.archtecture}"

-- Include directories relative to root folder (soln dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Aria/vendor/GLFW/include"

include "Aria/vendor/GLFW"

project "Aria"
  location "Aria"
  kind "SharedLib"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "ariapch.h"
  pchsource "Aria/src/ariapch.cpp" -- needed for VS

  files 
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
  }

  links
  {
    "GLFW",
    "opengl32.lib",
  }

  filter "system:windows"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    defines 
    {
      "ARIA_PLATFORM_WINDOWS",
      "ARIA_BUILD_DLL",
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }
  
  filter "configurations:Debug"
    defines "ARIA_DEBUG"
    symbols "On"
  
  filter "configurations:Release"
    defines "ARIA_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "ARIA_DIST"
    optimize "On"

  -- filter { "system:windows", "configurations:Release"}
  --   buildoptions "/MT"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files 
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  includedirs
  {
    "Aria/vendor/spdlog/include",
    "Aria/src"
  }

  links
  {
    "Aria"
  }

  filter "system:windows"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    defines 
    {
      "ARIA_PLATFORM_WINDOWS",
    }

  filter "configurations:Debug"
    defines "ARIA_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "ARIA_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "ARIA_DIST"
    optimize "On"
