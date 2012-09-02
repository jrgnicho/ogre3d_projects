/*
 * InputManager.h
 *
 *  Created on: Aug 14, 2012
 *      Author: coky
 */

#ifndef IOMANAGER_H_
#define IOMANAGER_H_

//#define USING_UNBUFFERRED
#define USING_BUFFERRED

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <input_handling/InputEventHandler.h>

class InputManager
{
public:
	InputManager(std::string windowHandle = std::string());
	virtual ~InputManager();

	// initialization
	void setup();
	void cleanup();

	// device access methods
	OIS::Keyboard* getKeyboard();
	OIS::Mouse* getMouse();

	void setEventHandler(InputEventHandler *handler);

//	// keyboard methods
//	virtual bool keyPressed(const OIS::KeyEvent &evnt){return true;}
//	virtual bool keyReleased(const OIS::KeyEvent &evnt){return true;}
//	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent &evnt) = 0;
//
//	// mouse methods
//	virtual bool mousePressed(const OIS::MouseEvent &evnt,OIS::MouseButtonID id){return true;}
//	virtual bool mouseReleased(const OIS::MouseEvent &evnt,OIS::MouseButtonID id){return true;}
//	virtual bool mouseMoved(const OIS::MouseEvent &evnt){return true;}
//	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent &evnt) = 0;

protected:

	// members
	std::string _windowHandle;
	OIS::InputManager *_InputManager;
	OIS::Keyboard *_Keyboard;
	OIS::Mouse *_Mouse;
	bool _UseBufferedInput;

};

#endif /* IOMANAGER_H_ */
