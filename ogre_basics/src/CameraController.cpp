/*
 * CameraController.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: coky
 */

#include <controllers/CameraController.h>
#include <OGRE/OgreTimer.h>
#include <state_management/StateManager.h>
#include <boost/foreach.hpp>

const std::string CAMERA_CTRL_CLASS_NAME = "Camera Controller";
const std::string POSITION_NODE_NAME = "PositionNode";
const std::string ROLL_NODE_NAME = "RollNode";
const std::string YAW_NODE_NAME = "YawNode";
const std::string CONTROLLER_NAME_TEMPLATE = "Controller";
std::vector<std::string> CameraController::_AssignedNames = std::vector<std::string>();


CameraController::CameraComponentIds::CameraComponentIds(std::string cameraName)
{
	if(!CameraController::checkIfNameAvailable(cameraName))
	{
		cameraName = CameraController::generateAutomaticName();
	}

	CameraName = cameraName;
	ControllerName = cameraName + CONTROLLER_NAME_TEMPLATE;
	ControllerNodeName = ControllerName + "Node";
	PositionNodeName = ControllerNodeName + "/" + POSITION_NODE_NAME;
	RollNodeName = ControllerNodeName + "/" + ROLL_NODE_NAME;
	YawNodeName = ControllerName + "/" + YAW_NODE_NAME;
}

CameraController::CameraComponentIds::CameraComponentIds()
{
	CameraName = CameraController::generateAutomaticName();
	ControllerName = CameraName + CONTROLLER_NAME_TEMPLATE;
	ControllerNodeName = ControllerName + "Node";
	PositionNodeName = ControllerNodeName + "/" + POSITION_NODE_NAME;
	RollNodeName = ControllerNodeName + "/" + ROLL_NODE_NAME;
	YawNodeName = ControllerName + "/" + YAW_NODE_NAME;
}

CameraController::CameraController()
:_PosNode(0),
 _RollNode(0),
 _YawNode(0),
 _Camera(0),
 _Pos(0.0f,0.0f,0.0f),
 _Yaw(Ogre::Real(0.0f)),
 _Roll(Ogre::Real(0.0f)),
 _CumulativeRoll(Ogre::Real(0.0f)),
 _CumulativeYaw(Ogre::Real(0.0f)),
 _MaxYaw(MAX_YAW),
 _MinYaw(MIN_YAW),
 _MaxRoll(MAX_ROLL),
 _MinRoll(MIN_ROLL),
 _MaxSpeed(MAX_SPEED),
 _MinSpeed(MIN_SPEED),
 _SpeedIncrement(SPEED_INCREMENT),
 _Speed(INITIAL_SPEED),
 _WindowAdjmFactor(WINDOW_ADJUSTMENT_FACTOR),
 _ScrollWheelAdjmFactor(SCROLL_WHEEL_ADJUSTMENT_FACTOR),
 _CamControllerNames(generateAutomaticName())
{
	// TODO Auto-generated constructor stub
}

CameraController::CameraController(std::string controllerName)
:_PosNode(0),
 _RollNode(0),
 _YawNode(0),
 _Camera(0),
 _Pos(0.0f,0.0f,0.0f),
 _Yaw(Ogre::Real(0.0f)),
 _Roll(Ogre::Real(0.0f)),
 _CumulativeRoll(Ogre::Real(0.0f)),
 _CumulativeYaw(Ogre::Real(0.0f)),
 _MaxYaw(MAX_YAW),
 _MinYaw(MIN_YAW),
 _MaxRoll(MAX_ROLL),
 _MinRoll(MIN_ROLL),
 _MaxSpeed(MAX_SPEED),
 _MinSpeed(MIN_SPEED),
 _SpeedIncrement(SPEED_INCREMENT),
 _Speed(INITIAL_SPEED),
 _WindowAdjmFactor(WINDOW_ADJUSTMENT_FACTOR),
 _ScrollWheelAdjmFactor(SCROLL_WHEEL_ADJUSTMENT_FACTOR),
_CamControllerNames(controllerName)
{

}

