/*
 * test_level_builder.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: ros developer 
 */

#include <state_management/StateManager.h>
#include <state_types/TestLevelBuilder.h>
#include <utilities/ShapeDrawer.h>

int main(int argc,char** argv)
{
	// creating state manager
	StateManager *stateManager = StateManager::getSingleton();

	// creating test state scene
	TestLevelBuilder* level_builder_ptr = (TestLevelBuilder*)TestLevelBuilder::getSingleton();

	// start
	stateManager->start(level_builder_ptr);

	// cleanup
	//stateManager->cleanup();
	stateManager->destroySingleton();

	return 0;


}


