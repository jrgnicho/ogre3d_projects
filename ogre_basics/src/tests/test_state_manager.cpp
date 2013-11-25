/*
 * test_class.cpp
 *
 *  Created on: Aug 13, 2012
 *      Author: coky
 */
#include <state_management/StateManager.h>
#include <state_types/TestState.h>
#include <state_types/TestDisplayImageState.h>
#include <OGRE/Ogre.h>
int main(int argc,char** argv)
{
	std::cout<<"Working directory: "<<argv[0]<<"\n";
	StateManager *stateManager = StateManager::getSingleton();
	//stateManager->setup();// change access to setup and cleanup functions

	TestState *testState = (TestState*)TestState::getSingleton();
	//testState->setup();

	TestDisplayImageState *testImgDisplayState = (TestDisplayImageState*)TestDisplayImageState::getSingleton();
	//testImgDisplayState->setup();

	// setting next state as the image display
	testState->setChangeState(testImgDisplayState);
	testImgDisplayState->setChangeState(testState);

	// setting next state to push
	testState->setNextState(testImgDisplayState);

	stateManager->start(testState);

	//stateManager->cleanup();
	StateManager::destroySingleton();

	return 0;
}

