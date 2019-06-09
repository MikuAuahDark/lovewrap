LOVEWrap
========

Writing LOVE games using C++. This code is used in Alpha Stellar. This by mean need necessary modifications
in boot.lua. One LOVE fork that can be used is [livesim3-love](https://github.com/MikuAuahDark/livesim3-love).

Entry Point
-----------

There are 3 entry points that must be defined (not in any particular namespace):

* `int gameConfig(lua_State *L)` is the equivalent of `love.conf`. At the moment, Lua string must be passed. Example:

```cpp
int gameConfig(lua_State *L)
{
	luaL_dostring(L, R"(
	function love.conf(t)
		t.identity = "astellar"
		t.appendidentity = true
		t.version = "11.3"
		t.console = false
		t.accelerometerjoystick = true
		t.externalstorage = true
		t.gammacorrect = false
 
		t.audio.mixwithsystem = true
 
		t.window.title = "Alpha Stellar"
		t.window.icon = nil
		t.window.width = 800
		t.window.height = 600
		t.window.borderless = false
		t.window.resizable = false
		t.window.minwidth = 320
		t.window.minheight = 240
		t.window.fullscreen = false
		t.window.fullscreentype = "desktop"
		t.window.vsync = 0
		t.window.msaa = 0
		t.window.depth = nil
		t.window.stencil = nil
		t.window.display = 1
		t.window.highdpi = true
		t.window.x = nil
		t.window.y = nil
 
		t.modules.audio = true
		t.modules.event = true
		t.modules.font = true
		t.modules.graphics = true
		t.modules.image = true
		t.modules.joystick = true
		t.modules.keyboard = true
		t.modules.math = true
		t.modules.mouse = true
		t.modules.physics = false
		t.modules.sound = true
		t.modules.system = true
		t.modules.thread = false -- using std::thread
		t.modules.timer = true
		t.modules.touch = true
		t.modules.video = true
		t.modules.window = true
	end
	)");
	lua_pushboolean(L, 1);
	return 1;
}
```

* `lovewrap::Scene *gameInitialize()` means game initialization. You have to return Scene object. Example:

```cpp
static astellar::AstellarScene *gameScene = nullptr;

lovewrap::Scene *gameInitialize()
{
	return gameScene = new astellar::AstellarScene();
}
```

* `void gameQuit()` is called when your game exit. Free any memory used here. Example:

```cpp
void gameQuit()
{
	delete gameScene;
	gameScene = nullptr;
}
```

Scene Object
------------

The Scene object is somewhat like gamestate bject but only one can be returned on `gameInitialize` and cannot be changed. It contains most things needed
by Alpha Stellar, so more things will be added as time goes on.

Main.cpp
--------

`Main.cpp` contains `main` entry point. Note that it doesn't support iOS (must not exit).
