#include "LuaInterface.h"
#include "CCLogSystem.h"

LuaInterface::LuaInterface()
{
}

LuaInterface::~LuaInterface()
{
}

void LuaInterface::log(const char* msg)
{
	if (msg == NULL) return;
	
	CCLUA_INFO("@lua:%s\n", msg);
}