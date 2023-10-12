
dofile("utiltools.lua")

work_dir = "../build"
workspace "HelloMyCpp" -- 解决方案
	location( work_dir )
    startproject( "Lesson1_FileCrypto" )
    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "Win32",
        "Win64"
    }

    filter "platforms:Win32"
        system "Windows"
        architecture "x32"

    filter "platforms:Win64"
        system "Windows"
        architecture "x86_64"


proj_dir = work_dir .. "/%{prj.name}"
bin_dir = proj_dir .. "/bin/%{cfg.buildcfg}"
obj_dir = proj_dir .. "/obj/%{cfg.buildcfg}"

function CreateProject(projname, projtype, linkerObj)
    project( projname )
        location( proj_dir )
        language( "C++" )
        kind( projtype )

        objdir (  obj_dir )
        targetdir ( bin_dir )

        if linkerObj ~= nil then
            links ( linkerObj.name )
            includedirs
            { 
                linkerObj.includes
            }
        end

        files
        {
            "../client/%{prj.name}/**.h",  -- 当前文件夹所有.h文件
            "../client/%{prj.name}/**.cpp" -- 当前文件夹所有.cpp文件
        }
        
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} \"../../bin/\"")
        }
end

-- 静态库
CreateProject("Frameworks", "StaticLib")

local linkerObj = {
    name = "Frameworks", 
    includes = "../client/Frameworks"
}
CreateProject("Lesson1_FileCrypto", "ConsoleApp", linkerObj)

