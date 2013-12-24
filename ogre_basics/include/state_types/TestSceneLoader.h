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
#include <boost/shared_ptr.hpp>

class TestSceneLoader :public TestState {

public:

	virtual ~TestSceneLoader();

	static StateInterface* getSingleton();

protected:

	TestSceneLoader();
	virtual void setupSceneComponents();
	virtual void setupCameraControllers();
	virtual void cleanupSceneComponents();

protected:

	static const std::string SCENE_RESOURCE;
	static const std::string STATE_NAME;

};
typedef boost::shared_ptr<TestSceneLoader> TestSceneLoaderPtr;

#endif /* TESTSCENELOADER_H_ */
