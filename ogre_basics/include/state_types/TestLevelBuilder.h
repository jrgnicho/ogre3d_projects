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


	// frame callbacks
	virtual bool frameStarted(const Ogre::FrameEvent &evnt);

public:

	static const float GRID_SIZE;
	static const int GRID_SEGMENTS;
	static const std::string STATE_NAME;
	static const std::string GRID_NODE;
	static const std::string GRID_ENTITY;

protected:
	TestLevelBuilder();
	virtual void setupSceneComponents();
	virtual void setupCameraControllers();
	virtual void cleanupSceneComponents();

	virtual bool mousePressed(const OIS::MouseEvent &evnt,OIS::MouseButtonID id);
	void move_pointer_node(const OIS::MouseState &ms);
	void create_voxel(const Ogre::Vector3& pos);

protected:

	float grid_length_;
	float grid_width_;
	int grid_segments_;
	float voxel_size_;
	Ogre::RaySceneQuery* ray_scene_query_;
	Ogre::SceneNode* pointer_node_;
	std::string pointer_entity_name_;
	bool create_new_voxel_;

	std::vector<std::string> nodes_names_;

};

#endif /* TESTLEVELBUILDER_H_ */
