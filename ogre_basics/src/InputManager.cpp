/*
 * InputManager.cpp
 *
 *  Created on: Aug 14, 2012
 *      Author: coky
 */

#include <input_handling/InputManager.h>

InputManager::InputManager(std::string windowHandle):
_windowHandle(windowHandle),
_InputManager(0),
_Keyboard(0),
_Mouse(0)
{
	// TODO Auto-generated constructor stub
}

InputManager::~InputManager() {
	// TODO Auto-generated destructor stub
}

void InputManager::setup()
{
	OIS::ParamList paramList;
	paramList.insert(std::make_pair("WINDOW",_windowHandle));

#ifdef OIS_WIN32_PLATFORM
	paramList.insert(std::make_pair(std::string("w32_mouse"),std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_mouse"),std::string("DISCL_NONEXCLUSIVE")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"),std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"),std::string("DISCL_NONEXCLUSIVE")));
#endif

#ifdef OIS_LINUX_PLATFORM
	paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	// creating input system instance
	_InputManager = OIS::InputManager::createInputSystem(paramList);

#ifdef USING_UNBUFFERRED
	_UseBufferedInput = false;
#endif

#ifdef USING_BUFFERRED
	_UseBufferedInput = true;
#endif

	_Keyboard = static_cast<OIS::Keyboard *>(_InputManager->createInputObject(OIS::OISKeyboard,_UseBufferedInput));
	_Mouse = static_cast<OIS::Mouse *>(_InputManager->createInputObject(OIS::OISMouse,_UseBufferedInput));
//	_Keyboard->setEventCallback(this);
//	_Mouse->setEventCallback(this);
}

void InputManager::setEventHandlers(InputEventHandler *handler)
{
	_Keyboard->setEventCallback(handler);
	_Mouse->setEventCallback(handler);
}

void InputManager::cleanup()
{
	if(_InputManager)
	{
		if(_Keyboard)_InputManager->destroyInputObject(_Keyboard);
		if(_Mouse)_InputManager->destroyInputObject(_Mouse);
		OIS::InputManager::destroyInputSystem(_InputManager);
		_InputManager = NULL;
		_Keyboard = NULL;
		_Mouse = NULL;
	}
}
