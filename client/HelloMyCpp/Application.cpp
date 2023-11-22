#include "Application.h"
#include "CCLogSystem.h"
#include "LuaTest.h"
#include "CCLogSystem.h"
#include "FileManager.h"

using namespace Fancy;

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
	g_fileManager = FileManager::GetInstance();

	if (CCLogSysInit() == 0)
	{
		LogAddConsoleHandler(LOGL_DEBUG | LOGL_INFO);
		std::string fullpath = g_fileManager->getFullPath("bin/game.log");
		LogAddFileHandler(fullpath.c_str(), LOGL_DEBUG | LOGL_INFO);
	}
}

void Application::Run()
{
	//Fancy::lua_test0();
	//Fancy::lua_test1();
	//Fancy::lua_test2();
	//Fancy::lua_test3();

	Fancy::lua_test4();
	Fancy::lua_test5();
	Fancy::lua_test6();
	/*
	for (int index = 0; index < 99999; index++)
	{
		CCLOG_INFO("Fxkk=======>>> %d", index);
	}
	*/
}