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
		WIREFRAME_GRAY = 8
	};

	typedef std::map<int,Ogre::MaterialPtr> MaterialMap;
	typedef boost::shared_ptr<MaterialCreator> Ptr;

public:
	virtual ~MaterialCreator();

	static MaterialCreator::Ptr get_singleton();
	Ogre::MaterialPtr get_material(Colors c);

public:

	static const std::string RESOURCE_GROUP;
	static const std::string RESOURCE_DIRECTORY;

protected:

	std::map<int,Ogre::MaterialPtr> material_map_;
	static MaterialCreator::Ptr single_instance_;

protected:

	MaterialCreator();

	void init();
	void close();

	Ogre::MaterialPtr build_solid_material(std::string name,Ogre::ColourValue &diffuse,
			Ogre::ColourValue &ambient, Ogre::ColourValue &specular);

	Ogre::MaterialPtr build_wireframe_material(std::string name,Ogre::ColourValue &diffuse,
			Ogre::ColourValue &ambient, Ogre::ColourValue &specular);


};

#endif /* MATERIALCREATOR_H_ */
