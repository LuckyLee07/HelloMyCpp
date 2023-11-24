#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <complex>

extern const double PI = 3.14159;

extern int calcComplex(lua_State *L)
{
	double r = luaL_checknumber(L, 1);
	double i = luaL_checknumber(L, 2);

	std::complex<double> c(r, i);
	
	//存入绝对值
	lua_pushnumber(L, std::abs(c));
	//存入角度
	lua_pushnumber(L, std::arg(c)*180.0 / PI);

	return 2;
}
