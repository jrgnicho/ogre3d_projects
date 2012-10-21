/*
 * GameObject.h
 *
 *  Created on: Oct 6, 2012
 *      Author: coky
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h> // collision types
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btTransform.h>
#include <boost/shared_ptr.hpp>
#include <utilities/OgreBulletConversions.h>

/*
 * Game Object Superclass interface
 */

class GameMotionState: public btMotionState
{

public:
	GameMotionState(Ogre::SceneNode *node = NULL)
	:_SceneNode(node)
	{
		_StartTransform.setOrigin(
				OgreBulletConversions::convertOgreVecToBulletVec(node->getPosition()));
		_StartTransform.setRotation(
				OgreBulletConversions::convertOgreQuatToBulletQuat( node->getOrientation()));
	}

	virtual ~GameMotionState()
	{

	}

	virtual void setWorldTransform(const btTransform &worldTransform)
	{
		if(_SceneNode == NULL)
		{
			return;
		}

		btQuaternion q = worldTransform.getRotation();
		const btVector3 &p = worldTransform.getOrigin();

		_SceneNode->setPosition(p.x(),p.y(),p.z());
		_SceneNode->setOrientation(q.w(),q.x(),q.y(),q.z());
	}

	virtual void getWorldTransform(btTransform &transform) const
	{
		if(_SceneNode == NULL)
		{
			transform = _StartTransform;
			return;
		}

		Ogre::Vector3 p = _SceneNode->getPosition();
		Ogre::Quaternion q = _SceneNode->getOrientation();

		transform.setOrigin(btVector3(p.x,p.y,p.z));
		transform.setRotation(btQuaternion(q.x,q.y,q.z,q.w));
	}

	void setSceneNode(Ogre::SceneNode *sceneNode)
	{
		_SceneNode = sceneNode;
	}

protected:

	Ogre::SceneNode* _SceneNode;
	btTransform _StartTransform;
};

class GameObject
{
public:
	enum DynamicType
	{
		NONE = 0,
		KINEMATIC = 1,
		STATIC = 2,
		DYNAMIC = 4
	};

	typedef boost::shared_ptr<GameObject> Ptr;
	typedef GameObject::Ptr GameObjectPtr;

public:

	GameObject(BroadphaseNativeTypes collisionType = INVALID_SHAPE_PROXYTYPE,
			DynamicType type = NONE,
			btScalar mass = 0.0f,
			btTransform transform = btTransform(),
			std::string name = "")
	:_DynamicType(type),
	 _CollisionType(collisionType),
	 _CollisionShape(),
	 _MotionState(),
	 _RigidBody(),
	 _Transform(transform),
	 _Mass(mass),
	 _Inertia(0.0f,0.0f,0.0f),
	 _LinearFactor(1,0,1), // no motion along y
	 _AngularFactor(0,1,0), // rotation about y only
	 _Name(name.empty() ? generateName(): name),
	 _SceneNode(NULL)
	{
		_ObjectCount++;
	}

	virtual ~GameObject()
	{
		finalize();
	}

	bool isInitialized()
	{
		return _Initialized;
	}

	virtual void initialise() = 0;
	virtual void finalize()
	{
//		if(StateManager::getSingleton()->getSceneManager() != NULL && _SceneNode != NULL)
//		{
//			StateManager::getSingleton()->getSceneManager()->destroySceneNode(_SceneNode);
//			_SceneNode = NULL;
//		}

//		if(_CollisionShape != NULL)
//		{
//			delete _CollisionShape;
//			_CollisionShape = NULL;
//		}
//
//		if(_RigidBody != NULL)
//		{
//			delete _RigidBody;
//			_RigidBody = NULL;
//		}
//
//		if(_MotionState != NULL)
//		{
//			delete _MotionState;
//			_MotionState = NULL;
//		}
	}

	Ogre::SceneNode* getSceneNode()
	{
		return _SceneNode;
	}

	GameMotionState* getMotionState()
	{
		return _MotionState.get();
	}

	btRigidBody* getRigidBody()
	{
		return _RigidBody.get();
	}

	virtual std::string getName()
	{
		return _Name;
	}

	virtual btTransform getTransform()
	{
		if(_MotionState != NULL)
		{
			_MotionState->getWorldTransform(_Transform);
		}

		return _Transform;
	}

	virtual void setTransform(btTransform &t)
	{
		if(_MotionState != NULL)
		{
			_MotionState->setWorldTransform(t);
		}
	}

	virtual void setMass(btScalar mass)
	{
		_Mass = mass;
	}

	virtual btScalar getMass()
	{
		return _Mass;
	}

	virtual void setLinearFactor(btVector3 &factor)
	{
		_LinearFactor = factor;
	}

	virtual const btVector3& getLinearFactor()
	{
		return _LinearFactor;
	}

	virtual void setAngularFactor(btVector3 &factor)
	{
		_AngularFactor = factor;
	}

	virtual const btVector3& getAngularFactor()
	{
		return _AngularFactor;
	}

	static std::string generateName()
	{
		std::stringstream name;
		name<<"GameObject"<<_ObjectCount;
		return name.str();
	}

protected:


	// general
	std::string _Name;
	btTransform _Transform;

	// visual components
	Ogre::SceneNode *_SceneNode;

	// dynamic components
	GameObject::DynamicType _DynamicType;
	BroadphaseNativeTypes _CollisionType;

	boost::shared_ptr<btRigidBody> _RigidBody;
	boost::shared_ptr<btCollisionShape> _CollisionShape;
	boost::shared_ptr<GameMotionState> _MotionState;
	btScalar _Mass;
	btVector3 _Inertia;
	btVector3 _LinearFactor; // determines allowed linear motion in axis, use (1,0,1) for 2D no y movement
	btVector3 _AngularFactor; // determines allowed angular motion in axis

	// object state
	bool _Initialized;

private:
	static int _ObjectCount;
};

int GameObject::_ObjectCount = 0;

#endif /* GAMEOBJECT_H_ */
