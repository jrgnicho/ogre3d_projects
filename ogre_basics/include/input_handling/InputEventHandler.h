/*
 * InputEventHandler.h
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#ifndef INPUTEVENTHANDLER_H_
#define INPUTEVENTHANDLER_H_
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
class InputEventHandler: public OIS::KeyListener, public OIS::MouseListener
{
public:

	virtual bool keyPressed(const OIS::KeyEvent &evnt){return true;}
	virtual bool keyReleased(const OIS::KeyEvent &evnt){return true;}
	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent &evnt) = 0;

	// mouse methods
	virtual bool mousePressed(const OIS::MouseEvent &evnt,OIS::MouseButtonID id){return true;}
	virtual bool mouseReleased(const OIS::MouseEvent &evnt,OIS::MouseButtonID id){return true;}
	virtual bool mouseMoved(const OIS::MouseEvent &evnt){return true;}
	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent &evnt) = 0;
};

#endif /* INPUTEVENTHANDLER_H_ */
