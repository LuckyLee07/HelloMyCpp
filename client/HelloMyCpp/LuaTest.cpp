#include "LuaTest.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <complex>
#include <iostream>
#include "ScriptLuaVM.h"
#include "FileManager.h"
#include "CCLogSystem.h"

namespace Fancy
{
	extern const double PI = 3.14159;

	extern int calcComplex(lua_State* L)
	{
		double r = luaL_checknumber(L, 1);
		double i = luaL_checknumber(L, 2);

		std::complex<double> c(r, i);

		//存入绝对值
		lua_pushnumber(L, std::abs(c));
		//存入角度
		lua_pushnumber(L, std::arg(c) * 180.0 / PI);

		return 2;
	}

	void lua_test0()
	{
		ScriptLuaVM::GetInstance()->callString("print('Hello, Lua!')\npr()");
	}

	void lua_test1()
	{
		const char *szLua_code =
			"r = string.gsub(c_Str, c_Mode, c_Tag)\n"
			"u = string.upper(r)";

        const char *szMode = "(%w+)%s*=%s*(%w+)";
        const char *szStr = "key1 = value1 key2 = value2";
        const char *szTag = "<%1>%2</%1>";

		ScriptLuaVM *pScriptVM = ScriptLuaVM::GetInstance();
		lua_State *L = pScriptVM->getLuaState();
		
		lua_pushstring(L, szMode);
		lua_setglobal(L, "c_Mode");
		lua_pushstring(L, szTag);
		lua_setglobal(L, "c_Tag");
		lua_pushstring(L, szStr);
		lua_setglobal(L, "c_Str");

		if (pScriptVM->callString(szLua_code))
		{
			lua_getglobal(L, "r");
			std::cout << "r = " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);

			lua_getglobal(L, "u");
			std::cout << "u = " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}

