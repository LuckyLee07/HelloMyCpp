
--dofile("utiltools.lua")

work_dir = "build"
workspace "HelloMyCpp" -- 解决方案
	location( work_dir )
    startproject( "Lesson0_HelloLua" )
    configurations { "Debug", "Release" }

    filter "platforms:Win32"
        system "Windows"
        architecture "x32"
    filter{}
    filter "platforms:Win64"
        system "Windows"
        architecture "x86_64"
    filter{}
    filter "platforms:Mac"
        system "macosx"
        architecture "arm64"
    filter{}

proj_dir = work_dir .. "/%{prj.name}"
bin_dir = proj_dir .. "/bin/%{cfg.buildcfg}"
obj_dir = proj_dir .. "/obj/%{cfg.buildcfg}"

function CreateProject(projname, projtype, linkers)
    project( projname )
        location( proj_dir )
        language( "C++" )
        kind( projtype )

        objdir (  obj_dir )
        targetdir ( bin_dir )

        for index, linker in pairs(linkers) do
            links ( linker.libn )
            libdirs  ( linker.libdirs )
            sysincludedirs( linker.includes )
        end

        files
        {
            "client/%{prj.name}/**.h",  -- 当前文件夹所有.h文件
            "client/%{prj.name}/**.cpp" -- 当前文件夹所有.cpp文件
        }

        -- Platform-specific settings
        filter "system:macosx"
            -- macOS specific settings
            -- You can add macOS specific include directories, defines, etc. here
        
        filter "system:windows"
            postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} \"../../bin/\"" }
end

function Create3rdLibs(libsname, projtype, srcfiles, linkers)
    project( libsname )
        location( proj_dir )
        language( "C++" )
        kind( projtype )

        objdir (  obj_dir )
        targetdir ( bin_dir )

        files( srcfiles )
        
        filter "system:macosx"
            targetextension ".a"
        filter{}
        filter "system:windows"
            postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} \"../../bin/\"" }
        filter{}

        if not linkers then return end

        for index, linker in pairs(linkers) do
            links ( linker.libn )
            libdirs  ( linker.libdirs )
            sysincludedirs( linker.includes )
        end
end

-- 现有库
local Linkers = 
{
    {
        libdirs = "bin",
        libn = "libLua", 
        includes = "client/external/lua/lua",
    },
    {
        libdirs = "bin",
        libn = "libFrameworks", 
        includes = "client/Frameworks",
    },
}

-- 静态库/第三方库
Create3rdLibs("libLua", "StaticLib", {
    "client/external/lua/lua/**.h",
    "client/external/lua/lua/**.c",
})
Create3rdLibs("Frameworks", "StaticLib", {
    "client/%{prj.name}/**.h",
    "client/%{prj.name}/**.cpp"
}, { Linkers[1] })

-- 实际项目
CreateProject("Lesson0_HelloLua", "ConsoleApp", { Linkers[1], Linkers[2] })

CreateProject("Lesson1_FileCrypto", "ConsoleApp", { Linkers[2] })

