/*
 * GameLevel.h
 *
 *  Created on: Oct 10, 2012
 *      Author: coky
 */

#ifndef GAMELEVEL_H_
#define GAMELEVEL_H_

//#define USE_PARALLEL_DISPATCHER
//#define USE_PARALLEL_SOLVER
//#define USE_PTHREADS

#include <game_objects/GameObject.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>

int GameLevel::_InstanceCount = 0;
class GameLevel: public GameObject
{
public:
	GameLevel(btTransform t = btTransform(),std::string name = "")
	:GameObject(GameObject::STATIC,0.0f,name.empty() ? getName() : name)
	{
		// TODO Auto-generated constructor stub
		_InstanceCount++;

	}

	virtual ~GameLevel()
	{

	}

	virtual std::string getName()
	{
		std::stringstream name;
		name<<"GameLevel"<<_InstanceCount;
		return name.str();
	}

	virtual void addGameObject(GameObject* obj)
	{
		_GameObjects.insert(std::make_pair(obj->getName(),obj));
		if(_SceneNode != NULL)
		{
			_SceneNode->addChild(obj->getSceneNode());
		}
	}

protected:

	// object container
	std::map<std::string,GameObject*> _GameObjects;

	// simulation elements
	btBroadphaseInterface* _BroadphaseInterface;
	btCollisionConfiguration* _CollisionConfiguration;
	btCollisionDispatcher* _CollisionDispatcher;
	btConstraintSolver* _ConstraintSolver;
	btDiscreteDynamicsWorld* _DynamicsWorld;

	// simulation properties
	btScalar _TimeStep;
	btScalar _FixedTimeStep;
	int _MaxSubSteps;

private:

	static int _InstanceCount;
};

#endif /* GAMELEVEL_H_ */
