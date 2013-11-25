/*
 * TestSceneState.h
 *
 *  Created on: Nov 23, 2013
 *      Author: ros developer 
 */

#ifndef TESTSCENESTATE_H_
#define TESTSCENESTATE_H_

#include <state_types/TestState.h>

class TestSceneState : public TestState
{
public:
	typedef boost::shared_ptr<TestSceneState> Ptr;

public:
	virtual ~TestSceneState();

	static StateInterface* getSingleton();

public:

	static const std::string STATE_NAME;

protected:
	TestSceneState();
	virtual void setupSceneComponents();
	virtual void setupCameraControllers();
	virtual void cleanupSceneComponents();


protected:

	std::vector<Ogre::Entity*> entities_;
};

#endif /* TESTSCENESTATE_H_ */
