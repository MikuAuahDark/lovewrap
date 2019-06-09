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

// STL
#include <map>
#include <string>
#include <vector>

// Lua
extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

// lovewrap
#include "LOVEWrap.h"
#include "Scene.h"

// Current scene
lovewrap::Scene *currentScene = nullptr;

static int loveLoad(lua_State *L)
{
	// args
	std::vector<std::string> args;
	int argLen = lua_objlen(L, 1);

	for (int i = 1; i <= argLen; i++)
	{
		lua_pushinteger(L, (lua_Integer) i);
		lua_rawget(L, 1);

		size_t strSize;
		const char *str = lua_tolstring(L, -1, &strSize);
		args.push_back(std::string(str, strSize));
		lua_pop(L, 1);
	}

	currentScene->load(args);
	return 0;
}

static int loveUpdate(lua_State *L)
{
	currentScene->update(luaL_checknumber(L, 1));
	return 0;
}

static int loveDraw(lua_State *L)
{
	currentScene->draw();
	return 0;
}

static int loveQuit(lua_State *L)
{
	lua_pushboolean(L, (int) currentScene->quit());
	return 1;
}

typedef void(*EventHandlerFunc)(const std::vector<love::Variant> &);

inline ptrdiff_t getIntegerFromVariant(const std::vector<love::Variant> &arg, size_t index)
{
	if (index > arg.size())
		throw love::Exception("index %u is out of range", (uint32_t) index);

	const love::Variant &var = arg[index - 1];
	if (var.getType() != love::Variant::NUMBER)
		throw love::Exception("index %u is not a number", (uint32_t) index);

	return var.getData().number;
}

inline bool getBooleanFromVariant(const std::vector<love::Variant> &arg, size_t index, bool implicitConversion = false)
{
	if (index > arg.size())
		throw love::Exception("index %u is out of range", (uint32_t) index);

	const love::Variant &var = arg[index - 1];
	const love::Variant::Data &data = var.getData();
	love::Variant::Type varType = var.getType();

	if (implicitConversion)
	{
		if (varType == love::Variant::BOOLEAN)
			return data.boolean;
		else if (varType == love::Variant::NUMBER)
			return abs(data.number) <= 0.000001;
		else if (varType == love::Variant::NIL)
			return false;
		else
			throw love::Exception("index %u is not a boolean", (uint32_t) index);
	}
	else if (varType != var.BOOLEAN)
		throw love::Exception("index %u is not a boolean", (uint32_t) index);

	return var.getData().boolean;
}

inline std::string getStringFromVariant(const std::vector<love::Variant> &arg, size_t index)
{
	if (index > arg.size())
		throw love::Exception("index %u is out of range", (uint32_t) index);

	const love::Variant &var = arg[index - 1];
	const love::Variant::Data &data = var.getData();
	love::Variant::Type varType = var.getType();

	switch(varType)
	{
		case love::Variant::SMALLSTRING:
		{
			return std::string(data.smallstring.str, data.smallstring.len);
		}
		case love::Variant::STRING:
		{
			return std::string(data.string->str, data.string->len);
		}
		default:
			throw love::Exception("index %u is not a string", (uint32_t) index);
	}
}

template<typename T> inline T getConstantFromVariant(
	const std::vector<love::Variant> &arg,
	size_t index,
	bool (*func)(const char*, T&)
)
{
	std::string str = getStringFromVariant(arg, index);
	T outVal;
	if (func(str.c_str(), outVal))
		return outVal;
	else
		throw love::Exception("index %u invalid constant value '%s'", (uint32_t) index, str.c_str());
}

static void loveEventKeyPressed(const std::vector<love::Variant> &arg)
{
	using namespace love::keyboard;

	currentScene->keyPressed(
		getConstantFromVariant<Keyboard::Key>(arg, 1, &Keyboard::getConstant),
		getConstantFromVariant<Keyboard::Scancode>(arg, 2, &Keyboard::getConstant),
		getBooleanFromVariant(arg, 3)
	);
}

static void loveEventKeyReleased(const std::vector<love::Variant> &arg)
{
	using namespace love::keyboard;

	currentScene->keyReleased(
		getConstantFromVariant<Keyboard::Key>(arg, 1, &Keyboard::getConstant),
		getConstantFromVariant<Keyboard::Scancode>(arg, 2, &Keyboard::getConstant)
	);
}

