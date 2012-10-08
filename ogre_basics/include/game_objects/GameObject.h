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

/*
 * Game Object Superclass interface
 */
int GameObject::_ObjectCount = 0;

class GameMotionState: public btMotionState
{

public:
	GameMotionState(const btTransform startTrans = btTransform(),Ogre::SceneNode *node = NULL)
	:_StartTransform(startTrans),
	 _SceneNode(node)
	{

	}

	~GameMotionState()
	{

	}

	virtual void setWorldTransform(const btTransform &worldTransform)
	{
		if(_SceneNode == NULL)
		{
			return;
		}

		btQuaternion q = worldTransform.getRotation();
		btVector3 p = worldTransform.getOrigin();

		_SceneNode->setPosition(p.x(),p.y(),p.z());
		_SceneNode->setOrientation(q.w(),q.x(),q.y(),q.z());
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

public:

	GameObject(DynamicType type = NONE,btScalar mass = 0.0f)
	:_DynamicType(type),
	 _CollisionType(INVALID_SHAPE_PROXYTYPE),
	 _CollisionShape(NULL),
	 _MotionState(NULL),
	 _RigidBody(NULL),
	 _Mass(mass),
	 _Inertia(0.0f,0.0f,0.0f),
	 _LinearFactor(1,0,1), // no motion along y
	 _AngularFactor(0,1,0), // rotation about y only
	 _Name(generateName()),
	 _SceneNode(NULL)
	{
		_ObjectCount++;
	}

	virtual ~GameObject()
	{

	}

	virtual void initialise();

	Ogre::SceneNode* getSceneNode()
	{
		return _SceneNode;
	}

	GameMotionState* getMotionState()
	{
		return _MotionState;
	}

	std::string getName()
	{
		return _Name;
	}

	void setName(std::string name)
	{
		_Name = name;
	}

	static std::string generateName()
	{
		std::stringstream name;
		name<<"Object"<<_ObjectCount;
		return name.str();
	}

protected:

	static int _ObjectCount;

	// general
	std::string _Name;

	// visual components
	Ogre::SceneNode *_SceneNode

	// dynamic components
	GameObject::DynamicType _DynamicType;
	BroadphaseNativeTypes _CollisionType

	btRigidBody* _RigidBody;
	btCollisionShape* _CollisionShape;
	btScalar _Mass;
	btVector3 _Inertia;
	btVector3 _LinearFactor; // determines allowed linear motion in axis, use (1,0,1) for 2D no y movement
	btVector3 _AngularFactor; // determines allowed angular motion in axis
	GameMotionState* _MotionState;

};

#endif /* GAMEOBJECT_H_ */
