#pragma once

namespace Fancy
{	
	// 1��������Lua����
	extern void lua_test0(); //����callString

	// 2����Lua��������
	extern void lua_test1(); //����lua_pushstring

	extern void lua_test2(); //����lua_settable

	extern void lua_test3(); //����lua_setfield

	// 3����C++�е���Lua�Ӻ���
	extern void lua_test4(); //����lua_setfield

	// 4����Lua�е���C++����
	extern void lua_test5(); //����lua_pushcfunction

	extern void lua_test6();
}
