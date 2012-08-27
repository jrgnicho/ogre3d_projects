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
_ParentSceneNode(0),
_CameraController()
{
	// TODO Auto-generated constructor stub
	_StateName = STATE_TYPE_NAME;

}

TestState::~TestState()
{
	// TODO Auto-generated destructor stub
	std::cout<<getStateName()<<": Destructor called"<<"\n";
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
	setupCameraControllers();
}

void TestState::cleanup()
{
	std::cout<<getStateName() + ": cleanup started"<<"\n";
	cleanupSceneComponents();
	cleanupCameraControllers();
	std::cout<<getStateName() + ": cleanup ended"<<"\n";
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
			plane,1000,1000,20,20,true,1,100,100,Ogre::Vector3::UNIT_Y);
	Ogre::Entity *planeEntity = sceneManager->createEntity(planeEntityName,planeMeshName);
	planeEntity->setMaterialName(planeMaterialName);
	planeEntity->setCastShadows(false);
	planeNode->attachObject(planeEntity);

}

void TestState::setupCameraControllers()
{
	_CameraController.setup();
}

void TestState::cleanupCameraControllers()
{
	_CameraController.cleanup();
}

void TestState::cleanupSceneComponents()
{
	std::cout<<getStateName() + ": Destroying scene components"<<"\n";
	_ParentSceneNode->detachAllObjects();
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();
	sceneManager->destroyEntity("PlaneEntity");
	sceneManager->destroySceneNode(_ParentSceneNode);

}

void TestState::moveCamera()
{
	_CameraController.moveCamera();
}

void TestState::enter()
{
	if(_ParentSceneNode != NULL && !_ParentSceneNode->isInSceneGraph())
	{
		StateManager::getSingleton()->addChildSceneNode(_ParentSceneNode);
		_CameraController.connectToScene();
	}
}

void TestState::exit()
{
	if(_ParentSceneNode != NULL && _ParentSceneNode->isInSceneGraph())
	{
		StateManager::getSingleton()->removeChildSceneNode(_ParentSceneNode);
		_CameraController.disconnectFromScene();
	}
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
	OIS::Keyboard *keyboard = (StateManager::getSingleton()->getInputManager()).getKeyboard();
	if(keyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		StateManager::getSingleton()->shutdown();
	}
	else
	{
		_CameraController.applyKeyboardState(keyboard,evnt.timeSinceLastFrame);
	}
	return true;
}

bool TestState::processUnbufferedMouseInput(const Ogre::FrameEvent &evnt)
{
	const OIS::MouseState &ms = StateManager::getSingleton()->getInputManager().getMouse()->getMouseState();
	_CameraController.applyMouseState(ms,evnt.timeSinceLastFrame);

	return true;
}
