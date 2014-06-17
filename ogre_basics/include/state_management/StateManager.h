/*
 * StateManager.h
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_

#include <input_handling/InputManager.h>
#include <utilities/General.h>
#include <map>
#include <vector>
#include <utility>
#include <exception>
#include <OGRE/OgreMath.h>
#include <OGRE/OgreMatrix4.h>


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

	static StateManager* init(const application_parameters::OgreParameters& parameters);
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

	application_parameters::OgreParameters parameters_;
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
