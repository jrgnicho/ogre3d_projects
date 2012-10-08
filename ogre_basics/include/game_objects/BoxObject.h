/*
 * BoxObject.h
 *
 *  Created on: Oct 7, 2012
 *      Author: coky
 */

#ifndef BOXOBJECT_H_
#define BOXOBJECT_H_
#include <game_objects/GameObject.h>

class BoxObject : public GameObject
{
public:
	BoxObject(btVector3 sides = btVector3(1,1,1),
			GameObject::DynamicType type = GameObject::KINEMATIC,
			btScalar mass = 0.0f);

	virtual ~BoxObject();

	virtual void initialise();

protected:
	btVector3 _BoxSides;


};

#endif /* BOXOBJECT_H_ */