void CameraController::setup()
{
	if(_Camera == NULL)
	{
		// camera and viewport setup
		Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();
		Ogre::RenderWindow *renderWindow = StateManager::getSingleton()->getRenderWindow();
		Ogre::SceneNode *worldSceneNode = StateManager::getSingleton()->getParentSceneNode();
		Ogre::Viewport *viewport = NULL;

		// creating roll and yaw nodes
		Ogre::Matrix4 initialTransform = CAMERA_NODE_INITIAL_TRANSFORM;
		_PosNode = worldSceneNode->createChildSceneNode(_CamControllerNames.PositionNodeName,initialTransform.getTrans(),
				Ogre::Quaternion::IDENTITY);
		_RollNode = _PosNode->createChildSceneNode(_CamControllerNames.RollNodeName);
		_YawNode = _RollNode->createChildSceneNode(_CamControllerNames.YawNodeName);

		// setting up camera
		_Camera = sceneManager->createCamera(_CamControllerNames.CameraName);
		viewport = renderWindow->addViewport(_Camera);
		_Camera->setNearClipDistance(CAMERA_NEAR_CLIP_DISTANCE);
		_Camera->setFarClipDistance(CAMERA_FAR_CLIP_DISTANCE);
		_Camera->setAspectRatio(((float)viewport->getActualWidth())/((float)viewport->getActualHeight()));
		Ogre::Matrix4 camTransform = CAMERA_RELATIVE_TRANSFORM;
		_Camera->rotate(camTransform.extractQuaternion()); // rotation relative to parent node
		_Camera->move(camTransform.getTrans());

		// attaching camera to nodes
		_YawNode->attachObject(_Camera);

		std::cout<<CAMERA_CTRL_CLASS_NAME<<":\t"<<_CamControllerNames.CameraName<<" set"<<"\n";
	}
}

void CameraController::cleanup()
{
	if(_Camera != NULL)
	{
		Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();
		Ogre::RenderWindow *renderWindow = StateManager::getSingleton()->getRenderWindow();
		Ogre::SceneNode *worldSceneNode = StateManager::getSingleton()->getParentSceneNode();

		worldSceneNode->removeChild(_PosNode);
		_YawNode->detachObject(_Camera);

		renderWindow->removeViewport(_Camera->getViewport()->getZOrder());
		sceneManager->destroyCamera(_Camera);
		sceneManager->destroySceneNode(_YawNode); // this might throw a segmentation fault
		sceneManager->destroySceneNode(_RollNode);
		sceneManager->destroySceneNode(_PosNode);

		_PosNode = 0;
		_RollNode = 0;
		_YawNode = 0;
		_Camera = 0;

		std::cout<<CAMERA_CTRL_CLASS_NAME<<":\t"<<"Cleaned up camera components"<<"\n";
	}
}

CameraController::~CameraController() {
	// TODO Auto-generated destructor stub
	cleanup();
}

bool CameraController::processUnbufferedKeyInput(const Ogre::FrameEvent &evnt)
{
	using namespace Ogre;
	OIS::Keyboard *keyboard = StateManager::getSingleton()->getInputManager().getKeyboard();
	Ogre::Real timeElapsedInSecs = evnt.timeSinceLastEvent;

	if(keyboard->isKeyDown(OIS::KC_LSHIFT) || keyboard->isKeyDown(OIS::KC_RSHIFT))
	{
		if(keyboard->isKeyDown(OIS::KC_UP))
		{
			_Pos.z = _Speed.z*timeElapsedInSecs;
		}

		if(keyboard->isKeyDown(OIS::KC_DOWN))
		{
			_Pos.z = -_Speed.z*timeElapsedInSecs;
		}
	}

	if(keyboard->isKeyDown(OIS::KC_ADD))
	{
		_Speed += _SpeedIncrement;
	}

	if(keyboard->isKeyDown(OIS::KC_SUBTRACT))
	{
		_Speed -= _SpeedIncrement;
	}

	checkNextSpeedBounds();

	if(keyboard->isKeyDown(OIS::KC_RIGHT))
	{
		_Pos.y = -_Speed.y*timeElapsedInSecs;
	}

	if(keyboard->isKeyDown(OIS::KC_LEFT))
	{
		_Pos.y = _Speed.y*timeElapsedInSecs;
	}

	if(keyboard->isKeyDown(OIS::KC_UP))
	{
		_Pos.x = _Speed.x*timeElapsedInSecs;
	}

	if(keyboard->isKeyDown(OIS::KC_DOWN))
	{
		_Pos.x = -_Speed.x*timeElapsedInSecs;
	}

	return true;
}

