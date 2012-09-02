/*
 * TestDisplayImageState.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: coky
 */

#include <state_types/TestDisplayImageState.h>

const std::string TestDisplayImageState::STATE_TYPE_NAME = "TestDisplayImageState";
StateInterface* TestDisplayImageState::_Instance = NULL;

TestDisplayImageState::TestDisplayImageState()
:StateInterface(),
_BackgroundMaterial(0),
 _DrawingRectangle(0),
 _SceneNode(0),
 _DisplayCamera(0),
 _MaterialName("DisplayMaterial"),
 _ImageName(TEXTURE_NAME),
 _NodeName("DisplayImageNode"),
 _CameraName("DisplayCamera")
{
	// TODO Auto-generated constructor stub
	_StateName = STATE_TYPE_NAME;

}

TestDisplayImageState::~TestDisplayImageState() {
	// TODO Auto-generated destructor stub
	cleanup();
}

StateInterface* TestDisplayImageState::getSingleton()
{
	if(_Instance == NULL)
	{
		_Instance = new TestDisplayImageState();
		_Instance->setup();
	}

	return _Instance;
}

void TestDisplayImageState::setup()
{
	// setting material with loaded texture
	if(_BackgroundMaterial == NULL)
	{
		_BackgroundMaterial = (Ogre::Material*)((Ogre::MaterialManager::getSingleton().create(_MaterialName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)).getPointer());
		_BackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(_ImageName);
		_BackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		_BackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		_BackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	}


	if(_DrawingRectangle == NULL)
	{
		// creating drawing rectangle on which to draw the texture defined in the newly created material
		_DrawingRectangle = new Ogre::Rectangle2D(true);
		_DrawingRectangle->setMaterial(_BackgroundMaterial->getName());
		_DrawingRectangle->setCorners(-1.0f,1.0f,1.0f,-1.0f);
		_DrawingRectangle->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND); // render before all other objects
		_DrawingRectangle->setBoundingBox(
				Ogre::AxisAlignedBox(Ogre::AxisAlignedBox::EXTENT_INFINITE)); // ensures that it is always visible
	}

	if(_SceneNode == NULL)
	{
		// creating scene node for rectangle
		_SceneNode = StateManager::getSingleton()->getSceneManager()->createSceneNode(_NodeName);
		_SceneNode->attachObject(_DrawingRectangle);
	}

	if(_DisplayCamera == NULL)
	{
		// creating camera
		_DisplayCamera = StateManager::getSingleton()->getSceneManager()->createCamera(_CameraName);
		_DisplayCamera->setNearClipDistance(5.0f);
		_DisplayCamera->setFarClipDistance(1000.0f);
	}
}

void TestDisplayImageState::cleanup()
{
	if(_BackgroundMaterial != NULL)
	{
		// this removal attempt may throw an error
		Ogre::MaterialManager::getSingleton().destroyResourcePool(_BackgroundMaterial->getName());
		_BackgroundMaterial = NULL;
	}

	if(_DrawingRectangle != NULL)
	{
		_SceneNode->detachObject(_DrawingRectangle);
		delete _DrawingRectangle;
		_DrawingRectangle = NULL;
	}

	if(_SceneNode != NULL)
	{
		StateManager::getSingleton()->getSceneManager()->destroySceneNode(_SceneNode);
		_SceneNode = NULL;
	}

	if(_DisplayCamera != NULL)
	{

		StateManager::getSingleton()->getSceneManager()->destroyCamera(_DisplayCamera);
		_DisplayCamera = NULL;
	}
}

bool TestDisplayImageState::pause()
{
	exit();
	return true;
}

void TestDisplayImageState::resume()
{
	enter();
}

void TestDisplayImageState::enter()
{
	// adding scene node
	StateManager::getSingleton()->getParentSceneNode()->addChild(_SceneNode);

	// adding camera
	Ogre::Viewport *viewport = StateManager::getSingleton()->getRenderWindow()->addViewport(_DisplayCamera);
	_DisplayCamera->setAspectRatio(((float)viewport->getActualWidth())/((float)viewport->getActualHeight()));

}

void TestDisplayImageState::exit()
{
	// removing scene node
	StateManager::getSingleton()->getParentSceneNode()->removeChild(_SceneNode);

	// remove viewport;
	StateManager::getSingleton()->getRenderWindow()->removeViewport(_DisplayCamera->getViewport()->getZOrder());
}

bool TestDisplayImageState::frameStarted(const Ogre::FrameEvent &evnt)
{
	return true;
}

bool TestDisplayImageState::frameEnded(const Ogre::FrameEvent &evnt)
{
	if(!processUnbufferedKeyInput(evnt))
	{
		return false;
	}

	return true;
}

bool TestDisplayImageState::keyPressed(const OIS::KeyEvent &evnt)
{
	if(evnt.key == OIS::KC_ESCAPE)
	{
		popState();
		return true; // end here
	}

	if(evnt.key == OIS::KC_N)
	{
		changeState();
		return true;
	}

	if(evnt.key == OIS::KC_P)
	{
		pushState();
		return true;
	}

	return true;
}

bool TestDisplayImageState::processUnbufferedKeyInput(const Ogre::FrameEvent &evnt)
{
	OIS::Keyboard *keyboard = StateManager::getSingleton()->getInputManager().getKeyboard();
	return true;
}

bool TestDisplayImageState::processUnbufferedMouseInput(const Ogre::FrameEvent &evnt)
{
	return true;
}


