/*
 * ShapeDrawer.cpp
 *
 *  Created on: Oct 21, 2012
 *      Author: coky
 */

#include <utilities/ShapeDrawer.h>
#include <utilities/MaterialCreator.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreSubMesh.h>
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <boost/assign/list_inserter.hpp> // for 'push_back()'
#include <boost/assign/list_of.hpp>
#include <boost/assert.hpp>
#include <algorithm>
#include <functional>

using namespace boost::assign;

const std::string ShapeDrawer::RESOURCE_GROUP="UserMeshes";
const std::string ShapeDrawer::RESOURCE_DIRECTORY="user_media/meshes";
const std::string ShapeDrawer::MESH_BOX_NAME="BoxMesh";
const std::string ShapeDrawer::MESH_SPHERE_NAME= "SphereMesh";
const std::string ShapeDrawer::MESH_PIRAMID_NAME = "PiramidMesh";
const std::string ShapeDrawer::MESH_CAPSULE_NAME = "CapsuleMesh";
const std::string ShapeDrawer::MESH_CYLINDER_NAME = "CylinderMesh";
const std::string ShapeDrawer::MESH_PLANE_NAME = "PlaneMesh";
const std::string ShapeDrawer::SERIALIZED_MESH_EXTENSION = ".mesh";

ShapeDrawer::ShapeDrawerPtr ShapeDrawer::_InstancePtr;

ShapeDrawer::ShapeDrawer() {
	// TODO Auto-generated constructor stub
	init();

}

ShapeDrawer::~ShapeDrawer() {
	// TODO Auto-generated destructor stub
	//close();
}

ShapeDrawer::ShapeDrawerPtr ShapeDrawer::getSingleton()
{
	if(!_InstancePtr)
	{
		_InstancePtr = ShapeDrawerPtr(new ShapeDrawer());
	}
	return _InstancePtr;
}

void ShapeDrawer::init()
{
	// create resource group
	std::cout<<"Creating ShapeDrawer Resource Group"<<std::endl;
	Ogre::ResourceGroupManager& r = Ogre::ResourceGroupManager::getSingleton();
	r.createResourceGroup(RESOURCE_GROUP);
	r.addResourceLocation(RESOURCE_DIRECTORY,"FileSystem",RESOURCE_GROUP);
	r.initialiseResourceGroup(RESOURCE_GROUP);
	r.loadResourceGroup(RESOURCE_GROUP);

	// creting meshes
	std::cout<<"Creating Meshes"<<std::endl;
	mesh_map_ = MeshMap();
	mesh_map_.insert(std::make_pair((int)ShapeDrawer::BOX,buildBox()->getName()));
	mesh_map_.insert(std::make_pair((int)ShapeDrawer::PLANE,buildPlane(1.0,1.0,1,1,1,1)->getName()));

	std::cout<<"Completed ShapeDrawer initialization"<<std::endl;
}

void ShapeDrawer::close()
{

	std::cout<<"Destroying ShapeDrawer Resource Group "<<RESOURCE_GROUP<<"\n";
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(RESOURCE_GROUP);
}

Ogre::MeshPtr ShapeDrawer::get_mesh(ShapeType type)
{
	Ogre::MeshPtr m;
	if(mesh_map_.count((int)type)> 0)
	{
		m = Ogre::MeshManager::getSingletonPtr()->getByName(mesh_map_[(int)type],RESOURCE_GROUP);
	}

	return m;
}

void ShapeDrawer::exportMeshes(std::string path)
{
	std::cout<<"Not implemented"<<std::endl;
	return;
}

