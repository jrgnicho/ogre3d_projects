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
#include <input_handling/InputEventHandler.h>

const Ogre::Real WINDOW_ADJUSTMENT_FACTOR = Ogre::Real(0.13f);
const Ogre::Real SCROLL_WHEEL_ADJUSTMENT_FACTOR = Ogre::Real(200.0f);
const Ogre::Real SPEED_INCREMENT = Ogre::Real(2.0f);
const Ogre::Real MAX_SPEED = 100*SPEED_INCREMENT;
const Ogre::Real MIN_SPEED = 1.0f*SPEED_INCREMENT;
const Ogre::Real INITIAL_SPEED = 12*SPEED_INCREMENT;
const Ogre::Radian MAX_YAW = Ogre::Radian(M_PI_2);
const Ogre::Radian MIN_YAW = Ogre::Radian(-M_PI_2);
const Ogre::Radian MAX_ROLL = Ogre::Radian(2*M_PI_2);
const Ogre::Radian MIN_ROLL = Ogre::Radian(0);
const Ogre::Matrix4 CAMERA_RELATIVE_TRANSFORM = Ogre::Matrix4(Ogre::Quaternion(Ogre::Radian(-M_PI_2),Ogre::Vector3::UNIT_Z)*
		Ogre::Quaternion(Ogre::Radian(M_PI_2),Ogre::Vector3::UNIT_X));
const Ogre::Matrix4 CAMERA_NODE_INITIAL_TRANSFORM = Ogre::Matrix4(
		1, 0, 0, 10,
		0, 1, 0, 0,
		0, 0, 1, 10,
		0, 0, 0, 1);
const std::string CAMERA_NAME = "Camera";
const float CAMERA_NEAR_CLIP_DISTANCE = 0.2f;
const float CAMERA_FAR_CLIP_DISTANCE = 5000.0f;

class CameraController: public InputEventHandler
{
	struct CameraComponentIds
	{
		CameraComponentIds();
		CameraComponentIds(std::string cameraName);

		std::string CameraName;
		std::string ControllerName;
		std::string PositionNodeName;
		std::string RollNodeName;
		std::string YawNodeName;
		std::string ControllerNodeName;
	};

public:
	CameraController();
	CameraController(std::string cameraName);
	virtual ~CameraController();

	// setup and cleanup
	void setup();
	void cleanup();
	void connectToScene();
	void disconnectFromScene();

	/* spatial configuration parameters.  These values are relative to their own local space defined by the corresponding
	 * parent scene node.
	*/
	Ogre::Radian getRoll();
	Ogre::Radian getYaw();
	Ogre::Vector3 getPosition();
	void setPosition(const Ogre::Vector3& pos);

	//void applyMouseState(const OIS::MouseState &ms,Ogre::Real timeElapsedInSecs);
	//void applyKeyboardState(const OIS::Keyboard *keyboard,Ogre::Real timeElapsedInSecs);
	void moveCamera();
	Ogre::Camera* getCamera();
	Ogre::Matrix4 getCameraTransform();

	// overloaded input handling methods
	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent &evnt);
	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent &evnt);

protected:

	// movement bound checking methods
	void checkNextMovementBounds();
	void checkNextSpeedBounds();

	// ids
	CameraComponentIds _CamControllerNames;

	static std::string generateAutomaticName();
	static bool checkIfNameAvailable(std::string);
	static std::vector<std::string> _AssignedNames;

	// the roll node is set as the parent of the yaw node
	Ogre::SceneNode *_PosNode;
	Ogre::SceneNode *_RollNode;
	Ogre::SceneNode *_YawNode;
	Ogre::Camera *_Camera;
	Ogre::Viewport *_Viewport;

	// motion variables:  All these variables are applied relative to the last movement
	Ogre::Vector3 _Pos; //
	Ogre::Radian _Yaw; // rotation about world +y,
	Ogre::Radian _Roll; // rotation about local +z,
	Ogre::Real _WindowAdjmFactor; // Adjustment factor applied to x,y locations of the mouse in the display window;
	Ogre::Real _ScrollWheelAdjmFactor;

	// motion thresholds:  Should prevent from running into awkward poses on the camera
	Ogre::Vector3 _CumulativePosition;
	Ogre::Radian _CumulativeRoll;
	Ogre::Radian _CumulativeYaw;
	Ogre::Radian _MaxYaw, _MinYaw;
	Ogre::Radian _MaxRoll, _MinRoll;

	// translation rates
	Ogre::Vector3 _Speed;
	Ogre::Real _MaxSpeed, _MinSpeed;
	Ogre::Real _SpeedIncrement;
};

#endif /* CAMERACONTROLLER_H_ */
