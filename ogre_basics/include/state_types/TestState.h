/*
 * TestState.h
 *
 *  Created on: Aug 19, 2012
 *      Author: coky
 *
 *  Description:
 *  	This state implementation creates a box and a plane in the scene.  It's purpose is to show the building
 *  	blocks of a state object and test the state manager.
 */

#ifndef TESTSTATE_H_
#define TESTSTATE_H_

#include <state_types/StateInterface.h>
#include <controllers/CameraController.h>

static const std::string STATE_TYPE_NAME = "TestState";

class TestState : public StateInterface
{
public:

	virtual ~TestState();

	// state control methods
	virtual void enter();
	virtual void exit();
	virtual bool pause(){return true;}
	virtual void resume(){}

	virtual void setup();
	virtual void cleanup();

	// frame callbacks
	virtual bool frameStarted(const Ogre::FrameEvent &evnt);
	virtual bool frameEnded(const Ogre::FrameEvent &evnt);

	// input callbacks
	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent &evnt);
	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent &evnt);

	static StateInterface* getSingleton();

protected:
	TestState();

	void setupSceneComponents();
	void setupCameraControllers();
	void cleanupSceneComponents();
	void cleanupCameraControllers();
	void moveCamera();

	// ogre stuff
	Ogre::SceneNode *_ParentSceneNode;

	// controllers
	CameraController _CameraController;
};

#endif /* TESTSTATE_H_ */
