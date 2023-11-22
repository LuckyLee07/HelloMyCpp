
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


function do_link( linkers )
    for index, linker in pairs(linkers) do
        links ( linker.lib_name )
        libdirs  ( linker.lib_dirs )
        sysincludedirs( linker.lib_include )
    end
end


function CreateProject(projname, projtype, srcfiles, linkers)
    project( projname )
        location( proj_dir )
        language( "C++" )
        kind( projtype )

        objdir (  obj_dir )
        targetdir ( bin_dir )

        files( srcfiles )
        
        filter "system:macosx"
            if projtype == "StaticLib" then 
                targetextension ".a"
            end
            postbuildcommands { "cp %{cfg.buildtarget.relpath} ../../bin/" }
        filter{}
        filter "system:windows"
            postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} \"../../bin/\"" }
        filter{}

        if linkers then do_link(linkers) end
end


-- 现有第三方库
local Linkers = {
    {
        lib_dirs = "./bin",
        lib_name = "LuaLib", 
        lib_include = "client/external/lua/lua",
    },
    {
        lib_dirs = "./bin",
        lib_name = "Frameworks", 
        lib_include = "client/Frameworks",
    },
}

-- 静态库/第三方库
CreateProject("LuaLib", "StaticLib", {
    "client/external/lua/lua/**.h",
    "client/external/lua/lua/**.c",
})
CreateProject("Frameworks", "StaticLib", {
    "client/%{prj.name}/**.h",
    "client/%{prj.name}/**.cpp"
}, { Linkers[1] })


-- 实际项目
CreateProject("HelloMyCpp", "ConsoleApp", {
    "client/%{prj.name}/**.h",
    "client/%{prj.name}/**.cpp"
}, { Linkers[1], Linkers[2] })


