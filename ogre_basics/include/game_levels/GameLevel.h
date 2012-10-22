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
#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include <boost/shared_ptr.hpp>

class GameLevel: public GameObject
{
public:
	GameLevel(btTransform t = btTransform(),std::string name = "")
	:GameObject(STATIC_PLANE_PROXYTYPE,
			GameObject::STATIC,
			0.0f,
			t,
			name.empty() ? generateName() : name),
	_SolverIterations(20),
	_SolverMode(SOLVER_SIMD | SOLVER_USE_WARMSTARTING),
	_EnableSpu(true),
	_MaxSubSteps(4),
	_FixedTimeStep(btScalar(1.0f)/btScalar(80.0f)),
	_Gravity(0.0f,0.0f,9.81f)
	{
		// TODO Auto-generated constructor stub
		_InstanceCount++;

	}

	virtual ~GameLevel()
	{

	}

	virtual void addGameObject(GameObject::Ptr obj) = 0;

	virtual void removeGameObject(GameObject::Ptr obj) = 0;

	virtual void stepSimulation(btScalar seconds)
	{
		_DynamicsWorld->stepSimulation(seconds,_MaxSubSteps,_FixedTimeStep);
	}

	static std::string generateName()
	{
		std::stringstream name;
		name<<"GameLevel"<<_InstanceCount;
		return name.str();
	}

protected:

	// object container
	std::map<std::string,GameObject::Ptr> _GameObjects;

	// simulation elements
	boost::shared_ptr<btBroadphaseInterface> _BroadphaseInterface;
	boost::shared_ptr<btCollisionConfiguration> _CollisionConfiguration;
	boost::shared_ptr<btCollisionDispatcher> _CollisionDispatcher;
	boost::shared_ptr<btConstraintSolver> _ConstraintSolver;
	boost::shared_ptr<btDiscreteDynamicsWorld> _DynamicsWorld;

	// simulation properties:  explanation for the two parameters below can be found at:
	//		http://bulletphysics.org/mediawiki-1.5.8/index.php/Stepping_the_World
	btScalar _FixedTimeStep;
	int _MaxSubSteps;

	// dynamic properties
	btVector3 _Gravity;
	int _SolverIterations;
	int _SolverMode;
	bool _EnableSpu;

private:

	static int _InstanceCount;
};

int GameLevel::_InstanceCount = 0;

#endif /* GAMELEVEL_H_ */
