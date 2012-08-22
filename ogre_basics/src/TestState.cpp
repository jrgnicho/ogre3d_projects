/*
 * TestState.cpp
 *
 *  Created on: Aug 19, 2012
 *      Author: coky
 */

#include <state_types/TestState.h>

StateInterface* StateInterface::_Instance = NULL;

TestState::TestState()
:StateInterface(),
_ParentSceneNode(0)
{
	// TODO Auto-generated constructor stub
	_StateName = STATE_TYPE_NAME;

}

TestState::~TestState()
{
	// TODO Auto-generated destructor stub
	std::cout<<getStateName()<<": Destructor called";
}

StateInterface* TestState::getSingleton()
{
	if(_Instance == NULL)
	{
		TestState *state = new TestState();
		_Instance = state;
		state->setup();
	}

	return _Instance;
}

void TestState::setup()
{
	StateManager::getSingleton()->manageState(this);
	setupSceneComponents();
}

void TestState::cleanup()
{
	cleanupSceneComponents();
	delete _Instance;
	//StateInterface::cleanup();
}

void TestState::setupSceneComponents()
{
	// scene element name strings
	std::string planeEntityName = "PlaneEntity",planeMeshName = "PlaneMesh";
	std::string planeMaterialName = "Examples/Rockwall", planeNodeName = "PlaneNode";
	std::string boxEntityName = "BoxEntity", boxMaterialName = "Examples/Rockwall";
	std::string boxNodeName = "BoxNode";

	// creating instances of scene nodes
	_ParentSceneNode = StateManager::getSingleton()->getSceneManager()->createSceneNode(getStateName() + "Node");
	Ogre::SceneNode *planeNode = _ParentSceneNode->createChildSceneNode(planeNodeName);
	//Ogre::SceneNode *boxNode = _ParentSceneNode->createChildSceneNode(boxNodeName);

	// creating scene visual entities
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();
	Ogre::Plane plane = Ogre::Plane(0.0f,0.0f,1.0f,0.0f);
	Ogre::MeshManager::getSingleton().createPlane(planeMeshName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane,1000,1000,20,20,true,1,5,5,Ogre::Vector3::UNIT_Y);
	Ogre::Entity *planeEntity = sceneManager->createEntity(planeEntityName,planeMeshName);
	planeEntity->setMaterialName(planeMaterialName);
	planeEntity->setCastShadows(false);
	planeNode->attachObject(planeEntity);

}

void TestState::cleanupSceneComponents()
{
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();
	sceneManager->destroySceneNode(_ParentSceneNode);
	sceneManager->destroyEntity("PlaneEntity");

}

void TestState::moveCamera()
{

}

void TestState::enter()
{
	StateManager::getSingleton()->addChildSceneNode(_ParentSceneNode);
}

void TestState::exit()
{
	StateManager::getSingleton()->removeChildSceneNode(_ParentSceneNode);
}

bool TestState::frameStarted(const Ogre::FrameEvent &evnt)
{
	return true;
}

bool TestState::frameEnded(const Ogre::FrameEvent &evnt)
{
	if(!processUnbufferedKeyInput(evnt))
	{
		return false;
	}

	if(!processUnbufferedMouseInput(evnt))
	{
		return false;
	}

	moveCamera();

	return true;
}

bool TestState::processUnbufferedKeyInput(const Ogre::FrameEvent &evnt)
{
	return true;
}

bool TestState::processUnbufferedMouseInput(const Ogre::FrameEvent &evnt)
{
	return true;
}
