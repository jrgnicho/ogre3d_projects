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

const std::string CONFIGURATION_FILES_DIRECTORY="config/test_setup/";
const std::string RESOURCE_FILE_NAME="resources.cfg";
const std::string PLUGINS_FILE_NAME = "plugins.cfg";
const std::string OGRE_CONFIG_FILE_NAME = "ogre.cfg";
const std::string HELP_TEXT="test_state_manager program";

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

		StateManager *stateManager = StateManager::init(parameters);

		TestState *testState = (TestState*)TestState::getSingleton();

		TestDisplayImageState *testImgDisplayState = (TestDisplayImageState*)TestDisplayImageState::getSingleton();

		// setting next state as the image display
		testState->setChangeState(testImgDisplayState);
		testImgDisplayState->setChangeState(testState);

		// setting next state to push
		testState->setNextState(testImgDisplayState);

		stateManager->start(testState);

		StateManager::destroySingleton();
	}

	return 0;
}