bool CameraController::processUnbufferedMouseInput(const Ogre::FrameEvent &evnt)
{
	using namespace Ogre;

	const OIS::MouseState &ms = StateManager::getSingleton()->getInputManager().getMouse()->getMouseState();
	Ogre::Real timeElapsedInSecs = evnt.timeSinceLastEvent;

	// pitch and roll rotations
	if(ms.buttonDown(OIS::MB_Right))
	{
		_Roll = Radian(Degree(-ms.X.rel*_WindowAdjmFactor));
		_Yaw = Radian(Degree(ms.Y.rel*_WindowAdjmFactor));
	}

	// scroll wheel z translation
	if(ms.Z.rel != 0)
	{
		_Pos.z = _Speed.z*timeElapsedInSecs*(Real(ms.Z.rel)/_ScrollWheelAdjmFactor);

//		std::cout<<CAMERA_CTRL_CLASS_NAME<<
//				":\tupdated camera angle , yaw: "<<
//				_Yaw.valueDegrees()<<", roll: "<<
//				_Roll.valueDegrees()<<"\n"<<
//				"\t\tcamera pos, x: "<<_Pos.x<<
//				", y: "<<_Pos.y<<", z: "<<_Pos.z<<"\n"<<
//				"\t\tmouse rel z val : "<<ms.Z.rel<<"\n";

	}

	return true;
}

void CameraController::moveCamera()
{
	if(_PosNode != NULL)
	{
		// checking bounds
		checkNextMovementBounds();

		// performing all relative rotations
		_YawNode->yaw(_Yaw);
		_RollNode->roll(_Roll);

		// computing actual translation vector
		_Pos = (_RollNode->getOrientation()*_YawNode->getOrientation())*_Pos;
		_PosNode->translate(_Pos,Ogre::SceneNode::TS_LOCAL);

		// computing cumulative orientation and translation values
		_CumulativePosition += _Pos;
		_CumulativeRoll += _Roll;
		_CumulativeYaw += _Yaw;

		// resetting variables
		_Pos = Ogre::Vector3::ZERO;
		 _Roll = _Yaw =Ogre::Radian(0.0f);
	}
}

void CameraController::checkNextMovementBounds()
{
	if(_CumulativeRoll + _Roll > _MaxRoll )
	{
		_CumulativeRoll -= _MaxRoll;// equivalent to revolving 2*PI radians since _MaxRoll equals that value
	}

	if(_CumulativeRoll + _Roll < _MinRoll )
	{
		_CumulativeRoll += _MaxRoll;
	}

	// yaw part needs to be completed
	if(_Yaw > _MaxYaw)
	{
		_Yaw = _MaxYaw;
	}

	if(_Yaw < _MinYaw)
	{
		_Yaw = _MinYaw;
	}
}

void CameraController::checkNextSpeedBounds()
{
	if(_Speed > _MaxSpeed*Ogre::Vector3::UNIT_SCALE)
	{
		_Speed = _MaxSpeed;
	}

	if(_Speed < _MinSpeed*Ogre::Vector3::UNIT_SCALE)
	{
		_Speed = _MinSpeed;
	}
}

void CameraController::connectToScene()
{
	if(_PosNode != NULL && !_PosNode->isInSceneGraph())
	{
		Ogre::SceneNode *worldNode = StateManager::getSingleton()->getParentSceneNode();
		worldNode->addChild(_PosNode);
	}
}

void CameraController::disconnectFromScene()
{
	if(_PosNode != NULL && _PosNode->isInSceneGraph())
	{
		_PosNode->getParentSceneNode()->removeChild(_PosNode);
	}
}

Ogre::Matrix4 CameraController::getCameraTransform()
{
	using namespace Ogre;
	Matrix4 camTransform = Matrix4::IDENTITY;
	camTransform.makeTransform(_CumulativePosition,Ogre::Vector3::UNIT_SCALE,
			_RollNode->getOrientation()*_YawNode->getOrientation());

	return camTransform;
}

Ogre::Radian CameraController::getRoll()
{
	return _CumulativeRoll;
}

Ogre::Radian CameraController::getYaw()
{
	return _CumulativeYaw;
}

Ogre::Vector3 CameraController::getPosition()
{
	return _CumulativePosition;
}

Ogre::Camera* CameraController::getCamera()
{
	return _Camera;
}

std::string CameraController::generateAutomaticName()
{
	std::stringstream nameBuilder;
	nameBuilder << CAMERA_NAME <<_AssignedNames.size() + 1;

	return nameBuilder.str();
}

bool CameraController::checkIfNameAvailable(std::string name)
{
	for(std::vector<std::string>::iterator i = _AssignedNames.begin(); i != _AssignedNames.end(); i++)
	{
		if(name.compare(*i) == 0)
		{
			return false;
		}
	}

	return true;
}
