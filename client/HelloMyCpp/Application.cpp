#include "Application.h"
#include "LuaTest.h"
#include "CCLogSystem.h"
#include "FileManager.h"
#include "ScriptLuaVM.h"
#include "LuaInterface.h"

#include "tolua++.h"

using namespace Fancy;

extern int tolua_open_all(lua_State* tolua_S);

Application::Application()
{
	Initialize();
}

Application::~Application()
{
	CCLogSysRelease();
}

void Application::Initialize()
{
	// 开启打印系统 
	if (CCLogSysInit() == 0)
	{
		LogAddConsoleHandler(LOGL_DEBUG | LOGL_INFO | LOGL_ERROR);
		std::string fullpath = GetFileManager()->getFullPath("bin/game.log");
		LogAddFileHandler(fullpath.c_str(), LOGL_DEBUG | LOGL_INFO | LOGL_ERROR);
	}
	
	// 设置ToLua对象 
	ScriptLuaVM* pScriptVM = ScriptLuaVM::GetInstance();
	tolua_open_all(pScriptVM->getLuaState());

	// 设置lua可用的c++对象 
	pScriptVM->setUserTypePointer("LuaInterface", "LuaInterface", LuaInterface::GetInstance());
}

void Application::Run()
{
	//Fancy::lua_test0();
	//Fancy::lua_test1();
	//Fancy::lua_test2();
	//Fancy::lua_test3();
	//Fancy::lua_test4();
	//Fancy::lua_test5();

	Fancy::lua_test6();
}