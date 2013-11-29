/*
 * MaterialCreator.h
 *
 *  Created on: Nov 22, 2013
 *      Author: ros developer 
 */

#ifndef MATERIALCREATOR_H_
#define MATERIALCREATOR_H_

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreMaterialManager.h"

class MaterialCreator {

public:

	enum Colors
	{
		BLUE = 0,
		YELLOW = 1,
		RED = 2,
		GREEN = 3,
		GRAY = 4,
		BLACK = 5,
		WIREFRAME_WHITE = 7,
		WIREFRAME_GRAY = 8,
		WIREFRAME_TRANSPARENT = 9,
		TRANSPARENT_RED = 10,
		TRANSPARENT_BLUE = 11

	};

	typedef std::map<int,std::string> MaterialMap;
	typedef boost::shared_ptr<MaterialCreator> Ptr;

public:
	virtual ~MaterialCreator();

	static MaterialCreator::Ptr get_singleton();
	Ogre::MaterialPtr get_material(Colors c);

	Ogre::MaterialPtr create_texture_material(std::string image_name,float utile = 1.0f,float vtile = 1.0f,float alpha = 1.0f);

	Ogre::MaterialPtr build_solid_material(std::string name,Ogre::ColourValue &diffuse,
			Ogre::ColourValue &ambient, Ogre::ColourValue &specular);
	Ogre::MaterialPtr build_transparent_material(std::string name,Ogre::ColourValue &diffuse,
			Ogre::ColourValue &ambient);

	Ogre::MaterialPtr build_wireframe_material(std::string name,Ogre::ColourValue &diffuse,
			Ogre::ColourValue &ambient, Ogre::ColourValue &specular,
			Ogre::ColourValue edge_color = Ogre::ColourValue(0,0,0,1));

public:

	static const std::string RESOURCE_GROUP;
	static const std::string RESOURCE_DIRECTORY;

protected:

	MaterialMap material_map_;
	static MaterialCreator::Ptr single_instance_;

protected:

	MaterialCreator();

	void init();
	void close();


};

#endif /* MATERIALCREATOR_H_ */