static int loveGameLoop(lua_State *L)
{
	static bool eventHandlerInitialized = false;
	static std::map<std::string, EventHandlerFunc> eventHandler;

	if (!eventHandlerInitialized)
	{
		eventHandler["keypressed"] = &loveEventKeyPressed;
		eventHandler["keyreleased"] = &loveEventKeyReleased;
		eventHandlerInitialized = true;
	}

	double dt = 0;

	if (lovewrap::event::isLoaded())
	{
		auto inst = lovewrap::event::getInstance();

		love::event::Message *msg = nullptr;
		inst->pump();

		while (inst->poll(msg))
		{
			if (msg->name.compare("quit") == 0 && currentScene->quit() == false)
			{
				if (msg->args.size() > 0)
					msg->args[0].toLua(L);
				else
					lua_pushinteger(L, 0);

				return 1;
			}
			else
			{
				auto iter = eventHandler.find(msg->name);
				if (iter != eventHandler.end())
				{
					try
					{
						iter->second(msg->args);
					}
					catch (love::Exception &e)
					{
						fprintf(stderr, "Exception '%s': %s\n", msg->name.c_str(), e.what());
						return luaL_error(L, "%s", e.what());
					}
				}
				else
					fprintf(stderr, "Missing event handler: %s\n", msg->name.c_str());
			}
		}
	}

	if (lovewrap::timer::isLoaded())
		dt = lovewrap::timer::step();

	try
	{
		currentScene->update(dt);
	}
	catch (love::Exception &e)
	{
		lua_pushstring(L, e.what());
		lua_error(L);
	}

	if (lovewrap::graphics::isLoaded() && lovewrap::graphics::isActive())
	{
		lovewrap::graphics::origin();
		lovewrap::graphics::clear(lovewrap::graphics::getBackgroundColor());
		try
		{
			currentScene->draw();
		}
		catch (love::Exception &e)
		{
			lua_pushstring(L, e.what());
			lua_error(L);
		}
		lua_gc(L, LUA_GCCOLLECT, 0);
		lovewrap::graphics::present();
	}

	return 0;
}

static int loveRun(lua_State *L)
{
	// args
	std::vector<std::string> args;
	lua_getglobal(L, "arg");
	int argLen = lua_objlen(L, 1);
	size_t strSize;
	const char *str = nullptr;

	// index -2 = argv[0]
	lua_pushinteger(L, -2);
	lua_rawget(L, 1);
	str = lua_tolstring(L, -1, &strSize);
	args.push_back(std::string(str, strSize));

	for (int i = 1; i <= argLen; i++)
	{
		lua_pushinteger(L, (lua_Integer) i);
		lua_rawget(L, 1);

		str = lua_tolstring(L, -1, &strSize);
		if (str != nullptr)
			args.push_back(std::string(str, strSize));

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	try
	{
		currentScene->load(args);
	}
	catch (love::Exception &e)
	{
		lua_pushstring(L, e.what());
		lua_error(L);
	}

	lua_pushcfunction(L, &loveGameLoop);
	return 1;
}

int baseMain(lua_State *L)
{
	lua_getglobal(L, "love");

	// love.load
	lua_pushcfunction(L, &loveLoad);
	lua_setfield(L, -2, "load");
	// love.update
	lua_pushcfunction(L, &loveUpdate);
	lua_setfield(L, -2, "update");
	// love.draw
	lua_pushcfunction(L, &loveDraw);
	lua_setfield(L, -2, "draw");
	// love.quit
	lua_pushcfunction(L, &loveQuit);
	lua_setfield(L, -2, "quit");
	// love.run
	lua_pushcfunction(L, &loveRun);
	lua_setfield(L, -2, "run");

	lua_pop(L, 1);
	lua_pushboolean(L, 1);
	return 1;
}

namespace lovewrap
{

lua_CFunction initializeScene(Scene *scene)
{
	currentScene = scene;
	return &baseMain;
}

Scene::Scene() {}
Scene::~Scene() {}
void Scene::load(std::vector<std::string>) {}
void Scene::update(double) {}
void Scene::draw() {}
bool Scene::quit() {return false;}
void Scene::visible(bool) {}
void Scene::focus(bool) {}
void Scene::resize(int, int) {}
void Scene::keyPressed(love::keyboard::Keyboard::Key, love::keyboard::Keyboard::Scancode, bool) {}
void Scene::keyReleased(love::keyboard::Keyboard::Key, love::keyboard::Keyboard::Scancode) {}
void Scene::textInput(std::string) {}
void Scene::mousePressed(int, int, int, bool) {}
void Scene::mouseReleased(int, int, int, bool) {}
void Scene::mouseMoved(int, int, int, int, bool) {}
void Scene::mouseFocus(bool) {}

}