		delete pScriptVM;
	}

	void lua_test2()
	{
        const char *szLua_code = "x = {} --用于存放结果的table\n"
			"x[1], x[2] = string.gsub(c.Str, c.Mode, c.Tag) --x[1]里是结果，x[2]里是替换次数\n"
			"x.u = string.upper(x[1])";

        const char *szMode = "(%w+)%s*=%s*(%w+)";
        const char *szStr = "key1 = value1 key2 = value2";
        const char *szTag = "<%1>%2</%1>";

		ScriptLuaVM *pScriptVM = ScriptLuaVM::GetInstance();
		lua_State *L = pScriptVM->getLuaState();

		lua_newtable(L);
		lua_pushstring(L, "Mode");	//key
		lua_pushstring(L, szMode);	//value

		//设置newtable[Mode]=szMode
		//由于上面两次压栈，现在table元素排在栈顶往下数第三的位置
		//lua_settable会自己弹出上面压入的key和value
		lua_settable(L, -3);

		lua_pushstring(L, "Tag");	//key
		lua_pushstring(L, szTag);	//value
		lua_settable(L, -3);//设置newtable[Tag]=szTag

		lua_pushstring(L, "Str");	//key
		lua_pushstring(L, szStr);	//value
		lua_settable(L, -3);//设置newtable[Str]=szStr

		lua_setglobal(L, "c");//将栈顶元素置为Lua中的全局变量c

		if (pScriptVM->callString(szLua_code))
		{
			lua_getglobal(L, "x");
			if (lua_istable(L, -1))
			{
				lua_pushstring(L, "u");//key
				lua_gettable(L, -2);

				std::cout << "x.u = " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);

				//取得x[1]和x[2]
				for (int index = 1; index <= 2; index++)
				{
					lua_pushnumber(L, index);
					lua_gettable(L, -2);

					std::cout << "x[" << index << "] = " << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			lua_pop(L, 1);//弹出栈顶的x
		}
		delete pScriptVM;
	}

	void lua_test3()
	{
        const char *szLua_code = "x = {} --用于存放结果的table\n"
			"x[1], x[2] = string.gsub(c.Str, c.Mode, c.Tag) --x[1]里是结果，x[2]里是替换次数\n"
			"x.u = string.upper(x[1])";

        const char *szMode = "(%w+)%s*=%s*(%w+)";
        const char *szStr = "key1 = value1 key2 = value2";
        const char *szTag = "<%1>%2</%1>";

		ScriptLuaVM *pScriptVM = ScriptLuaVM::GetInstance();
		lua_State *L = pScriptVM->getLuaState();
		lua_newtable(L);

		//newtable[Tag]=szTag设置
		lua_pushstring(L, szMode);
		lua_setfield(L, -2, "Mode");

		lua_pushstring(L, szTag);
		lua_setfield(L, -2, "Tag");

		lua_pushstring(L, szStr);
		lua_setfield(L, -2, "Str");

		lua_setglobal(L, "c");//将栈顶元素置为Lua中的全局变量c

		if (pScriptVM->callString(szLua_code))
		{
			lua_getglobal(L, "x");
			if (lua_istable(L, -1))
			{
				lua_getfield(L, -1, "u"); // x.u取值与上例的差异
				std::cout << "x.u = " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);

				//取得x[1]和x[2]
				for (int index = 1; index <= 2; index++)
				{
					lua_rawgeti(L, -1, index);// x[i]取值与上例的差异
					std::cout << "x[" << index << "] = " << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			lua_pop(L, 1);//弹出栈顶的x
		}
		delete pScriptVM;
	}

	void lua_test4()
	{
        const char *szLua_code =
			"function gsub(Str, Mode, Tag)\n"
			"	a, b = string.gsub(Str, Mode, Tag)\n"
			"	c = string.upper(a)\n"
			"	return a, b, c\n"
			"end";

        const char *szMode = "(%w+)%s*=%s*(%w+)";
        const char *szStr = "key1 = value1 key2 = value2";
        const char *szTag = "<%1>%2</%1>";
		
		ScriptLuaVM *pScriptVM = ScriptLuaVM::GetInstance();
		lua_State *L = pScriptVM->getLuaState();

		if (pScriptVM->callString(szLua_code))
		{
			//Lua执行后取得全局变量的值
			lua_getglobal(L, "gsub");

			if (lua_isfunction(L, -1)) //确认下是个函数
			{
				//依次放入三个参数
				lua_pushstring(L, szStr);
				lua_pushstring(L, szMode);
				lua_pushstring(L, szTag);

				//调用, 我们有3个参数，但我们可以只需2个
				//如果 errfunc 是 0 ，返回在栈顶的错误信息就和原始错误信息完全一致
				if (lua_pcall(L, 3, 2, 0) != LUA_OK)
				{
					const char *perr = lua_tostring(L, -1);
					fprintf(stderr, "[script error] %s \n", perr);
					lua_pop(L, 1);
				}
				else
				{
					// 调用正确，得到两个结果 注意在栈里的顺序
					std::cout << "a = " << lua_tostring(L, -2) << std::endl;
					std::cout << "b = " << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 2);
				}
			}
		}
		delete pScriptVM;
	}

	void lua_test5()
	{
        const char *szLua_code =
			"v, a = CalcComplex(3, 4)\n"
			"print('Fxkk===>>', v, a)";

		ScriptLuaVM *pScriptVM = ScriptLuaVM::GetInstance();
		lua_State *L = pScriptVM->getLuaState();

		//lua_pushcfunction(L, calcComplex);
		//lua_setglobal(L, "CalcComplex");
		// 以上代码可简写如下
		lua_register(L, "CalcComplex", calcComplex);

		// 执行lua代码
		pScriptVM->callString(szLua_code);
		
		delete pScriptVM;
	}

	void lua_test6()
	{
		ScriptLuaVM *pScriptVM = ScriptLuaVM::GetInstance();
		pScriptVM->callFile("res/scripts/script_init.lua");

		pScriptVM->callString("sayhello('Hello', 11, 99)");

		int result = 0;
		pScriptVM->callFunction("sayhello", "sis>i", "HelloWorld", 11, "hh", &result);
		pScriptVM->callFunction("sayhello", "sii>i", "HelloWorld", 11, 99, &result);
		CCLOG_INFO("Fxkk======>>> %d", result);
		CCLOG_INFO("Fxkk======>>> %d", 77);
		CCLOG_INFO("Fxkk======>>> %d", 88);
		CCLOG_INFO("Fxkk======>>> %d", 99);
	}

}
