#include "LuaInterface.h"
#include "CCLogSystem.h"
#include <windows.h>
#include <tchar.h>
#include <codecvt>
#include <assert.h>

LuaInterface::LuaInterface()
{
}

LuaInterface::~LuaInterface()
{
}

void LuaInterface::log(const char* msg)
{
	if (msg == NULL) return;
	
	CCLUA_INFO("@lua:%s", msg);
}

const char* LuaInterface::getModuleFileName()
{
	if (m_moduleFileName.empty())
	{
		TCHAR szPath[MAX_PATH];
		if (GetModuleFileName(NULL, szPath, MAX_PATH))
		{
			size_t length = _tcslen(szPath);
			std::wstring temp = szPath;

			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			m_moduleFileName = converter.to_bytes(temp);
		}
		else
		{
			CCLOG_ERROR("GetModuleFileName failed (%d)", GetLastError());
			assert(NULL);
		}
	}
	return m_moduleFileName.c_str();
}

