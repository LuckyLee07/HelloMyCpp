
--dofile("utiltools.lua")

work_dir = "build"
workspace "HelloMyCpp" -- 解决方案
	location( work_dir )
    startproject( "HelloMyCpp" ) -- 设置启动项

    platforms { "Win32", "Win64" }

    configurations { "Debug", "Release" }

    filter "platforms:Win32"
        system "Windows"
        architecture "x32"

    filter "platforms:Win64"
        system "Windows"
        architecture "x86_64"


targe_dir = "./bin/"
proj_dir = work_dir .. "/%{prj.name}"
obj_dir = proj_dir .. "/obj/%{cfg.buildcfg}"


function CreateProject(projname, projtype, srcfiles, linkers)
    project( projname )
        location( proj_dir )
        language( "C++" )
        kind( projtype )

        objdir (  obj_dir )
        targetdir ( targe_dir )

        files( srcfiles )
        
        disablewarnings { "4996" }

        if linkers ~= nil then
            for index, linker in pairs(linkers) do
                libdirs  ( targe_dir )
                links ( linker.lib_name )
                includedirs( linker.lib_include )
            end
        end
end

-- 现有第三方库
local Linkers = { }
Linkers['lib_lua'] = 
{
    lib_name = "liblua", 
    lib_include = "client/external/lua/lua",
}
Linkers['lib_tolua'] = 
{
    lib_name = "libtolua", 
    lib_include = "client/external/lua/tolua",
}
Linkers['lib_self'] = 
{
    lib_name = "frameworks", 
    lib_include = "client/Frameworks",
}
--------------External--------------
group "External" -- 先创建文件夹组 --

-- 静态库/第三方库
CreateProject("liblua", "StaticLib", {
    "client/external/lua/lua/**.h",
    "client/external/lua/lua/**.c",
})
CreateProject("libtolua", "StaticLib", {
    "client/external/lua/tolua/**.h",
    "client/external/lua/tolua/**.c",
}, { Linkers.lib_lua })

group "" -- 结束group文件夹组 --
--------------External--------------

CreateProject("frameworks", "StaticLib", {
    "client/%{prj.name}/**.h",
    "client/%{prj.name}/**.cpp"
}, { Linkers.lib_lua, Linkers.lib_tolua })

CreateProject("tolua++", "ConsoleApp", {
    "client/external/lua/tolua/**.h",
    "client/external/lua/tolua/**.c",
}, { Linkers.lib_lua })

CreateProject("HelloMyCpp", "ConsoleApp", {
    "client/%{prj.name}/**.h",
    "client/%{prj.name}/**.cpp",
}, Linkers )