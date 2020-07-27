-- The name of your workspace will be used, for example, to name the Visual Studio .sln file generated by Premake.
workspace "Premake"
	-- We set the location of the files Premake will generate
	-- location ""
	location "Generated"
	
	-- We indicate that all the projects are C++ only
	language "C++"
	
	-- We will compile for x86_64. You can change this to x86 for 32 bit builds.
	architecture "x86_64"

    cppdialect "C++17"
    defines "PROFILE"   
	
	-- Configurations are often used to store some compiler / linker settings together.
    -- The Debug configuration will be used by us while debugging.
    -- The optimized Release configuration will be used when shipping the app.
	configurations { "Debug", "Release" }
	
	filter { "configurations:Debug" }
    -- We want debug symbols in our debug config
        defines {"NDEBUG", "PROFILE", "SUPRESS"}
        symbols "On"
		buildoptions {"-Wdeprecated-declarations"}
		-- , "-ftime-report"
        
        -- We now only set settings for Release
    filter { "configurations:Release" }
		-- Release should be optimized
        symbols "On"
		optimize "On"
	
	-- Reset the filter for other settings
	filter { }
	
	-- Here we use some "tokens" (the things between %{ ... }). They will be replaced by Premake
	-- automatically when configuring the projects.
	-- * %{prj.name} will be replaced by "ExampleLib" / "App" / "UnitTests"
	--  * %{cfg.longname} will be replaced by "Debug" or "Release" depending on the configuration
	-- The path is relative to *this* folder
	targetdir ("Build/Bin/%{prj.name}/%{cfg.longname}")
	-- targetdir ""
    objdir ("Build/Obj/%{prj.name}/%{cfg.longname}")
    
-- This function includes GLFW's header files
function includeGLFW()
	includedirs "Libraries/GLFW/Include"
end	

function includeEntt()
	includedirs "Libraries/Entt/single_include"
end

-- This function includes GLM's header files
function includeGLM()
	includedirs "Libraries"
end	

-- This function links statically against GLFW
function linkGLFW()
	libdirs "Libraries/GLFW/Lib"
	
	-- Our static lib should not link against GLFW
	filter "kind:not StaticLib"
		links "glfw"
end

-- We will use this function to include Catch
function includeCatch()
	-- Catch is header-only, we need just the Catch.hpp header
	includedirs "Libraries/Catch"
	
	-- We can also configure Catch through defines
	defines "CATCH_CPP11_OR_GREATER"
end


-- Our first project, the static library
project "Entropy"
	-- kind is used to indicate the type of this project.
	kind "SharedLib"

	toolset "clang"
	
	-- We specify where the source files are.
	-- It would be better to separate header files in a folder and sources
	-- in another, but for our simple project we will put everything in the same place.
	-- Note: ** means recurse in subdirectories, so it will get all the files in ExampleLib/
	files ("Projects/Entropy/**.cpp", "Projects/Entropy/pch.gch")


	filter { "system:macosx" }
		links { "/System/Library/Frameworks/OpenGL.framework" }

	filter {}

	-- We need GLFW, so we include it
	includeGLFW()
	
	includeGLM()

	includeEntt()

	linkGLFW()

	includedirs "Libraries/Cute"
    
function useEntropy()
    -- The library's public headers
    includedirs "Projects/Entropy"
    
    -- We link against a library that's in the same workspace, so we can just
    -- use the project name - premake is really smart and will handle everything for us.
    links "Entropy"
    
    -- Users of Entropy need to link GLFW
    linkGLFW()
end

project "UnitTests"
	-- Catch prints information to the console
	kind "ConsoleApp"
	
    files "Projects/EntropyUnitTests/**"
    
    includedirs "Libraries/Cute"
	
    includeCatch()
    includeGLM()
	useEntropy()


-- The windowed app
project "Trespass"
	kind "WindowedApp"
	files "Projects/Trespass/**"

	targetname "play"

	-- We also need the headers
	includedirs "Projects/Entropy"
	
	useEntropy()

	includeGLM()

	
	-- Now we need to add the OpenGL system libraries
	
	filter { "system:windows" }
        links { "OpenGL32" }
        
	filter { "system:macosx" }
		links { "OpenGL.framework" }

	-- 	filter { "system:not windows" }
	-- links { "GL" }	
        
		
        
