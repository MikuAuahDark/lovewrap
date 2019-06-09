/**
 * Copyright (c) 2040 Dark Energy Processor
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

// Lua
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// love
#include "common/version.h"
#include "modules/love/love.h"

// lovewrap
#include "lovewrap/LOVEWrap.h"
#include "lovewrap/Scene.h"

#ifdef LOVE_WINDOWS
// Prefer the higher performance GPU on Windows systems that use nvidia Optimus.
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
LOVE_EXPORT uint32_t NvOptimusEnablement = 1;

// Same with AMD GPUs.
// https://community.amd.com/thread/169965
LOVE_EXPORT uint32_t AmdPowerXpressRequestHighPerformance = 1;
#endif

extern lovewrap::Scene *gameInitialize();
extern void gameQuit();
extern int gameConfig(lua_State *L);

int runGame(int argc, char *argv[])
{
	// Open Lua state
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lovewrap::initialize(L);

	// Preload LOVE
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, &luaopen_love);
	lua_setfield(L, -2, "love");
	lua_pushcfunction(L, &gameConfig);
	lua_setfield(L, -2, "conf");
	lovewrap::Scene *gameScene = gameInitialize();
	lua_pushcfunction(L, lovewrap::initializeScene(gameScene));
	lua_setfield(L, -2, "main");
	lua_pop(L, 2);

	// Add command line arguments to global arg (like stand-alone Lua).
	{
		lua_newtable(L);
		if (argc > 0)
		{
			lua_pushstring(L, argv[0]);
			lua_rawseti(L, -2, -2);
		}
		
		lua_pushstring(L, "embedded boot.lua");
		lua_rawseti(L, -2, -1);

		for (int i = 1; i < argc; i++)
		{
			lua_pushstring(L, argv[i]);
			lua_rawseti(L, -2, i);
		}

		lua_setglobal(L, "arg");
	}

	// mark it as C++ so it uses different entry point
	lua_pushstring(L, "lovewrap");
	lua_setglobal(L, "LOVE_USED_AS_CPP");

	// require "love"
	lua_getglobal(L, "require");
	lua_pushstring(L, "love");
	lua_call(L, 1, 0);
	// Boot

	// get debug.traceback
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	
	// require "love.boot" (preloaded when love was required)
	lua_getglobal(L, "require");
	lua_pushstring(L, "love.boot");
	lua_call(L, 1, 1);

	// Call the returned boot function.
	int status = lua_pcall(L, 0, 1, lua_gettop(L) - 1);
	int retval = 0;

	// If status is nonzero, that means there's error.
	if (status)
	{
		retval = 1;
		fprintf(stderr, "Lua Error: %s", lua_tostring(L, -1));
	}
	else
		// If the boot function return number, then
		// it should be the status code
		if (lua_isnumber(L, -1))
			retval = (int)lua_tonumber(L, -1);
	
	gameQuit();
	lua_close(L);

	// Back control to main
	return retval;
}

static_assert((love::VERSION_MAJOR == 11 && love::VERSION_MINOR >= 3) || love::VERSION_MAJOR > 11, "At least LOVE 11.3 is required");
int main(int argc, char *argv[])
{
	try
	{
		return runGame(argc, argv);
	}
	catch (love::Exception &e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return 1;
	}

	return 1;
}
