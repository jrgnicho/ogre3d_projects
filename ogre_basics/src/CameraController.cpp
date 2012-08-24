/*
 * CameraController.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: coky
 */

#include <controllers/CameraController.h>
#include <OGRE/OgreTimer.h>

CameraController::CameraController()
:_SceneNode(0),
 _Camera(0),
 _Pos(0.0f,0.0f,10.0f),
 _Yaw(Ogre::Real(0.0f)),
 _Roll(Ogre::Real(0.0f)),
 _MaxYaw(MAX_YAW),
 _MinYaw(MIN_YAW),
 _MaxRoll(MAX_ROLL),
 _MinRoll(MIN_ROLL)
{
	// TODO Auto-generated constructor stub

}

CameraController::~CameraController() {
	// TODO Auto-generated destructor stub
}

void CameraController::setCamera(Ogre::Camera *camera)
{
	_Camera = camera;
}

void CameraController::setCameraSceneNode(Ogre::SceneNode *cameraNode)
{
	_SceneNode = cameraNode;
}

void CameraController::applyKeyboardState(const OIS::Keyboard *keyboard,unsigned int timeElapsedInMili)
{

}

void CameraController::applyMouseState(const OIS::MouseState &ms,unsigned int timeElapsedInMili)
{
	using namespace Ogre;

	Real timeElapsed = ((Real)timeElapsedInMili)/(Real(1000.f));

	// pitch and roll rotations
	if(ms.buttonDown(OIS::MB_Right))
	{
		_Yaw += Radian(Degree(ms.X.rel*_WindowAdjmFactor));
		_Roll += Radian(Degree(ms.Y.rel*_WindowAdjmFactor));
	}

	// scroll wheel z translation
	if(ms.Z.rel != 0)
	{
		_Pos.z += _Speed.z*timeElapsed*(Real(ms.Z.rel)/_ScrollWheelAdjmFactor);
	}
}

void CameraController::moveCamera()
{
	Ogre::Matrix4 transform = getCameraTransform();
	_SceneNode->rotate(transform.extractQuaternion(),Ogre::SceneNode::TS_PARENT);
	_SceneNode->translate(transform.getTrans(),Ogre::SceneNode::TS_PARENT);
}

Ogre::Matrix4 CameraController::getCameraTransform()
{
	using namespace Ogre;
	Matrix4 posMat, yawMat, pitchMat, totalMat = Matrix4::IDENTITY;

	// computing world position transform
	posMat.setTrans(_Pos);

	yawMat.makeTransform(Vector3::ZERO,Vector3::UNIT_SCALE,Quaternion(_Yaw,Vector3::UNIT_Z));
	pitchMat.makeTransform(Vector3::ZERO,Vector3::UNIT_SCALE,Quaternion(_Roll,Vector3::UNIT_X));

	totalMat = posMat * yawMat * pitchMat;

	return totalMat;
}

Ogre::SceneNode* CameraController::getCameraSceneNode()
{
	return _SceneNode;
}

Ogre::Camera* CameraController::getCamera()
{
	return _Camera;
}
