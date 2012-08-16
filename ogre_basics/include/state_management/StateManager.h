/*
 * StateManager.h
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_

#include <input_handling/InputManager.h>
#include <state_management/StateElement.h>
class StateManager {
public:

	virtual ~StateManager();

	// state management methods
	void start(StateElement *state);
	void changeState(StateElement *state);
	void pushState(StateElement *state);
	void popState(StateElement *state);

	//
	void setup();
	void cleanup();

	static StateManager* getSingleton();

protected:
	StateManager();

	InputManager _InputManager;
};

#endif /* STATEMANAGER_H_ */
