/*
 * test_class.cpp
 *
 *  Created on: Aug 13, 2012
 *      Author: coky
 */
#include <state_management/StateManager.h>
#include <state_types/TestState.h>
#include <OGRE/Ogre.h>
int main(int argc,char** argv)
{
	std::cout<<"Working directory: "<<argv[0]<<"\n";
	StateManager *stateManager = StateManager::getSingleton();
	stateManager->setup();// change access to setup and cleanup functions
	TestState *testState = (TestState*)TestState::getSingleton();
	//testState->setup();

	stateManager->start(testState);

	stateManager->cleanup();

	return 0;
}

