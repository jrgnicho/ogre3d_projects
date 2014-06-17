/*
 * test_scene_loader.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: ros developer 
 */

#include <state_management/StateManager.h>
#include <state_types/TestSceneLoader.h>
#include <utilities/ShapeDrawer.h>

const std::string CONFIGURATION_FILES_DIRECTORY="config/level_builder/";
const std::string RESOURCE_FILE_NAME="test_scene_loader_resources.cfg";
const std::string PLUGINS_FILE_NAME = "plugins.cfg";
const std::string OGRE_CONFIG_FILE_NAME = "ogre.cfg";
const std::string HELP_TEXT="test_scene_loader program";

int main(int argc,char** argv)
{
	// ogre parameter setup
	application_parameters::OgreParameters parameters;
	parameters.ConfigFile = OGRE_CONFIG_FILE_NAME;
	parameters.ResourcesFile = RESOURCE_FILE_NAME;
	parameters.PluginsFile = PLUGINS_FILE_NAME;
	parameters.ParentDirectory = CONFIGURATION_FILES_DIRECTORY;

	if(application_parameters::utils::parse_console_parameters(argc,argv,HELP_TEXT,parameters,parameters))
	{
		// creating state manager
		StateManager *stateManager = StateManager::init(parameters);

		// creating test state scene
		TestSceneLoader* scene_loader_ptr = (TestSceneLoader*)TestSceneLoader::getSingleton();

		// start
		stateManager->start(scene_loader_ptr);

		// cleanup
		//stateManager->cleanup();
		stateManager->destroySingleton();
	}

	return 0;


}
