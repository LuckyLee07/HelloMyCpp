#ifndef __SCRIPT_LUA_VM_H__
#define __SCRIPT_LUA_VM_H__

#include "Singleton.h"

#define LUA_OK	0
#define LUA_VER	"Lua 5.1.4"

extern "C" {
#include <lua.h>
}
//struct lua_State;
class ScriptLuaVM : public Singleton<ScriptLuaVM>
{
public:
	ScriptLuaVM(void);
	~ScriptLuaVM();

	bool callFile(const char *fpath);

	bool callString(const char *szLua_code);

	lua_State* getLuaState() { return m_pState; }

	static void showLuaError(lua_State* L, const char* msg);

private:
	lua_State *m_pState;
};

//------------require�Զ���loader_Lua------------
extern int myLuaLoader(lua_State * m_state);
extern void addLuaLoader(lua_State* L, lua_CFunction func);

#endif //__SCRIPT_LUA_VM_H__