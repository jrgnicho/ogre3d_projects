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

public:
	enum ShapeType
	{
		SPHERE = 1,
		BOX = 2,
		PIRAMID = 3,
		CAPSULE = 4,
		CYLINDER = 5,
		PLANE = 6
	};

	typedef boost::shared_ptr<ShapeDrawer> Ptr;
	typedef ShapeDrawer::Ptr ShapeDrawerPtr;
	typedef std::map<int,std::string> MeshMap;

public:
	virtual ~ShapeDrawer();

	static ShapeDrawer::ShapeDrawerPtr getSingleton();

	Ogre::MeshPtr get_mesh(ShapeType type);
	Ogre::MeshPtr create_grid_mesh(float width,float height,int x_segments,int y_segments);

	void exportMeshes(std::string path);

	static const std::string RESOURCE_GROUP;
	static const std::string RESOURCE_DIRECTORY;
	static const std::string MESH_BOX_NAME;
	static const std::string MESH_SPHERE_NAME;
	static const std::string MESH_PIRAMID_NAME;
	static const std::string MESH_CAPSULE_NAME;
	static const std::string MESH_CYLINDER_NAME;
	static const std::string MESH_PLANE_NAME;
	static const std::string SERIALIZED_MESH_EXTENSION;

protected:
	ShapeDrawer();

	void init();
	void close();


	Ogre::MeshPtr buildBox();
	Ogre::MeshPtr buildSphere();
	Ogre::MeshPtr buildCapsule();
	Ogre::MeshPtr buildCylinder();
	Ogre::MeshPtr buildPyramid();
	Ogre::MeshPtr buildPlane(float width,float height,int x_segments = 1,int y_segments = 1,
			float u_tile = 1.0f,float v_tile = 1.0f);

	static void computeNormals(const std::vector<Ogre::Vector3> &vertices, const std::vector<unsigned int> &faces,
			std::vector<Ogre::Vector3> &normals);

protected:

	static ShapeDrawer::ShapeDrawerPtr _InstancePtr;
	MeshMap mesh_map_;

};

#endif /* SHAPEDRAWER_H_ */
