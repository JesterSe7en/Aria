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
IncludeDir["Glad"] = "Aria/vendor/Glad/include"
IncludeDir["imgui"] = "Aria/vendor/imgui"
IncludeDir["glm"] = "Aria/vendor/glm"

include "Aria/vendor/GLFW"
include "Aria/vendor/Glad"
include "Aria/vendor/imgui"

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
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.inl"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.imgui}",
    "%{IncludeDir.glm}"
  }

  links
  {
    "GLFW",
    "Glad",
    "imgui",
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
    buildoptions "/MDd"
    symbols "On"
  
  filter "configurations:Release"
    defines "ARIA_RELEASE"
    buildoptions "/MD"
    optimize "On"

  filter "configurations:Dist"
    defines "ARIA_DIST"
    buildoptions "/MD"
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
    "Aria/src",
    "%{IncludeDir.glm}"
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
    buildoptions "/MDd"
    symbols "On"

  filter "configurations:Release"
    defines "ARIA_RELEASE"
    buildoptions "/MD"
    optimize "On"

  filter "configurations:Dist"
    defines "ARIA_DIST"
    buildoptions "/MD"
    optimize "On"
