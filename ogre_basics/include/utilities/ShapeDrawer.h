/*
 * ShapeDrawer.h
 *
 *  Created on: Oct 21, 2012
 *      Author: coky
 */

#ifndef SHAPEDRAWER_H_
#define SHAPEDRAWER_H_

#include <OgreEntity.h>
#include <OgreMesh.h>

class ShapeDrawer
{
	enum ShapeType
	{
		SPHERE = 1,
		BOX = 2,
		PIRAMID = 3,
		CAPSULE = 4,
		CYLINDER = 5
	};

public:
	virtual ~ShapeDrawer();

	static ShapeDrawer* getSingleton();

	Ogre::Entity* createEntity(ShapeType shape);

	void exportMeshes(std::string path);

	static const std::string RESOURCE_GROUP_NAME;
	static const std::string DEFAULT_SAVE_PATH;

protected:
	ShapeDrawer();

	Ogre::MeshPtr buildBox();
	Ogre::MeshPtr buildSphere();
	Ogre::MeshPtr buildCapsule();
	Ogre::MeshPtr buildCylinder();
	Ogre::MeshPtr buildPyramid();

};

#endif /* SHAPEDRAWER_H_ */
