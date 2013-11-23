/*
 * test_material_creator.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: ros developer 
 */
#include <state_management/StateManager.h>
#include <utilities/MaterialCreator.h>
#include <OGRE/Ogre.h>

int main(int argc,char** argv)
{
	StateManager* sm_ptr = StateManager::getSingleton();
	MaterialCreator::Ptr mc_ptr = MaterialCreator::get_singleton();

	return 0;
}



