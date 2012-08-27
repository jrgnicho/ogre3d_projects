/*
 * StateManager.cpp
 *
 *  Created on: Aug 15, 2012
 *      Author: coky
 */

#include <state_management/StateManager.h>
#include <state_types/StateInterface.h>
#include <boost/foreach.hpp>
#include <map>
#include <utility>

StateManager* StateManager::_Instance = NULL;

StateManager::StateManager()
:_OgreRoot(0),
 _SceneManager(0),
 _RenderWindow(0),
 _ParentSceneNode(0),
 _ShutdownIssued(false),
 _Parameters()
{
	// TODO Auto-generated constructor stub

}

StateManager::~StateManager() {
	// TODO Auto-generated destructor stub
}

void StateManager::setup()
{
	//_Parameters = OgreParameters();
	_ParentDirectory = ""; // use boost or ros package to find the application directory;
	std::stringstream logStream;

	if(_OgreRoot==NULL)
	{
		// initializing root
		std::cout<<"Creating Ogre Root Instance"<<"\n";
		std::string dir = _ParentDirectory;
		_OgreRoot = new Ogre::Root(dir + _Parameters.PluginsFile,
				dir + _Parameters.ConfigFile,dir + _Parameters.LogFile);

		logStream<<"\n ---------------------------------------------------------------------";
		logStream<<"\n ------------------ Created Ogre Root Instance  ----------------------";
		Ogre::LogManager::getSingleton().logMessage(Ogre::String(logStream.str()));logStream.str("");
		// calling setup methods
		try
		{
			setupRenderSystem();
			setupResources();
			setupScene();
		}
		catch(StateManager::InitializationException e)
		{
			logStream<<"Initialization fault: "<<e.what();
			Ogre::LogManager::getSingleton().logMessage(Ogre::String(logStream.str()));
			std::cout<<logStream.str()<<"\n";
			shutdown();
			return;
		}

		_OgreRoot->addFrameListener(this);
		Ogre::WindowEventUtilities::addWindowEventListener(_RenderWindow,this);

		std::size_t windowHandle;
		std::stringstream ss;
		_RenderWindow->getCustomAttribute("WINDOW",&windowHandle);
		ss<<windowHandle;
		_InputManager = InputManager(ss.str()); // needs to pass the right window handle string from the window instance
		_InputManager.setup();

	}

}

void StateManager::setupRenderSystem() throw (StateManager::InitializationException)
{
	Ogre::Root *root = _OgreRoot;
	if(!root->restoreConfig() && !root->showConfigDialog())
	{
		throw InitializationException("Render System was not selected");
	}
	root->initialise(false);
	_RenderWindow = root->createRenderWindow(_Parameters.WindowName,
			_Parameters.WindowWidth,_Parameters.WindowHeight,false);
}

void StateManager::setupResources() throw (StateManager::InitializationException)
{
	typedef std::string Str;
	Str sectionName, typeName, archiveName;
	Str dir = _ParentDirectory + "";
	Ogre::ConfigFile configFile;
	configFile.load(dir + _Parameters.ResourcesFile);
	Ogre::ConfigFile::SectionIterator iter = configFile.getSectionIterator();
	while(iter.hasMoreElements())
	{
		sectionName = iter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap& settings = *(iter.getNext());

		for(Ogre::ConfigFile::SettingsMultiMap::iterator i = settings.begin(); i != settings.end(); i++)
		{
			std::pair<Ogre::String,Ogre::String> item = *i;
			typeName = item.first;
			archiveName = item.second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archiveName,typeName,sectionName);
			//Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(sectionName);
		}
	}

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(_Parameters.Mipmaps);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void StateManager::setupScene() throw (StateManager::InitializationException)
{
	// scene setp
	_SceneManager = _OgreRoot->createSceneManager(Ogre::ST_GENERIC,_Parameters.SceneManagerName);
	_SceneManager->setAmbientLight(_Parameters.SceneAmbientLightColor);
	_SceneManager->createLight()->setPosition(_Parameters.SceneAmbientLightPosition);
//	_SceneManager->setSkyBox(true,_Parameters.SceneSkyBoxMaterialName,_Parameters.SceneSkyBoxDistance,
//			true,_Parameters.ParentNodeTransform.extractQuaternion());
	_SceneManager->setSkyBox(true,_Parameters.SceneSkyBoxMaterialName,_Parameters.SceneSkyBoxDistance,
			true,Ogre::Quaternion::IDENTITY);

	// creating nodes
	_ParentSceneNode = _SceneManager->getRootSceneNode()->createChildSceneNode(_Parameters.ParentNodeName,
			_Parameters.ParentNodeTransform.getTrans(),_Parameters.ParentNodeTransform.extractQuaternion());

}

/*
 * the cleanup method must dispose of objects and resources in a very specific sequence.  Otherwise a
 * segmentation fault or similar errors will be produced during the cleanup. Always use the sequence shown below.
 */
void StateManager::cleanup()
{
	// destroying all states
	std::cout<<"State Manager: cleanup started"<<"\n";
	while(!_ManagedStates.empty())
	{
		StateInterface *state = _ManagedStates.begin()->second;
		state->cleanup();
		delete state;
		_ManagedStates.erase(_ManagedStates.begin());
	}

	// cleaning up ogre components
	std::cout<<"State Manager:\t-destroying the ogre scene manager"<<"\n";
	_OgreRoot->destroySceneManager(_SceneManager);

	std::cout<<"State Manager:\t-removing and destroying input devices"<<"\n";
	_InputManager.cleanup();

	// removing listeners
	_OgreRoot->removeFrameListener(this);
	if(_RenderWindow != NULL)
	{
		std::cout<<"State Manager:\t-removing window listener and destroying window"<<"\n";
		Ogre::WindowEventUtilities::removeWindowEventListener(_RenderWindow,this);
		_RenderWindow->destroy();
		_RenderWindow = NULL;
	}

	std::cout<<"State Manager:\t-destroying ogre root instance"<<"\n";
	delete _OgreRoot;

	_SceneManager = 0;
	_RenderWindow = 0;
	_OgreRoot = 0;

	std::cout<<"State Manager: cleanup finished"<<"\n";
}

