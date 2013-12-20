/*
 * TestSceneLoader.h
 *
 *  Created on: Dec 20, 2013
 *      Author: ros-industrial
 */

#ifndef TESTSCENELOADER_H_
#define TESTSCENELOADER_H_

#include <state_types/TestState.h>
#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreTextAreaOverlayElement.h>

class TestSceneLoader :public TestState {
public:

	virtual ~TestSceneLoader();

protected:
	TestSceneLoader();

};

#endif /* TESTSCENELOADER_H_ */
