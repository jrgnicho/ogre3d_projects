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
#include <utilities/OgreBulletConversions.h>
#include <boost/make_shared.hpp>

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
			_SceneNode = StateManager::getSingleton()->getParentSceneNode()->createChildSceneNode(_Name,
					OgreBulletConversions::convertBulletVec3ToOgreVec3(_Transform.getOrigin()),
					OgreBulletConversions::convertBulletQuatToOgreQuat(_Transform.getRotation()));

			// add entity object that contains box mesh, later on
		}

		// dynamic components
		{
			_CollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(_BoxSides));//new btBoxShape(_BoxSides);
			if(this->_DynamicType == GameObject::DYNAMIC)// compute inertia
			{
				_CollisionShape->calculateLocalInertia(_Mass,_Inertia);
			}

			// initialize the following
			// motion state
			_MotionState = boost::make_shared<GameMotionState>(_SceneNode);

			// rigid body
			btRigidBody::btRigidBodyConstructionInfo consInfo(_Mass,_MotionState.get(),_CollisionShape.get(),_Inertia);
			_RigidBody = boost::make_shared<btRigidBody>(consInfo);//new btRigidBody(consInfo);
		}

		_Initialized = true;
	}

}
