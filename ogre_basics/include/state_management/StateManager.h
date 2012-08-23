/*
 * StateManager.h
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_

#include <input_handling/InputManager.h>
#include <map>
#include <vector>
#include <utility>
#include <exception>
#include <OGRE/OgreMath.h>
#include <OGRE/OgreMatrix4.h>

namespace OgreDefaults
{
	const std::string PLUGINS_FILE = "plugins.cfg";
	const std::string CONFIGURATION_FILE = "ogre.cfg";
	const std::string LOG_FILE = "Ogre.log";
	const std::string RESOURCES_FILE = "resources.cfg";
	const std::string WINDOW_NAME = "Ogre Window";
	const std::string SCENE_MANAGER_NAME = "WorldScene";
	const Ogre::ColourValue SCENE_AMBIENT_LIGHT_COLOR = Ogre::ColourValue(1.0f,1.0f,1.0f);
	const Ogre::Vector3 SCENE_AMBIENT_LIGHT_POSITION = Ogre::Vector3(0.0f,0.0f,100.0f);
	const std::string SCENE_SKY_BOX_MATERIAL_NAME = "Examples/MorningSkyBox";
	const float SCENE_SKY_BOX_DISTANCE = 2000.0f;
	const std::string PARENT_NODE_NAME = "WorldNode";
	const Ogre::Matrix4 PARENT_NODE_TRANFORM = Ogre::Matrix4(
			1,0,0,0.0f,
			0,0,1,0.0f,
			0,-1,0,0.0f,
			0,0,0,1);

	const std::string CAMERA_NODE_NAME = "CameraNode";
	const std::string CAMERA_NAME = "WorldCamera";
	const Ogre::Matrix4 CAMERA_NODE_TRANSFORM = Ogre::Matrix4(
			1,0,0,0,
			0,1,0,500.0f,
			0,0,1,50.0f,
			0,0,0,1);
	const Ogre::Vector3 CAMERA_LOOK_AT_POINT = Ogre::Vector3(0.0f,0.0f,0.0f);
	const float CAMERA_NEAR_CLIP_DISTANCE = 5.0f;
	const float CAMERA_FAR_CLIP_DISTANCE = 5000.0f;

	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 480;
	const int MIPMAPS = 5;
}

struct OgreParameters
{
public:
	OgreParameters()
	:PluginsFile(OgreDefaults::PLUGINS_FILE),
	 ConfigFile(OgreDefaults::CONFIGURATION_FILE),
	 LogFile(OgreDefaults::LOG_FILE),
	 ResourcesFile(OgreDefaults::RESOURCES_FILE),
	 SceneManagerName(OgreDefaults::SCENE_MANAGER_NAME),
	 SceneAmbientLightColor(OgreDefaults::SCENE_AMBIENT_LIGHT_COLOR),
	 SceneSkyBoxMaterialName(OgreDefaults::SCENE_SKY_BOX_MATERIAL_NAME),
	 SceneSkyBoxDistance(OgreDefaults::SCENE_SKY_BOX_DISTANCE),
	 CameraName(OgreDefaults::CAMERA_NAME),
	 ParentNodeName(OgreDefaults::PARENT_NODE_NAME),
	 CameraNodeName(OgreDefaults::CAMERA_NODE_NAME),
	 ParentNodeTransform(OgreDefaults::PARENT_NODE_TRANFORM),
	 CameraNodeTransform(OgreDefaults::CAMERA_NODE_TRANSFORM),
	 CameraLookAtPoint(OgreDefaults::CAMERA_LOOK_AT_POINT),
	 CameraNearClipDistance(OgreDefaults::CAMERA_NEAR_CLIP_DISTANCE),
	 CameraFarClipDistance(OgreDefaults::CAMERA_FAR_CLIP_DISTANCE),
	 WindowName(OgreDefaults::WINDOW_NAME),
	 WindowHeight(OgreDefaults::WINDOW_HEIGHT),
	 WindowWidth(OgreDefaults::WINDOW_WIDTH),
	 Mipmaps(OgreDefaults::MIPMAPS)
	{

	}

	std::string PluginsFile;
	std::string ConfigFile;
	std::string LogFile;
	std::string ResourcesFile;
	std::string SceneManagerName;
	Ogre::ColourValue SceneAmbientLightColor;
	Ogre::Vector3 SceneAmbientLightPosition;
	std::string SceneSkyBoxMaterialName;
	float SceneSkyBoxDistance;
	std::string CameraName;
	std::string ParentNodeName;
	std::string CameraNodeName;
	Ogre::Matrix4 ParentNodeTransform;
	Ogre::Matrix4 CameraNodeTransform;
	Ogre::Vector3 CameraLookAtPoint;
	float CameraNearClipDistance;
	float CameraFarClipDistance;
	std::string WindowName;
	int WindowHeight;
	int WindowWidth;
	int Mipmaps;

};

class StateInterface;
class StateManager : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
	class InitializationException: public std::exception
	{
	public:
		InitializationException(std::string msg = "")
		:_Msg(msg)
		{

		}

		virtual ~InitializationException() throw ()
		{

		}

		virtual const char* what() const throw()
		{
			return _Msg.c_str();
		}

	protected:
		std::string _Msg;

	};

public:

	virtual ~StateManager();

	// state management methods
	void start(StateInterface *state);
	void changeState(StateInterface *state);
	bool pushState(StateInterface *state);
	void popState();
	void shutdown();

	//
	void setup();
	void cleanup();

	void manageState(StateInterface* state);

	static StateManager* getSingleton();
	static void destroySingleton();

	// device access
	const InputManager& getInputManager();

	// ogre methods
	bool frameStarted(const Ogre::FrameEvent &evnt);
	bool frameEnded(const Ogre::FrameEvent &evnt);

	// window callbacks
	void windowClosed(Ogre::RenderWindow *window);
	void windowResized(Ogre::RenderWindow *window);

	// ogre methods
	Ogre::SceneManager* getSceneManager();
	void addChildSceneNode(Ogre::SceneNode *node);
	void removeChildSceneNode(Ogre::SceneNode *node);
	Ogre::SceneNode* getParentSceneNode();
	Ogre::Matrix4 getParentTransform();

protected:
	StateManager();

	// Event handling
	void addEventHandler(StateInterface *state);
	void removeEventHandler(StateInterface *state);

	//setup methods
	//void loadResources() throw (InitializationException);
	// selects the rendenrer system and instantiates a window
	void setupRenderSystem() throw (InitializationException);
	void setupResources() throw (InitializationException);
	void setupScene() throw (InitializationException);

	// state management
	static StateManager *_Instance;
	InputManager _InputManager;
	std::map<std::string,StateInterface*> _ManagedStates;
	std::vector<StateInterface*> _StateStack;
	bool _ShutdownIssued;

	// ogre stuff
	Ogre::Root *_OgreRoot;
	Ogre::SceneManager *_SceneManager;
	Ogre::Viewport *_Viewport;
	Ogre::RenderWindow *_RenderWindow;
	Ogre::Camera *_Camera;
	std::string _ParentDirectory;
	OgreParameters _Parameters;

	// ogre scene
	Ogre::SceneNode *_ParentSceneNode;
	Ogre::SceneNode *_CameraSceneNode;
};

#endif /* STATEMANAGER_H_ */
