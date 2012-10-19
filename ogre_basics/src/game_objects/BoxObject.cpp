/*
 * BoxObject.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: coky
 */

#include <game_objects/BoxObject.h>
#include <OgreSceneManager.h>
#include <state_management/StateManager.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>

BoxObject::BoxObject(btVector3 sides,GameObject::DynamicType type,btScalar mass,btTransform t,std::string name)
:GameObject(BOX_SHAPE_PROXYTYPE,type,mass,t,name),
_BoxSides(sides)
{
	// TODO Auto-generated constructor stub

}

BoxObject::~BoxObject() {
	// TODO Auto-generated destructor stub
}

void BoxObject::initialise()
{
	if(!_Initialized)
	{
		// visual components
		{
			_SceneNode = StateManager::getSingleton()->getSceneManager()->createSceneNode(_Name);
			// add entity object that contains box mesh, later on
		}

		// dynamic components
		{
			_CollisionShape = new btBoxShape(_BoxSides);
			if(this->_DynamicType == GameObject::DYNAMIC)// compute inertia
			{
				_CollisionShape->calculateLocalInertia(_Mass,_Inertia);
			}

			// initialize the following
			// motion state
			_MotionState = new GameMotionState(_Transform,_SceneNode);

			// rigid body
			btRigidBody::btRigidBodyConstructionInfo consInfo(_Mass,_MotionState,_CollisionShape,_Inertia);
			_RigidBody = new btRigidBody(consInfo);
		}

		_Initialized = true;
	}

}
