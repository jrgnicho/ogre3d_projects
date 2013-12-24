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
	const std::string RESOURCES_FILE = "resources_local.cfg";
	const std::string WINDOW_NAME = "Ogre Window";
	const std::string SCENE_MANAGER_NAME = "WorldScene";
	const Ogre::ColourValue SCENE_AMBIENT_LIGHT_COLOR = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
	const Ogre::Vector3 SCENE_AMBIENT_LIGHT_POSITION = Ogre::Vector3(400,400,0);
	const std::string SCENE_SKY_BOX_MATERIAL_NAME = "Examples/MorningSkyBox";
	const float SCENE_SKY_BOX_DISTANCE = 2000.0f;
	const std::string PARENT_NODE_NAME = "WorldNode";
	const Ogre::Matrix4 PARENT_NODE_TRANFORM = Ogre::Matrix4(
			1,0 ,0,0.0f,
			0,0 ,1,0.0f,
			0,-1,0,0.0f,
			0,0 ,0,1);

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
	 ParentDirectory(""),
	 SceneManagerName(OgreDefaults::SCENE_MANAGER_NAME),
	 SceneAmbientLightColor(OgreDefaults::SCENE_AMBIENT_LIGHT_COLOR),
	 SceneSkyBoxMaterialName(OgreDefaults::SCENE_SKY_BOX_MATERIAL_NAME),
	 SceneSkyBoxDistance(OgreDefaults::SCENE_SKY_BOX_DISTANCE),
	 ParentNodeName(OgreDefaults::PARENT_NODE_NAME),
	 ParentNodeTransform(OgreDefaults::PARENT_NODE_TRANFORM),
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
	std::string ParentDirectory;
	std::string SceneManagerName;
	Ogre::ColourValue SceneAmbientLightColor;
	Ogre::Vector3 SceneAmbientLightPosition;
	std::string SceneSkyBoxMaterialName;
	float SceneSkyBoxDistance;
	std::string ParentNodeName;
	Ogre::Matrix4 ParentNodeTransform;
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


	void manageState(StateInterface* state);

	static StateManager* init(const OgreParameters& parameters);
	static StateManager* getSingleton();
	static void destroySingleton();

	// device access
	InputManager& getInputManager();

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
	Ogre::RenderWindow* getRenderWindow();

protected:
	StateManager();

	//
	void setup();
	void cleanup();


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
	static StateManager *instance_;

	OgreParameters parameters_;
	InputManager input_manager_;
	std::map<std::string,StateInterface*> managed_states_;
	std::vector<StateInterface*> state_stack_;
	bool shutdown_issued_;

	// ogre stuff
	Ogre::Root *ogre_root_;
	Ogre::SceneManager *scene_manager_;
	Ogre::Viewport *viewport_;
	Ogre::RenderWindow *render_window_;

	// ogre scene
	Ogre::SceneNode *parent_scene_node_;
};

#endif /* STATEMANAGER_H_ */
