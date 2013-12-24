/*
 * test_scene_loader.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: ros developer 
 */

#include <state_management/StateManager.h>
#include <state_types/TestSceneLoader.h>
#include <utilities/ShapeDrawer.h>

int main(int argc,char** argv)
{
	// creating parameters
	OgreParameters parameters;
	parameters.ResourcesFile = "scene_loader_resources.cfg";

	// creating state manager
	StateManager *stateManager = StateManager::init(parameters);

	// creating test state scene
	TestSceneLoader* scene_loader_ptr = (TestSceneLoader*)TestSceneLoader::getSingleton();

	// start
	stateManager->start(scene_loader_ptr);

	// cleanup
	//stateManager->cleanup();
	stateManager->destroySingleton();

	return 0;


}
