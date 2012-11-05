/*
 * ShapeDrawer.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: coky
 */

#include <utilities/ShapeDrawer.h>

ShapeDrawer::ShapeDrawer() {
	// TODO Auto-generated constructor stub

}

ShapeDrawer::~ShapeDrawer() {
	// TODO Auto-generated destructor stub
}

ShapeDrawer* ShapeDrawer::getSingleton()
{
	if(_InstancePtr == NULL)
	{
		_InstancePtr = boost::make_shared<ShapeDrawer>();
	}
	return _InstancePtr.get();
}

Ogre::MeshPtr ShapeDrawer::buildBox()
{
	using namespace Ogre;

	// mesh parameters
	const size_t numVertices = 16;
	const size_t numFaces = 12;
	const size_t numVerticesPerFace = 3;

	// creating vertices
	std::vector<Ogre::Vector3> vertices {Vector3(-0.5f,-0.5f,-0.5f),
		  Vector3(+0.5f,-0.5f,-0.5f),
		  Vector3(-0.5f,+0.5f,-0.5f),
		  Vector3(+0.5f,+0.5f,-0.5f),
		  Vector3(-0.5f,-0.5f,+0.5f),
		  Vector3(+0.5f,-0.5f,+0.5f),
		  Vector3(-0.5f,+0.5f,+0.5f),
		  Vector3(+0.5f,+0.5f,+0.5f),
		  Vector3(-0.5f,-0.5f,-0.5f),
		  Vector3(+0.5f,-0.5f,-0.5f),
		  Vector3(-0.5f,+0.5f,-0.5f),
		  Vector3(+0.5f,+0.5f,-0.5f),
		  Vector3(-0.5f,-0.5f,+0.5f),
		  Vector3(+0.5f,-0.5f,+0.5f),
		  Vector3(-0.5f,+0.5f,+0.5f),
		  Vector3(+0.5f,+0.5f,+0.5f)};

	// faces array (every three elements correspond to a single triangle)
	std::vector<unsigned int> faces { 4, 6, 2, 2, 0, 4, 1, 3, 7,7, 5, 1, 8, 9, 13, 13, 12, 8,
		10, 14, 15, 15,11, 10, 8, 10, 11, 11, 9, 8, 4, 5, 7, 7, 6, 4};

	// normals array
	std::vector<Vector3> normals;
	computeNormals(vertices,faces,normals);

	// building manual object
	ManualObject obj(MESH_BOX_NAME);
	obj.begin(MESH_MATERIAL_NAME,RenderOperation::OT_TRIANGLE_LIST,ShapeDrawer::RESOURCE_GROUP_NAME);

	// adding mesh data to object
	for(unsigned int i = 0; i < faces.size(); i+=3)
	{
		for(unsigned int j = 0; j < numVerticesPerFace ; j++)
		{
			Vector3 &v = vertices[i + j];
			Vector3 &n = normals[(int)((float)i/(float)numVerticesPerFace)];

			obj.position(v.x,v.y,v.z);
			obj.normal(n);
			obj.textureCoord(v.x/100.0f,v.z/100.0f);
		}

		obj.triangle(3*i, 3*i + 1, 3*i + 2);
	}

	obj.end();

	// creating mesh
	std::string meshName = MESH_BOX_NAME + SERIALIZED_MESH_EXTENSION;
	Ogre::MeshPtr meshPtr = obj.convertToMesh(meshName,ShapeDrawer::RESOURCE_GROUP_NAME);
	return meshPtr;

}

Ogre::MeshPtr ShapeDrawer::buildSphere()
{


	Ogre::MeshPtr meshPtr;
	return meshPtr;
}

Ogre::MeshPtr ShapeDrawer::buildCylinder()
{
	Ogre::MeshPtr meshPtr;
	return meshPtr;
}

Ogre::MeshPtr ShapeDrawer::buildCapsule()
{
	Ogre::MeshPtr meshPtr;
	return meshPtr;
}

Ogre::MeshPtr ShapeDrawer::buildCylinder()
{
	Ogre::MeshPtr meshPtr;
	return meshPtr;
}

Ogre::MeshPtr ShapeDrawer::buildPyramid()
{
	Ogre::MeshPtr meshPtr;
	return meshPtr;
}
