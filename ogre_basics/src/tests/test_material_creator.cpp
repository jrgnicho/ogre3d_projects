/*
 * test_material_creator.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: ros developer 
 */
#include <state_management/StateManager.h>
#include <utilities/MaterialCreator.h>
#include <OGRE/Ogre.h>


const std::string CONFIGURATION_FILES_DIRECTORY="config/test_setup/";
const std::string RESOURCE_FILE_NAME="resources.cfg";
const std::string PLUGINS_FILE_NAME = "plugins.cfg";
const std::string OGRE_CONFIG_FILE_NAME = "ogre.cfg";
const std::string HELP_TEXT="test_material_creator program";


int main(int argc,char** argv)
{
	// ogre parameter setup
	application_parameters::OgreParameters parameters;
	parameters.ConfigFile = OGRE_CONFIG_FILE_NAME;
	parameters.ResourcesFile = RESOURCE_FILE_NAME;
	parameters.PluginsFile = PLUGINS_FILE_NAME;
	parameters.ParentDirectory = CONFIGURATION_FILES_DIRECTORY;
	parameters.ConsoleLogging = true;

	if(application_parameters::utils::parse_console_parameters(argc,argv,HELP_TEXT,parameters,parameters))
	{
		StateManager* sm_ptr = StateManager::init(parameters);
		MaterialCreator::Ptr mc_ptr = MaterialCreator::get_singleton();

		sm_ptr->destroySingleton();
	}

	return 0;
}



