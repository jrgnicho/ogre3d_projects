/*
 * StateInterface.h
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#ifndef STATEINTERFACE_H_
#define STATEINTERFACE_H_

#include <input_handling/InputEventHandler.h>
#include <state_management/StateManager.h>
#include <OGRE/OgreFrameListener.h>

class StateInterface: public InputEventHandler, public Ogre::FrameListener
{
public:

	virtual ~StateInterface(){}

	// state control methods
	virtual std::string getStateName(){return _StateName;}
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){return false;}
	virtual void resume(){}


	virtual void setup() = 0;
	virtual void cleanup()
	{
		delete this; // this might produced a segmentation fault
	}

	// frame callbacks
	virtual bool frameStarted(const Ogre::FrameEvent &evnt) = 0;
	virtual bool frameEnded(const Ogre::FrameEvent &evnt) = 0;

	static StateInterface* getSingleton()
	{
		/*	the overloaded method of each subclass should do the following,
			however it should use its own constructor
			instead
		*/
//		if(_Instance == NULL)
//		{
//			_Instance = new StateInterface();
//			_Instance->setup();
//		}
//		return _Instance;
		return NULL;
	}


protected:
	StateInterface(){}

	// removes current state and places it with the requested one instead
	void changeState(StateInterface *state)
	{
		StateManager::getSingleton()->changeState(state);
	}

	// pauses the current state and pushes the requested one on top of the stack
	bool pushState(StateInterface *state)
	{
		return StateManager::getSingleton()->pushState(state);
	}

	// pops the current state from the stack, yielding control to the next state
	void popState()
	{
		StateManager::getSingleton()->popState();
	}

	void shutdown()
	{
		StateManager::getSingleton()->shutdown();
	}

	static StateInterface *_Instance;
	std::string _StateName;

};

#endif /* STATEINTERFACE_H_ */
