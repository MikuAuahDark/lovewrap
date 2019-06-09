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

// LOVEWrap.cpp contains some functions that can't simply by binded 1:1 into
// its Lua counterpart, like love.graphics.newShader

// STL
#include <mutex>
#include <thread>

// love
#include "modules/graphics/wrap_Shader.h"

// lovewrap
#include "LOVEWrap.h"

static lua_State *globalL = nullptr;
static std::recursive_mutex luaLock;

// Assume error message is at -1
inline std::string getLuaError(lua_State *L)
{
	size_t strSize;
	const char *str = lua_tolstring(globalL, -1, &strSize);
	return std::string(str, strSize);
}

// Class used to reset Lua stack
class lua_stack_balance
{
public:
	lua_stack_balance() = delete;
	lua_stack_balance(const lua_stack_balance&) = delete;
	lua_stack_balance& operator=(const lua_stack_balance&) = delete;

	lua_stack_balance(lua_State *L): L(L) {
		capturedTop = lua_gettop(L);
	}

	~lua_stack_balance()
	{
		lua_settop(L, capturedTop);
	}

private:
	int capturedTop;
	lua_State *L;
};

namespace lovewrap
{

void initialize(lua_State *L)
{
	std::lock_guard<std::recursive_mutex> dummyLock(luaLock);
	globalL = L;
}

namespace graphics
{

Shader *newShader(const std::string &data)
{
	std::lock_guard<std::recursive_mutex> dummyLock(luaLock);
	lua_stack_balance dummyLS(globalL);

	lua_getglobal(globalL, "love");
	lua_getfield(globalL, -1, "graphics");
	lua_getfield(globalL, -1, "newShader");
	lua_pushlstring(globalL, data.c_str(), data.length());

	// Call love.graphics.newShader(data)
	if (lua_pcall(globalL, 1, 1, 0))
		throw love::Exception("%s", getLuaError(globalL).c_str());

	// Have shader in -1
	Shader *shader = luax_checkshader(globalL, -1);
	// Retain so when Lua GC it we still have the object
	shader->retain();
	return shader;
}

Shader *newShader(const std::string &vertex, const std::string &pixel)
{
	std::lock_guard<std::recursive_mutex> dummyLock(luaLock);
	lua_stack_balance dummyLS(globalL);

	lua_getglobal(globalL, "love");
	lua_getfield(globalL, -1, "graphics");
	lua_getfield(globalL, -1, "newShader");
	lua_pushlstring(globalL, vertex.c_str(), vertex.length());
	lua_pushlstring(globalL, pixel.c_str(), pixel.length());

	// Call love.graphics.newShader(vertex, pixel)
	if (lua_pcall(globalL, 2, 1, 0))
		throw love::Exception("%s", getLuaError(globalL).c_str());

	// Have shader in -1
	Shader *shader = luax_checkshader(globalL, -1);
	// Retain so when Lua GC it we still have the object
	shader->retain();
	return shader;
}

} // graphics
} // lovewrap
