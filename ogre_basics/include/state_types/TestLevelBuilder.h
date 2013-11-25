/*
 * TestLevelBuilder.h
 *
 *  Created on: Nov 24, 2013
 *      Author: ros developer 
 */

#ifndef TESTLEVELBUILDER_H_
#define TESTLEVELBUILDER_H_

#include <state_types/TestState.h>

class TestLevelBuilder: public TestState {

public:
	virtual ~TestLevelBuilder();

	static StateInterface* getSingleton();

public:

	static const float GRID_SIZE;
	static const int GRID_SEGMENTS;
	static const std::string STATE_NAME;

protected:
	TestLevelBuilder();
	virtual void setupSceneComponents();
	virtual void setupCameraControllers();
	virtual void cleanupSceneComponents();

	virtual bool mouseMoved(const OIS::MouseEvent &evnt); // will handle mouse movement events to move pointer

	void move_pointer_node(const OIS::MouseEvent &evnt);
	void create_voxel();

protected:

	float grid_length_;
	float grid_width_;
	int grid_segments_;
	float voxel_size_;
	Ogre::RaySceneQuery* ray_scene_query_;
	Ogre::SceneNode* pointer_node_;

	std::vector<std::string> nodes_names_;

};

#endif /* TESTLEVELBUILDER_H_ */
