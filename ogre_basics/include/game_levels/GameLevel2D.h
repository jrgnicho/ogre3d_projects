/*
 * GameLevel2D.h
 *
 *  Created on: Oct 14, 2012
 *      Author: coky
 */

#ifndef GAMELEVEL2D_H_
#define GAMELEVEL2D_H_

#include <game_levels/GameLevel.h>

class GameLevel2D: public GameLevel
{
public:
	GameLevel2D(btTransform &transform,std::string name = "");

	virtual ~GameLevel2D();

	virtual void initialise();

	virtual void addGameObject(GameObject::Ptr obj);

	virtual void removeGameObject(GameObject::Ptr obj);

protected:

	void registerGameObject(GameObject::Ptr obj);
	void unregisterGameObject(GameObject::Ptr obj);

private:

	static int _InstanceCount;

};

#endif /* GAMELEVEL2D_H_ */
