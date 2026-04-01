workspace "NexusEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NexusEngine"
	location "NexusEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	buildoptions "/utf-8"

	pchheader "nxpch.h"
	pchsource "%{prj.name}/src/nxpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NX_PLATFORM_WINDOWS",
			"NX_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	buildoptions "/utf-8"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"NexusEngine/vendor/spdlog/include",
		"NexusEngine/src",
	}

	links
	{
		"NexusEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NX_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"
