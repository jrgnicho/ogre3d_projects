/*
 * TestLevelBuilder.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: ros developer 
 */

#include <state_types/TestLevelBuilder.h>
#include <utilities/ShapeDrawer.h>
#include <utilities/MaterialCreator.h>
#include <cmath>

const float TestLevelBuilder::GRID_SIZE = 40.0f;
const int TestLevelBuilder::GRID_SEGMENTS = 80;
const std::string TestLevelBuilder::STATE_NAME = "TestLevelBuilder";

TestLevelBuilder::TestLevelBuilder():
		grid_width_(GRID_SIZE),
		grid_length_(GRID_SIZE),
		grid_segments_(GRID_SEGMENTS),
		voxel_size_(GRID_SIZE/GRID_SEGMENTS)
	{
	// TODO Auto-generated constructor stub
	_StateName = STATE_NAME;

}

TestLevelBuilder::~TestLevelBuilder() {
	// TODO Auto-generated destructor stub
}

StateInterface* TestLevelBuilder::getSingleton()
{
	if(_Instance == NULL)
	{
		TestLevelBuilder *state = new TestLevelBuilder();
		_Instance = state;
		StateManager::getSingleton()->manageState(_Instance);
		//state->setup(); // called by SceneManager
	}

	return _Instance;
}

void TestLevelBuilder::setupSceneComponents()
{
	// creating instances of scene nodes
	Ogre::SceneManager* mngr =  StateManager::getSingleton()->getSceneManager();
	_ParentSceneNode = mngr->createSceneNode("World");

	// create grid
	std::string grid_node_name = "GridNode";
	Ogre::SceneNode* grid_node = _ParentSceneNode->createChildSceneNode(grid_node_name);
	Ogre::MeshPtr grid_mesh = ShapeDrawer::getSingleton()->create_grid_mesh(grid_width_,grid_length_,grid_segments_,grid_segments_);
	Ogre::Entity* grid_entity = mngr->createEntity("GridObject",grid_mesh->getName());
	nodes_names_.push_back(grid_node_name);
	grid_node->attachObject(grid_entity);

	// create voxel pointer
	Ogre::MeshPtr cube_mesh = ShapeDrawer::getSingleton()->get_mesh(ShapeDrawer::BOX);
	Ogre::MaterialPtr cube_material= MaterialCreator::get_singleton()->get_material(MaterialCreator::TRANSPARENT_RED);
	Ogre::Entity* entity = mngr->createEntity(cube_mesh->getName());
	entity->setMaterial(cube_material);
	pointer_node_ = _ParentSceneNode->createChildSceneNode("PointerNode",Ogre::Vector3(voxel_size_/2,voxel_size_/2,voxel_size_/2));
	pointer_node_->attachObject(entity);
	pointer_node_->setScale(voxel_size_,voxel_size_,voxel_size_);

	// creating ray scene query object
	ray_scene_query_ = mngr->createRayQuery(Ogre::Ray());

}


void TestLevelBuilder::setupCameraControllers()
{
	TestState::setupCameraControllers();
	_CameraController.setPosition(Ogre::Vector3(-10,0,0.5f));
}

void TestLevelBuilder::cleanupSceneComponents()
{
	std::cout<< STATE_NAME<<"Destroying scene components"<<std::endl;
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();

	for(std::vector<std::string>::iterator iter = nodes_names_.begin();iter != nodes_names_.end();iter++)
	{
		Ogre::SceneNode* node = sceneManager->getSceneNode(*iter);
		Ogre::SceneNode::ObjectIterator i = node->getAttachedObjectIterator();
		while(i.hasMoreElements())
		{
			sceneManager->destroyEntity(i.getNext()->getName());
		}
		node->detachAllObjects();
		sceneManager->destroySceneNode(node);
	}
	sceneManager->destroySceneNode(_ParentSceneNode);

}

bool TestLevelBuilder::mouseMoved(const OIS::MouseEvent &evnt)
{
	move_pointer_node(evnt);
	return true;
}

void TestLevelBuilder::move_pointer_node(const OIS::MouseEvent &evnt)
{
	float x = evnt.state.X.abs/float(_CameraController.getCamera()->getViewport()->getActualWidth());
	float y = evnt.state.Y.abs/float(_CameraController.getCamera()->getViewport()->getActualHeight());


	//std::cout<<"Performing ray query for mouse coordinates: "<<x <<", "<< y <<std::endl;
	Ogre::Ray ray = _CameraController.getCamera()->getCameraToViewportRay(x,y);

	Ogre::Vector3 dir = ray.getDirection();
	Ogre::Vector3 pos = _CameraController.getCamera()->getRealPosition();
	ray = Ogre::Ray(pos,dir);

	// performing query

	ray_scene_query_->clearResults();
	ray_scene_query_->setRay(ray);
	Ogre::RaySceneQueryResult& result = ray_scene_query_->execute();
	Ogre::RaySceneQueryResult::iterator i = result.begin();

	if(i != result.end() && i->worldFragment)
	{
		std::cout<<"Moving voxel pointer"<<std::endl;
		Ogre::Vector3 pos = i->worldFragment->singleIntersection;
		pos.x = std::floor(pos.x) + voxel_size_/2.0f;
		pos.y = std::floor(pos.y) + voxel_size_/2.0f;
		pos.z = std::floor(pos.z) + voxel_size_/2.0f;

		pointer_node_->setPosition(pos);
		i++;
	}

}

void TestLevelBuilder::create_voxel()
{

}



