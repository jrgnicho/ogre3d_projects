/*
 * test_shape_drawer.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: ros developer 
 */

#include <state_management/StateManager.h>
#include <state_types/TestSceneState.h>
#include <utilities/ShapeDrawer.h>

const std::string CONFIGURATION_FILES_DIRECTORY="config/test_setup/";
const std::string RESOURCE_FILE_NAME="resources.cfg";
const std::string PLUGINS_FILE_NAME = "plugins.cfg";
const std::string OGRE_CONFIG_FILE_NAME = "ogre.cfg";
const std::string HELP_TEXT="test_shape_drawer program";


int main(int argc,char** argv)
{

	// ogre parameter setup
	application_parameters::OgreParameters parameters;
	parameters.ConfigFile = OGRE_CONFIG_FILE_NAME;
	parameters.ResourcesFile = RESOURCE_FILE_NAME;
	parameters.PluginsFile = PLUGINS_FILE_NAME;
	parameters.ParentDirectory = CONFIGURATION_FILES_DIRECTORY;
	parameters.ConsoleLogging = false;

	if(application_parameters::utils::parse_console_parameters(argc,argv,HELP_TEXT,parameters,parameters))
	{

		// creating state manager
		StateManager *stateManager = StateManager::init(parameters);

		// creating test state scene
		TestSceneState* scene_state_ptr = (TestSceneState*)TestSceneState::getSingleton();

		// start
		stateManager->start(scene_state_ptr);

		// cleanup
		//stateManager->cleanup();
		stateManager->destroySingleton();

	}

	return 0;


}




