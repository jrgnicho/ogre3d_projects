/*
 * StateElement.h
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#ifndef STATEELEMENT_H_
#define STATEELEMENT_H_

#include <input_handling/InputEventHandler.h>
#include <state_management/StateManager.h>

class StateElement: public InputEventHandler
{
public:

	virtual ~StateElement();

	virtual std::string getStateName(){return _StateName};
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual void pause{return false;}
	virtual void resume(){}

	virtual void setup() = 0;
	virtual void cleanup()=0;

	virtual static StateElement* getSingleton() = 0;

protected:
	StateElement();

	StateElement *_Instance;
	std::string _StateName;

};

#endif /* STATEELEMENT_H_ */
