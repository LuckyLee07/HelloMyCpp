#include "ScriptLuaVM.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <assert.h>
#include <iostream>
#include <string.h>
#include "FileManager.h"


static const char* lua_readfile(lua_State* L, void* data, size_t* size)
{
	FILE* pfile = (FILE*)data;

	if (!feof(pfile))
	{
		static char buffer[1024]; //缓冲区大小
		size_t bytesRead = fread(buffer, 1, sizeof(buffer), pfile);
		if (bytesRead > 0)
		{
			*size = bytesRead;
			return buffer;
		}
	}
	return NULL;
}

ScriptLuaVM::ScriptLuaVM()
	:m_pState(nullptr)
{
	m_pState = luaL_newstate();

	luaL_openlibs(m_pState);

	addLuaLoader(m_pState, myLuaLoader);
}

ScriptLuaVM::~ScriptLuaVM()
{
	if (m_pState) lua_close(m_pState);
}

bool ScriptLuaVM::callFile(const char *fpath)
{
	std::string rootpath = g_fileManager->getRootPath();
	const char* fullpath = std::string(rootpath + fpath).c_str();

	FILE* pfile = fopen(fullpath, "rb");
	assert(pfile != NULL);
	if (lua_load(m_pState, (lua_Reader)lua_readfile, pfile, fullpath) != LUA_OK)
	{
		const char *perr = lua_tostring(m_pState, -1);
		fprintf(stderr, "load failed: %s | error: %s\n", fpath, perr);
		lua_pop(m_pState, 1);
		//showLuaError(m_pState, perr);
		return false;
	}

	if (lua_pcall(m_pState, 0, 0, 0) != LUA_OK)
	{
		const char *perr = lua_tostring(m_pState, -1);
		fprintf(stderr, "call failed: %s | error: %s\n", fpath, perr);
		lua_pop(m_pState, 1);
		//showLuaError(m_pState, perr);
		return false;
	}

	return true;
}

bool ScriptLuaVM::callString(const char *szLua_code)
{
	if (szLua_code == nullptr) return false;
	
	int result = luaL_dostring(m_pState, szLua_code);
	if (result != LUA_OK)
	{	
		const char *perr = lua_tostring(m_pState, -1);
		fprintf(stderr, "[script error] %s \n", perr);
		lua_pop(m_pState, 1);
		//showLuaError(m_pState, perr);
		return false;
	}
	return true;
}

void ScriptLuaVM::showLuaError(lua_State* L, const char* msg)
{
	//send_LuaErrMsg(msg); //上传lua报错信息
	
	const char* luaTrack = lua_get_traceback(L);
	if (!luaTrack) return;

	const char* title = "lua error";
	std::string errStr = msg;
	errStr.append("\nluaTraceBack:\n");
	errStr.append(luaTrack);
	
	//弹出lua报错弹窗
	//pop_MessageBox(errStr.c_str(), title);
}


//------------require自定义loader_Lua------------
int myLuaLoader(lua_State * m_state)
{
	const std::string LuaExt = ".lua";
	const char* luapath = lua_tostring(m_state, 1);

	//兼容"socket.core"，这种写法
	std::string filename = luapath;
	size_t pos = filename.rfind(LuaExt);
	if (pos != std::string::npos && pos == (filename.length() - LuaExt.length()))
	{
		filename = filename.substr(0, pos);
	}

	pos = filename.find_first_of('.');
	while (pos != std::string::npos)
	{
		filename.replace(pos, 1, "/");
		pos = filename.find_first_of('.');
	}

	filename += LuaExt; //重新补上Lua后缀
	std::string rootpath = g_fileManager->getRootPath();
	const char* fullpath = std::string(rootpath + filename).c_str();

	FILE* pfile = fopen(fullpath, "rb");
	if (pfile != NULL)
	{
		//if (luaL_loadfile(m_state, fullpath) != LUA_OK)
		if (lua_load(m_state, (lua_Reader)lua_readfile, pfile, fullpath) != LUA_OK)
		{
			const char *perr = lua_tostring(m_state, -1);
			fprintf(stderr, "require lua %s error: %s\n", filename.c_str(), perr);
			lua_pop(m_state, 1);
		}
	}
	else
	{
		fprintf(stderr, "requrie not find [%s]\n", filename.c_str());
	}
	
	return 1;
}

//参考：https://blog.csdn.net/Yueya_Shanhua/article/details/52241544
void addLuaLoader(lua_State* m_state, lua_CFunction func)
{
	if (func == NULL) return;
	
	// stackcontent after the invoking of the function
	// getloader table（获得lua预加载好的package这个table，并将这个table压桟）
	lua_getglobal(m_state, "package"); /* L: package */
	// 获得package这个table里面的loaders元素，并将其压桟，这个loaders也是一个table，table里的元素全是方法
	lua_getfield(m_state, -1, "loaders"); /* L: package, loaders */
	// insertloader into index 2（将自己的loader方法压桟）
	lua_pushcfunction(m_state, func); /* L: package, loaders, func */
	// 遍历loaders这个table，将其下标为2的元素替换为自己的loader方法
	for (int i = lua_objlen(m_state, -2) + 1; i > 2; --i)
	{
		lua_rawgeti(m_state, -2, i - 1); /* L: package, loaders, func, function */
		//we call lua_rawgeti, so the loader table now is at -3
		lua_rawseti(m_state, -3, i); /* L: package, loaders, func */
	}
	lua_rawseti(m_state, -2, 2); /* L: package, loaders */
	// setloaders into package
	lua_setfield(m_state, -2, "loaders"); /* L: package */
	lua_pop(m_state, 1);
}
