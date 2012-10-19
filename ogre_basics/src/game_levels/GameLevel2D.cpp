/*
 * GameLevel2D.cpp
 *
 *  Created on: Oct 14, 2012
 *      Author: coky
 */

#include <game_levels/GameLevel2D.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

GameLevel2D::GameLevel2D(btTransform &trans,std::string name)
:GameLevel(trans,name),
 _Gravity(0.0f,-9.81f,0.0f)
{
	// TODO Auto-generated constructor stub
	_LinearFactor = btVector3(1.0f,1.0f,0.0f); // limits motion to x-y plane only no motion in or out of the screen
	_Gravity = btVector3(0.0f,-9.81f,0.0f);
	_SolverIterations = 20;
	_SolverMode = SOLVER_SIMD | SOLVER_USE_WARMSTARTING;
	_EnableSpu = true;
	_TimeStep = btScalar(1.0f)/btScalar(240.00f);
	_MaxSubSteps = 4;
	_FixedTimeStep = btScalar(1.0f)/btScalar(80.00f);
	_DynamicType = GameObject::STATIC;
	_CollisionType = BroadphaseNativeTypes::STATIC_PLANE_PROXYTYPE;
}

GameLevel2D::~GameLevel2D()
{
	// TODO Auto-generated destructor stub
}

void GameLevel2D::initialise()
{
	if(_Initialized)
	{
		return;
	}

	// initializing physics and collision
	_CollisionConfiguration = new btDefaultCollisionConfiguration();
	_BroadphaseInterface = new btDbvtBroadphase();
	_CollisionDispatcher = new btCollisionDispatcher(_CollisionConfiguration);
	_ConstraintSolver = new btSequentialImpulseConstraintSolver();
	_DynamicsWorld = new btDiscreteDynamicsWorld(_CollisionDispatcher,_BroadphaseInterface,
			_ConstraintSolver,_CollisionConfiguration);

	// setting dynamic simulation properties
	_DynamicsWorld->getSolverInfo().m_numIterations = _SolverIterations;
	_DynamicsWorld->getSolverInfo().m_solverMode = _SolverMode;
	_DynamicsWorld->getDispatchInfo().m_enableSPU = _EnableSpu;
	_DynamicsWorld->setGravity(_Gravity);

	// initialize collision object
	_CollisionShape = new btStaticPlaneShape(btVector3(0.0f,1.0f,0.0f),0.0f);
	//_MotionState = new GameMotionState(_Transform,_SceneNode); // not sure if null motion state can be used for static objects

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo consInfo(_Mass,_MotionState,_CollisionShape,_Inertia);
	_RigidBody = new btRigidBody(consInfo);

	// scene node
	_SceneNode = StateManager::getSingleton()->getSceneManager()->createSceneNode(_Name);
	// create plane here;

	// initializing objects
	for(std::map<std::string,GameObject*>::iterator i = _GameObjects.begin(); i != _GameObjects.end(); i++)
	{
		registerGameObject(i->second);
	}

	_Initialized = true;
}

void GameLevel2D::addGameObject(GameObject *obj)
{
	if(_GameObjects.count(obj->getName()) == 0)
	{
		_GameObjects.insert(std::make_pair(obj->getName(),obj));
		if(this->_Initialized)
		{
			registerGameObject(obj);
		}
	}
}

void GameLevel2D::removeGameObject(GameObject *obj)
{
	if(_GameObjects.count(obj->getName()) > 0)
	{
		if(obj->_Initialized)
		{
			unregisterGameObject(obj);
		}
		_GameObjects.erase(_GameObjects.find(obj->getName()));
	}
}

void GameLevel2D::registerGameObject(GameObject* obj)
{
	if(!obj->_Initialized)
	{
		obj->initialise();
	}

	if(obj->getSceneNode() != NULL)
	{
		_SceneNode->addChild(obj->getSceneNode());
	}

	if(obj->_RigidBody != NULL)
	{
		_DynamicsWorld->addRigidBody(obj->_RigidBody);
	}

	// enforcing 2d movement constrains
	obj->setLinearFactor(_LinearFactor);
	obj->setAngularFactor(_AngularFactor);
}

void GameLevel2D::unregisterGameObject(GameObject *obj)
{
	if(obj->getSceneNode() != NULL)
	{
		_SceneNode->removeChild(obj->getSceneNode());
	}

	if(obj->_RigidBody != NULL)
	{
		_DynamicsWorld->removeRigidBody(obj->_RigidBody);
	}
}
