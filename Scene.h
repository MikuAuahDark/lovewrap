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

#ifndef LOVEWRAP_SCENE_H
#define LOVEWRAP_SCENE_H

// STL
#include <string>
#include <vector>

// Lua
extern "C" {
#include "lua.h"
}

// love.keyboard
#include "modules/keyboard/Keyboard.h"

namespace lovewrap
{

class Scene
{
public:
	virtual ~Scene();
	virtual void load(std::vector<std::string> args);
	virtual void update(double deltaT);
	virtual void draw();
	virtual bool quit();
	virtual void focus(bool f);
	virtual void visible(bool v);
	virtual void resize(int w, int h);
	virtual void keyPressed(love::keyboard::Keyboard::Key key, love::keyboard::Keyboard::Scancode scancode, bool repeat);
	virtual void keyReleased(love::keyboard::Keyboard::Key key, love::keyboard::Keyboard::Scancode scancode);
	virtual void textInput(std::string text);
	virtual void mousePressed(int x, int y, int button, bool istouch);
	virtual void mouseReleased(int x, int y, int button, bool istouch);
	virtual void mouseMoved(int x, int y, int dx, int dy, bool istouch);
	virtual void mouseFocus(bool f);
protected:
	Scene();
};

lua_CFunction initializeScene(Scene *scene);

}

#endif
