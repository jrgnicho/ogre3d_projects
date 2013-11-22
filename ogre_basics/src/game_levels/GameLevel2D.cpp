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
#include <state_management/StateManager.h>

GameLevel2D::GameLevel2D(btTransform &trans,std::string name)
:GameLevel(trans,name)
{
	// TODO Auto-generated constructor stub
	_LinearFactor = btVector3(1.0f,1.0f,0.0f); // limits motion to x-y plane only no motion in or out of the screen
	_Gravity = btVector3(0.0f,-9.81f,0.0f);
	_SolverIterations = 20;
	_SolverMode = SOLVER_SIMD | SOLVER_USE_WARMSTARTING;
	_EnableSpu = true;
	//_TimeStep = btScalar(1.0f)/btScalar(240.00f);
	_MaxSubSteps = 4;
	_FixedTimeStep = btScalar(1.0f)/btScalar(80.00f);
	_DynamicType = GameObject::STATIC;
	_CollisionType = STATIC_PLANE_PROXYTYPE;
	 _Gravity = btVector3(0.0f,-9.81f,0.0f);
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
	_CollisionConfiguration = boost::shared_ptr<btCollisionConfiguration>(
			new btDefaultCollisionConfiguration());
	_BroadphaseInterface = boost::shared_ptr<btBroadphaseInterface>(new btDbvtBroadphase());
	_CollisionDispatcher = boost::shared_ptr<btCollisionDispatcher>(new btCollisionDispatcher(_CollisionConfiguration.get()));
	_ConstraintSolver = boost::shared_ptr<btConstraintSolver>(new btSequentialImpulseConstraintSolver());
	_DynamicsWorld = boost::shared_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(_CollisionDispatcher.get(),
			_BroadphaseInterface.get(),
			_ConstraintSolver.get(),
			_CollisionConfiguration.get()));

	// setting dynamic simulation properties
	_DynamicsWorld->getSolverInfo().m_numIterations = _SolverIterations;
	_DynamicsWorld->getSolverInfo().m_solverMode = _SolverMode;
	_DynamicsWorld->getDispatchInfo().m_enableSPU = _EnableSpu;
	_DynamicsWorld->setGravity(_Gravity);

	// initialize collision object
	_CollisionShape = boost::shared_ptr<btCollisionShape>(new btStaticPlaneShape(btVector3(0.0f,1.0f,0.0f),0.0f));
	//_MotionState = new GameMotionState(_Transform,_SceneNode); // not sure if null motion state can be used for static objects

	// rigid body
	btRigidBody::btRigidBodyConstructionInfo consInfo(_Mass,NULL,_CollisionShape.get(),_Inertia);
	_RigidBody = boost::shared_ptr<btRigidBody>(new btRigidBody(consInfo));

	// scene node
	_SceneNode = StateManager::getSingleton()->getParentSceneNode()->createChildSceneNode(_Name,
			OgreBulletConversions::convertBulletVec3ToOgreVec3(_Transform.getOrigin()),
			OgreBulletConversions::convertBulletQuatToOgreQuat(_Transform.getRotation()));
	// create plane here;

	// initializing objects
	for(std::map<std::string,GameObject::Ptr>::iterator i = _GameObjects.begin(); i != _GameObjects.end(); i++)
	{
		registerGameObject(i->second);
	}

	_Initialized = true;
}

void GameLevel2D::addGameObject(GameObject::Ptr obj)
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

void GameLevel2D::removeGameObject(GameObject::Ptr obj)
{
	if(_GameObjects.count(obj->getName()) > 0)
	{
		if(obj->isInitialized())
		{
			unregisterGameObject(obj);
		}
		_GameObjects.erase(_GameObjects.find(obj->getName()));
	}
}

void GameLevel2D::registerGameObject(GameObject::Ptr obj)
{
	if(!obj->isInitialized())
	{
		obj->initialise();
	}

	if(obj->getSceneNode() != NULL)
	{
		_SceneNode->addChild(obj->getSceneNode());
	}

	if(obj->getRigidBody() != NULL)
	{
		_DynamicsWorld->addRigidBody(obj->getRigidBody());
	}

	// enforcing 2d movement constrains
	obj->setLinearFactor(_LinearFactor);
	obj->setAngularFactor(_AngularFactor);
}

void GameLevel2D::unregisterGameObject(GameObject::Ptr obj)
{
	if(obj->getSceneNode() != NULL)
	{
		_SceneNode->removeChild(obj->getSceneNode());
	}

	if(obj->getRigidBody() != NULL)
	{
		_DynamicsWorld->removeRigidBody(obj->getRigidBody());
	}
}
