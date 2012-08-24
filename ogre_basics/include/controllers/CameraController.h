/*
 * CameraController.h
 *
 *  Created on: Aug 23, 2012
 *      Author: coky
 */

#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreMath.h>
#include <cmath>

const Ogre::Real WINDOW_ADJUSTMENT_FACTOR = Ogre::Real(0.13f);
const Ogre::Real SCROLL_WHEEL_ADJUSTMENT_FACTOR = Ogre::Real(120.0f);
const Ogre::Real SPEED_INCREMENT = Ogre::Real(1.0f);
const Ogre::Real MAX_SPEED = 10*SPEED_INCREMENT;
const Ogre::Real MIN_SPEED = 0.0f;
const Ogre::Radian MAX_YAW = Ogre::Radian(Ogre::Real(0.0f));
const Ogre::Radian MIN_YAW = Ogre::Radian(2*M_PI);
const Ogre::Radian MAX_ROLL = Ogre::Radian(M_PI_2);
const Ogre::Radian MIN_ROLL = Ogre::Radian(-M_PI_2);


class CameraController {
public:
	CameraController();
	virtual ~CameraController();

	void applyMouseState(const OIS::MouseState &ms,unsigned int timeElapsedInMili);
	void applyKeyboardState(const OIS::Keyboard *keyboard,unsigned int timeElapsedInMili);
	void moveCamera();
	void setCameraSceneNode(Ogre::SceneNode *cameraNode);
	void setCamera(Ogre::Camera *camera);
	Ogre::SceneNode* getCameraSceneNode();
	Ogre::Camera* getCamera();
	Ogre::Matrix4 getCameraTransform();

protected:

	Ogre::SceneNode *_SceneNode;
	Ogre::Camera *_Camera;

	// motion variables:  All these variables are applied relative to the last movement
	Ogre::Vector3 _Pos; //
	Ogre::Radian _Yaw; // rotation about world +z, this transform will be applied before the yaw transform
	Ogre::Radian _Roll; // rotation about local +x,
	Ogre::Real _WindowAdjmFactor; // Adjustment factor applied to x,y locations of the mouse in the display window;
	Ogre::Real _ScrollWheelAdjmFactor;

	// motion thresholds:  Should prevent from running into awkward poses on the camera
	Ogre::Radian _MaxYaw, _MinYaw;
	Ogre::Radian _MaxRoll, _MinRoll;

	// translation rates
	Ogre::Vector3 _Speed;
	Ogre::Real _MaxSpeed, MinSpeed;
	Ogre::Real _SpeedIncrement;

	// timer: Useful for providing time values to compute translations
	//Ogre::Timer _Timer;

};

#endif /* CAMERACONTROLLER_H_ */
