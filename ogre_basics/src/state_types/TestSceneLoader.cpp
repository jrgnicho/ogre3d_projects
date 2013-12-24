/*
 * TestSceneLoader.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: ros-industrial
 */

#include <state_types/TestSceneLoader.h>
#include <utilities/DotSceneLoader.h>

const std::string TestSceneLoader::SCENE_RESOURCE = "Mountains.scene";
const std::string TestSceneLoader::STATE_NAME = "SceneLoader";

TestSceneLoader::TestSceneLoader() {
	// TODO Auto-generated constructor stub
	_StateName = STATE_NAME;

}

StateInterface* TestSceneLoader::getSingleton()
{
	if(_Instance == NULL)
	{
		TestSceneLoader *state = new TestSceneLoader();
		_Instance = state;
		StateManager::getSingleton()->manageState(_Instance);
		//state->setup(); // called by SceneManager
	}

	return _Instance;
}

TestSceneLoader::~TestSceneLoader() {
	// TODO Auto-generated destructor stub
}

void TestSceneLoader::setupSceneComponents()
{
	// creating instances of scene nodes
	Ogre::SceneManager* mngr =  StateManager::getSingleton()->getSceneManager();
	_ParentSceneNode = mngr->createSceneNode("World");


	if(Ogre::ResourceGroupManager::getSingleton().resourceExists(
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,SCENE_RESOURCE))
	{
		// loading scene
		std::cout<<"Loading scene file "<<SCENE_RESOURCE<<std::endl;
		Ogre::DotSceneLoader scene_loader;
		scene_loader.parseDotScene(SCENE_RESOURCE,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,mngr,NULL,_ParentSceneNode->getName());
	}
	else
	{

		std::cout<<"Scene file resource "<<SCENE_RESOURCE<<" was not found in group "<<
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME<<std::endl;
	}

}

void TestSceneLoader::setupCameraControllers()
{
	TestState::setupCameraControllers();
	_CameraController.setPosition(Ogre::Vector3(-10,0,2));
}

void TestSceneLoader::cleanupSceneComponents()
{

	std::cout<< STATE_NAME<<"Destroying scene components"<<std::endl;
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();

	sceneManager->destroySceneNode(_ParentSceneNode);
}
