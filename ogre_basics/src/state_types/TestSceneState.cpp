/*
 * TestSceneState.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: ros developer 
 */

#include <state_types/TestSceneState.h>
#include <utilities/ShapeDrawer.h>
#include <cmath>

const std::string TestSceneState::STATE_NAME = "TestSceneState";

TestSceneState::TestSceneState():
	TestState()
{
	// TODO Auto-generated constructor stub
	_StateName = STATE_NAME;

}

TestSceneState::~TestSceneState() {
	// TODO Auto-generated destructor stub
}

StateInterface* TestSceneState::getSingleton()
{
	if(_Instance == NULL)
	{
		TestSceneState *state = new TestSceneState();
		_Instance = state;
		state->setup();
	}

	return _Instance;
}

void TestSceneState::setupCameraControllers()
{
	TestState::setupCameraControllers();
	_CameraController.setPosition(Ogre::Vector3(-40,0,0));
}

void TestSceneState::setupSceneComponents()
{
	// creating instances of scene nodes
	Ogre::SceneManager* mngr =  StateManager::getSingleton()->getSceneManager();
	_ParentSceneNode = mngr->createSceneNode("World");


	// create grid
	Ogre::SceneNode* grid_node = _ParentSceneNode->createChildSceneNode("Grid");
	Ogre::MeshPtr grid_mesh = ShapeDrawer::getSingleton()->create_grid_mesh(100,100,20,20);
	Ogre::Entity* grid_entity = mngr->createEntity("GridObject",grid_mesh->getName());
	entities_.push_back(grid_entity);
	grid_node->attachObject(grid_entity);

	// creating boxes in spherical arrangement
	float radius = 10;
	Ogre::Vector3 origin = Ogre::Vector3(0,0,radius);
	float size = 0.2f;
	int alpha_count = 18;
	int gamma_count = 36;
	float alpha_incr = (Ogre::Math::PI)/(alpha_count - 1);
	float gamma_incr = (2 * Ogre::Math::PI)/(gamma_count);
	float alpha =  -Ogre::Math::PI/2;
	float gamma = 0;
	Ogre::MeshPtr cube_mesh = ShapeDrawer::getSingleton()->get_mesh(ShapeDrawer::BOX);

	float x,y,z;
	std::stringstream ss;
	for(int i = 1; i < alpha_count-1; i++)
	{
		alpha +=alpha_incr;
		gamma = 0;

		z = radius*std::sin(alpha) + origin.z;
		for(int j = 0; j < gamma_count; j++)
		{
			ss.str("");
			ss<<"Box"<<i<<"_"<<j;
			gamma+=gamma_incr;
			x = radius*std::cos(alpha) * std::cos(gamma) + origin.x;
			y = radius*std::cos(alpha) * std::sin(gamma) + origin.y;

			Ogre::Entity* entity = mngr->createEntity(cube_mesh->getName());
			Ogre::SceneNode* node = _ParentSceneNode->createChildSceneNode(ss.str(),Ogre::Vector3(x,y,z));
			node->attachObject(entity);
			node->setScale(size,size,size);

			entities_.push_back(entity);

			std::cout<<"Created box "<<ss.str()<<std::endl;
		}
	}

}

void TestSceneState::cleanupSceneComponents()
{
	std::cout<< STATE_NAME<<"Destroying scene components"<<std::endl;
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();

	for(std::vector<Ogre::Entity*>::iterator iter = entities_.begin();iter != entities_.end();iter++)
	{
		sceneManager->destroyEntity(*iter);
	}
	sceneManager->destroySceneNode(_ParentSceneNode);

}