void StateManager::start(StateInterface *state)
{
	changeState(state);

	if(_RenderWindow != NULL)
	{
		if(_RenderWindow->isClosed())
		{
			_RenderWindow->destroy();
			_RenderWindow = _OgreRoot->createRenderWindow(_Parameters.WindowName,
					_Parameters.WindowWidth,_Parameters.WindowHeight,false);
			Ogre::WindowEventUtilities::addWindowEventListener(_RenderWindow,this);
		}
	}
	else
	{
		_RenderWindow = _OgreRoot->createRenderWindow(_Parameters.WindowName,
					_Parameters.WindowWidth,_Parameters.WindowHeight,false);
		Ogre::WindowEventUtilities::addWindowEventListener(_RenderWindow,this);
	}
	_OgreRoot->startRendering();
}

void StateManager::changeState(StateInterface *state)
{
	// remove and exit current state
	if(!_StateStack.empty())
	{
		removeEventHandler(_StateStack.back());
		_StateStack.back()->exit();
		_StateStack.pop_back();
	}

	// add and start state
	_StateStack.push_back(state);
	addEventHandler(state);
	_StateStack.back()->enter();
}

bool StateManager::pushState(StateInterface *state)
{
	if(!_StateStack.empty())
	{
		if(!_StateStack.back()->pause())
		{
			return false;
		}
		removeEventHandler(_StateStack.back());
	}

	_StateStack.push_back(state);
	addEventHandler(state);
	_StateStack.back()->enter();

	return true;
}

void StateManager::popState()
{
	if(!_StateStack.empty())
	{
		removeEventHandler(_StateStack.back());
		_StateStack.back()->exit();
		_StateStack.pop_back();
	}

	if(!_StateStack.empty())
	{
		addEventHandler(_StateStack.back());
		_StateStack.back()->resume();
	}
	else
	{
		shutdown();
	}
}

void StateManager::shutdown()
{
	_ShutdownIssued = true;
}

void StateManager::manageState(StateInterface *state)
{
	std::cout<<"State Manager: Registered " + state->getStateName() + "state as a managed state"<<"\n";
	_ManagedStates.insert(std::make_pair(state->getStateName(),state));
}

InputManager& StateManager::getInputManager()
{
	return _InputManager;
}

bool StateManager::frameStarted(const Ogre::FrameEvent &evnt)
{
	if(_ShutdownIssued)
	{
		removeEventHandler(_StateStack.back());
		while(!_StateStack.empty())
		{
			_StateStack.back()->exit();
			_StateStack.pop_back();
		}
		return false;
	}
	else
	{
		_InputManager.getKeyboard()->capture();
		_InputManager.getMouse()->capture();
	}

	return true;
}

bool StateManager::frameEnded(const Ogre::FrameEvent &evnt)
{
	return true;
}

void StateManager::windowResized(Ogre::RenderWindow *window)
{
	unsigned int width, height, depth;
	int left, top;

	// obtaining metrics
	window->getMetrics(width,height,depth,left,top);

	// updating mouse state
	_InputManager.getMouse()->getMouseState().width = width;
	_InputManager.getMouse()->getMouseState().height = height;

	for(int i = 0; i < window->getNumViewports(); i++)
	{
		Ogre::Viewport *viewport = window->getViewport(i);
		Ogre::Camera *camera = window->getViewport(i)->getCamera();
		float aspectRatio = ((float)viewport->getActualWidth())/((float)viewport->getActualHeight());
		camera->setAspectRatio(aspectRatio);
	}
}

void StateManager::windowClosed(Ogre::RenderWindow *window)
{
	if(_RenderWindow == window)
	{
		//Ogre::WindowEventUtilities::removeWindowEventListener(_RenderWindow,this);
		shutdown();
	}
}

void StateManager::addEventHandler(StateInterface *state)
{
	_OgreRoot->addFrameListener(state);
	_InputManager.setEventHandler(state);
}

void StateManager::removeEventHandler(StateInterface *state)
{
	_OgreRoot->removeFrameListener(state);
	_InputManager.setEventHandler(NULL);
}

Ogre::SceneManager* StateManager::getSceneManager()
{
	return _SceneManager;
}

void StateManager::addChildSceneNode(Ogre::SceneNode *node)
{
	_ParentSceneNode->addChild(node);
}

void StateManager::removeChildSceneNode(Ogre::SceneNode *node)
{
	_ParentSceneNode->removeChild(node);
}

Ogre::SceneNode* StateManager::getParentSceneNode()
{
	return _ParentSceneNode;
}

Ogre::Matrix4 StateManager::getParentTransform()
{
	return _Parameters.ParentNodeTransform;
}

Ogre::RenderWindow* StateManager::getRenderWindow()
{
	return _RenderWindow;
}

StateManager* StateManager::getSingleton()
{
	if(_Instance == NULL)
	{
		_Instance = new StateManager();
		_Instance->setup();
	}

	return _Instance;
}

void StateManager::destroySingleton()
{
	if(_Instance != 0)
	{
		std::cout<<"State Manager: destroying singleton"<<"\n";
		delete _Instance;
		_Instance = 0;
	}
}
