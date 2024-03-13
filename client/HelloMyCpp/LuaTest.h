#ifndef __LUA_TEST_H__
#define __LUA_TEST_H__

namespace Fancy
{
	// 1、简单运行Lua代码
	extern void lua_test0(); //测试callString

	// 2、与Lua交换数据
	extern void lua_test1(); //测试lua_pushstring

	extern void lua_test2(); //测试lua_settable

	extern void lua_test3(); //测试lua_setfield

	// 3、在C++中调用Lua子函数
	extern void lua_test4(); //测试lua_setfield

	// 4、在Lua中调用C++函数
	extern void lua_test5(); //测试lua_pushcfunction

	extern void lua_test6();
}

#endif //__LUA_TEST_H__