Ogre::MeshPtr ShapeDrawer::buildBox()
{
	using namespace Ogre;

	// mesh parameters
	const size_t num_vertices = 36;
	const size_t num_faces = 12;
	const size_t num_vertices_per_face = 3;


	using namespace Ogre;
	// creating vertices
	std::vector<Ogre::Vector3> vertices;
	vertices += Vector3(0.5f,0.5f,0.5f),	// top face 1
			Vector3(-0.5f,0.5f,0.5f),
			Vector3(-0.5f,-0.5f,0.5f),
			Vector3(-0.5f,-0.5f,0.5f),	// top face 2
			Vector3(0.5f,-0.5f,0.5f),
			Vector3(0.5f,0.5f,0.5f);

	// Rotation array will be used to produce remaining faces by rotating top face
	std::vector<Ogre::Quaternion> rots;
	std::function<Quaternion (Real,Vector3)> create_quaternion =
			[](Real angle,Vector3 axis) -> Quaternion {return Quaternion(Radian(angle),axis);};
	rots += create_quaternion(Ogre::Math::PI/2,Vector3(0,1,0)),	// right face
			create_quaternion(Ogre::Math::PI,Vector3(0,1,0)),		// bottom face
			create_quaternion(3*Ogre::Math::PI/2,Vector3(0,1,0)),	// left face
			create_quaternion(Ogre::Math::PI/2,Vector3(-1,0,0)),	// rear face
			create_quaternion(Ogre::Math::PI/2,Vector3(1,0,0));	// front face

	for(int i = 0; i < rots.size(); i++)
	{
		for(int j = 0;j < num_vertices_per_face * 2; j++)
		{
			vertices.push_back(rots[i]*vertices[j]);
		}
	}



	// faces array (every three elements correspond to a single triangle)
	std::vector<unsigned int> faces;
	for(int i = 0; i < num_vertices; i++){faces.push_back(i);}


	// normals array
	std::vector<Vector3> normals;
	computeNormals(vertices,faces,normals);

	// building manual object

	ManualObject obj(MESH_BOX_NAME);
	Ogre::MaterialPtr m_ptr = MaterialCreator::get_singleton()->get_material(MaterialCreator::BLUE);
	obj.begin(m_ptr->getName(),RenderOperation::OT_TRIANGLE_LIST,ShapeDrawer::RESOURCE_GROUP);

	// adding mesh data to object
	for(unsigned int i = 0; i < faces.size(); i+=num_vertices_per_face)
	{
		for(unsigned int j = 0; j < num_vertices_per_face; j++)
		{
			Vector3 &v = vertices[i + j];
			Vector3 &n = normals[i/3];

			obj.position(v.x,v.y,v.z);
			//obj.colour(Ogre::ColourValue(0,0,1.0f,1.0f));
			obj.normal(n);
			//obj.textureCoord(v.x/100.0f,v.z/100.0f);
		}

		obj.triangle(i, i + 1, i + 2);
	}

	obj.end();

	// creating mesh
	std::string meshName = MESH_BOX_NAME + SERIALIZED_MESH_EXTENSION;
	Ogre::MeshPtr meshPtr = obj.convertToMesh(meshName,ShapeDrawer::RESOURCE_GROUP);
	return meshPtr;

}

Ogre::MeshPtr ShapeDrawer::buildPlane(float width,float height,int x_segments,int y_segments,
		float u_tile ,float v_tile)
{

	// create name
	std::stringstream ss;
	int counter = 0;
	std::string name = MESH_PLANE_NAME + "0" + SERIALIZED_MESH_EXTENSION;
	while(Ogre::MeshManager::getSingletonPtr()->resourceExists(name))
	{
		ss.str("");
		ss<<MESH_PLANE_NAME<<++counter<<SERIALIZED_MESH_EXTENSION;
		name = ss.str();
	}

	Ogre::Plane plane = Ogre::Plane(0.0f,0.0f,1.0f,0.0f);
	Ogre::MeshPtr mesh_ptr = Ogre::MeshManager::getSingleton().createPlane(name,RESOURCE_GROUP,
			plane,width,height,x_segments,y_segments,true,1,u_tile,v_tile,Ogre::Vector3::UNIT_Y);

	return mesh_ptr;

}

Ogre::MeshPtr ShapeDrawer::create_grid_mesh(float width,float height,int x_segments,int y_segments)
{

	// create name
	std::stringstream ss;
	int counter = 0;
	std::string name = MESH_PLANE_NAME + "0";
	while(Ogre::MeshManager::getSingletonPtr()->resourceExists(name))
	{
		ss.str("");
		ss<<MESH_PLANE_NAME<<++counter;
		name = ss.str();
	}

	Ogre::MeshPtr mesh_ptr = buildPlane(width,height,x_segments,y_segments,1,1);

	// applying material to all submeshes

	Ogre::Mesh::SubMeshIterator iter= mesh_ptr->getSubMeshIterator();
	while(iter.hasMoreElements())
	{
		Ogre::SubMesh* sm = iter.getNext();
		sm->setMaterialName(MaterialCreator::get_singleton()->get_material(MaterialCreator::WIREFRAME_GRAY)->getName(),
				MaterialCreator::RESOURCE_GROUP);
	}
	mesh_ptr->updateMaterialForAllSubMeshes();

	return mesh_ptr;
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


Ogre::MeshPtr ShapeDrawer::buildPyramid()
{
	Ogre::MeshPtr meshPtr;
	return meshPtr;
}

void ShapeDrawer::computeNormals(const std::vector<Ogre::Vector3> &vertices, const std::vector<unsigned int> &faces,
		std::vector<Ogre::Vector3> &normals)
{

	// creates 2 vectors from the vertices of each face and computes the normal vector from the cross-product of the two
	Ogre::Vector3 vec1 = Ogre::Vector3::ZERO;
	Ogre::Vector3 vec2= Ogre::Vector3::ZERO;
	Ogre::Vector3 normal= Ogre::Vector3::ZERO;
	for(int i = 0; i < faces.size(); i +=3)
	{
		normal = Ogre::Vector3::ZERO;
		vec1 = vertices[faces[i+1]] - vertices[faces[i]];
		vec2 = vertices[faces[i+2]] - vertices[faces[i+1]];
		normal = vec1.crossProduct(vec2);
		normal.normalise();
		normals.push_back(normal);
	}
}
