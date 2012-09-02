/*
 * TestDisplayImageState.h
 *
 *  Created on: Sep 1, 2012
 *      Author: coky
 */

#ifndef TESTDISPLAYIMAGESTATE_H_
#define TESTDISPLAYIMAGESTATE_H_

#include <state_types/StateInterface.h>
#include <OGRE/OgreMaterial.h>

const std::string TEXTURE_NAME = "DiplayImage_1024x768.jpg";

class TestDisplayImageState : public StateInterface
{
public:

	virtual ~TestDisplayImageState();

	// state control methods
	virtual void enter();
	virtual void exit();
	virtual bool pause();
	virtual void resume();

	virtual void setup();
	virtual void cleanup();

	// frame callbacks
	virtual bool frameStarted(const Ogre::FrameEvent &evnt);
	virtual bool frameEnded(const Ogre::FrameEvent &evnt);

	// input callbacks
	virtual bool keyPressed(const OIS::KeyEvent &evnt);
	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent &evnt);
	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent &evnt);

	static StateInterface* getSingleton();

	static const std::string STATE_TYPE_NAME;

protected:

	// constructor
	TestDisplayImageState();

	// ogre members
	Ogre::SceneNode *_SceneNode;
	Ogre::Material *_BackgroundMaterial;
	Ogre::Rectangle2D *_DrawingRectangle;
	Ogre::Camera *_DisplayCamera;

	// string names
	std::string _ImageName;
	std::string _MaterialName;
	std::string _NodeName;
	std::string _CameraName;

	// singleton instance
	static StateInterface *_Instance;
};

#endif /* TESTDISPLAYIMAGESTATE_H_ */
