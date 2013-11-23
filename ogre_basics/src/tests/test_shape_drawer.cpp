/*
 * test_shape_drawer.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: ros developer 
 */

#include <state_management/StateManager.h>
#include <utilities/ShapeDrawer.h>

int main(int argc,char** argv)
{
	// creating state manager
	StateManager *stateManager = StateManager::getSingleton();

	// creating shape drawer
	ShapeDrawer::ShapeDrawerPtr drawer_ptr = ShapeDrawer::getSingleton();

	// getting meshes
	Ogre::MeshPtr cube_mesh_ptr = drawer_ptr->get_mesh(ShapeDrawer::BOX);
	Ogre::MeshPtr grid_mesh_ptr = drawer_ptr->create_grid_mesh(20,20,40,40);

	return 0;


}




