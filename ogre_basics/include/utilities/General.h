/*
 * General.h
 *
 *  Created on: Jun 12, 2014
 *      Author: ros developer 
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include <string.h>
#include <iostream>
#include <OGRE/OgreColourValue.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreMatrix4.h>

namespace application_parameters
{
	namespace defaults
	{
		const std::string PLUGINS_FILE = "plugins.cfg";
		const std::string CONFIGURATION_FILE = "ogre.cfg";
		const std::string LOG_FILE = "Ogre.log";
		const std::string RESOURCES_FILE = "resources_local.cfg";
		const std::string WINDOW_NAME = "Ogre Window";
		const std::string SCENE_MANAGER_NAME = "WorldScene";
		const Ogre::ColourValue SCENE_AMBIENT_LIGHT_COLOR = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
		const Ogre::Vector3 SCENE_AMBIENT_LIGHT_POSITION = Ogre::Vector3(400,400,0);
		const std::string SCENE_SKY_BOX_MATERIAL_NAME = "Examples/MorningSkyBox";
		const float SCENE_SKY_BOX_DISTANCE = 2000.0f;
		const std::string PARENT_NODE_NAME = "WorldNode";
		const Ogre::Matrix4 PARENT_NODE_TRANFORM = Ogre::Matrix4(
				1,0 ,0,0.0f,
				0,0 ,1,0.0f,
				0,-1,0,0.0f,
				0,0 ,0,1);

		const int WINDOW_WIDTH = 600;
		const int WINDOW_HEIGHT = 480;
		const int MIPMAPS = 5;
	}

	struct OgreParameters
	{
	public:
		OgreParameters()
		:PluginsFile(defaults::PLUGINS_FILE),
		 ConfigFile(defaults::CONFIGURATION_FILE),
		 LogFile(defaults::LOG_FILE),
		 ResourcesFile(defaults::RESOURCES_FILE),
		 ParentDirectory(""),
		 SceneManagerName(defaults::SCENE_MANAGER_NAME),
		 SceneAmbientLightColor(defaults::SCENE_AMBIENT_LIGHT_COLOR),
		 SceneSkyBoxMaterialName(defaults::SCENE_SKY_BOX_MATERIAL_NAME),
		 SceneSkyBoxDistance(defaults::SCENE_SKY_BOX_DISTANCE),
		 ParentNodeName(defaults::PARENT_NODE_NAME),
		 ParentNodeTransform(defaults::PARENT_NODE_TRANFORM),
		 WindowName(defaults::WINDOW_NAME),
		 WindowHeight(defaults::WINDOW_HEIGHT),
		 WindowWidth(defaults::WINDOW_WIDTH),
		 Mipmaps(defaults::MIPMAPS),
		 ConsoleLogging(false)
		{

		}

		std::string PluginsFile;
		std::string ConfigFile;
		std::string LogFile;
		std::string ResourcesFile;
		std::string ParentDirectory;
		std::string SceneManagerName;
		Ogre::ColourValue SceneAmbientLightColor;
		Ogre::Vector3 SceneAmbientLightPosition;
		std::string SceneSkyBoxMaterialName;
		float SceneSkyBoxDistance;
		std::string ParentNodeName;
		Ogre::Matrix4 ParentNodeTransform;
		std::string WindowName;
		int WindowHeight;
		int WindowWidth;
		int Mipmaps;
		bool ConsoleLogging;

	};

	namespace utils
	{

		bool parse_console_parameters(int argc,char** argv,std::string help_text,
				OgreParameters default_params,OgreParameters &params);

	}
}



#endif /* GENERAL_H_ */
