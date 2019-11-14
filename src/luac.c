#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "luac.h"

// ------------------ FORWARD DECLARATION ------------------
static int CheckLua(lua_State *L, int r);
static int lua_HostFunction(lua_State* L);

void luaScriptTest();




// Little error checking utility function
static int CheckLua(lua_State *L, int r) {
	if (r != LUA_OK) {
		printf("ERROR: %s\n", lua_tostring(L, -1));
		return 0;
	}
	return 1;
}


static int lua_HostFunction(lua_State* L) {
	float a = (float) lua_tonumber(L, 1);
	float b = (float) lua_tonumber(L, 2);

	printf("[C] HostFunction(%f, %f) \n", a, b);
	
	float c = a * b;
	lua_pushnumber(L, c);
	return 1;
}


void luaScriptTest() {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "HostFunction", lua_HostFunction);
	
	if (CheckLua(L, luaL_dofile(L, "res/script.lua"))) {
		
		/**
		 * CALLING LUA FUNCTION FROM C
		 * */
		lua_getglobal(L, "AddStuff");
		
		if (lua_isfunction(L, -1)) {
			lua_pushnumber(L, 3.5f);
			lua_pushnumber(L, 7.1f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0))) {
				printf("[C] Called in Lua 'AddStuff(3.5f, 7.1f)', got: %f\n", (float) lua_tonumber(L, -1));
			}
		}
		
		/**
		 * LUA call specific players from eg database
		 * */
		lua_getglobal(L, "GetPlayer");
		
		if (lua_isfunction(L, -1)) {
			
			lua_pushnumber(L, 1); // 0 - Todd, 1 - Pete

			if (CheckLua(L, lua_pcall(L, 1, 1, 0))) {
				
				if (lua_istable(L, -1)) {
					lua_pushstring(L, "Name");
					lua_gettable(L, -2);
					printf("Player name is %s\n", lua_tostring(L, -1));
					lua_pop(L, 1);

					lua_pushstring(L, "Level");
					lua_gettable(L, -2);
					printf("Player level: %i\n", (int) lua_tonumber(L, -1));
					lua_pop(L, 1);
				}
			}
		}


		lua_getglobal(L, "DoAThing");
		
		if (lua_isfunction(L, -1)) {
			
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0))) {
				printf("[C] Called in Lua 'DoAThing(5.0f, 6.0f)', got: %f\n", (float) lua_tonumber(L, -1));
			}
		}


		/**
		 * 
		 * CALLING LUA FUNCTION
		 * */
		lua_getglobal(L, "compareInts");
		
		if (lua_isfunction(L, -1)) {
			lua_pushnumber(L, 2.0f);
			lua_pushnumber(L, 2.0f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0))) {
				printf("[C] Called in Lua compareInts, got: %f\n", (float) lua_tonumber(L, -1));
			}
		}

		lua_getglobal(L, "getPlayerName");
		
		if (lua_isfunction(L, -1)) {
			
			lua_pushnumber(L, 0); // 0 - Todd, 1 - Pete

			if (CheckLua(L, lua_pcall(L, 1, 1, 0))) {
				printf("Player name is %s\n", lua_tostring(L, -1));
			}
			
		}
	}

	lua_close(L);
}
