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
	static const std::string MESH_MATERIAL_NAME;
	static const std::string MESH_BOX_NAME;
	static const std::string MESH_SPHERE_NAME;
	static const std::string MESH_PIRAMID_NAME;
	static const std::string MESH_CAPSULE_NAME;
	static const std::string MESH_CYLINDER_NAME;
	static const std::string SERIALIZED_MESH_EXTENSION;

protected:
	ShapeDrawer();

	boost::shared_ptr<ShapeDrawer> _InstancePtr;

	Ogre::MeshPtr buildBox();
	Ogre::MeshPtr buildSphere();
	Ogre::MeshPtr buildCapsule();
	Ogre::MeshPtr buildCylinder();
	Ogre::MeshPtr buildPyramid();

	static void computeNormals(const std::vector<Ogre::Vector3> &vertices, const std::vector<unsigned int> &faces,
			std::vector<Ogre::Vector3> &normals);

};

#endif /* SHAPEDRAWER_H_ */
