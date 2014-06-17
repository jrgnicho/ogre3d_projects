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

StateManager* StateManager::instance_ = NULL;

StateManager::StateManager()
:ogre_root_(0),
 scene_manager_(0),
 render_window_(0),
 parent_scene_node_(0),
 shutdown_issued_(false),
 parameters_()
{
	// TODO Auto-generated constructor stub
}

StateManager::~StateManager() {
	// TODO Auto-generated destructor stub

	cleanup();
}

void StateManager::setup()
{
	//parameters_ = OgreParameters();
	//parameters_.ParentDirectory = ""; // use boost or ros package to find the application directory;
	std::stringstream logStream;

	if(ogre_root_==NULL)
	{

		// initializing log output
		std::string dir = parameters_.ParentDirectory;
		Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager();
		logMgr->createLog(dir + parameters_.LogFile, true, parameters_.ConsoleLogging, false);

		// initializing root
		std::cout<<"Creating Ogre Root Instance"<<"\n";
		ogre_root_ = new Ogre::Root(dir + parameters_.PluginsFile,
				dir + parameters_.ConfigFile,dir + parameters_.LogFile);


		logStream<<"\n ---------------------------------------------------------------------";
		logStream<<"\n ------------------ Created Ogre Root Instance  ----------------------";
		Ogre::LogManager::getSingleton().logMessage(Ogre::String(logStream.str()));logStream.str("");
		Ogre::LogManager::getSingleton().setLogDetail(Ogre::LoggingLevel::LL_NORMAL);
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

		ogre_root_->addFrameListener(this);
		Ogre::WindowEventUtilities::addWindowEventListener(render_window_,this);

		std::size_t windowHandle;
		std::stringstream ss;
		render_window_->getCustomAttribute("WINDOW",&windowHandle);
		ss<<windowHandle;
		input_manager_ = InputManager(ss.str()); // needs to pass the right window handle string from the window instance
		input_manager_.setup();

	}

}

void StateManager::setupRenderSystem() throw (StateManager::InitializationException)
{
	Ogre::Root *root = ogre_root_;
	if(!root->restoreConfig() && !root->showConfigDialog())
	{
		throw InitializationException("Render System was not selected");
	}
	root->initialise(false);
	render_window_ = root->createRenderWindow(parameters_.WindowName,
			parameters_.WindowWidth,parameters_.WindowHeight,false);
}

void StateManager::setupResources() throw (StateManager::InitializationException)
{
	typedef std::string Str;
	Str sectionName, typeName, archiveName;
	Str dir = parameters_.ParentDirectory;
	Ogre::ConfigFile configFile;
	configFile.load(dir + parameters_.ResourcesFile);
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

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(parameters_.Mipmaps);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void StateManager::setupScene() throw (StateManager::InitializationException)
{
	// scene setp
	//scene_manager_ = ogre_root_->createSceneManager(Ogre::ST_GENERIC,parameters_.SceneManagerName);
	scene_manager_ = ogre_root_->createSceneManager("OctreeSceneManager",parameters_.SceneManagerName);


//	scene_manager_->setSkyBox(true,parameters_.SceneSkyBoxMaterialName,parameters_.SceneSkyBoxDistance,
//			true,parameters_.ParentNodeTransform.extractQuaternion());
	scene_manager_->setSkyBox(true,parameters_.SceneSkyBoxMaterialName,parameters_.SceneSkyBoxDistance,
			true,Ogre::Quaternion::IDENTITY);

	// creating nodes
	parent_scene_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode(parameters_.ParentNodeName,
			parameters_.ParentNodeTransform.getTrans(),parameters_.ParentNodeTransform.extractQuaternion());

	// adding light
	Ogre::Light* light = scene_manager_->createLight();
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(0,0,-1).normalisedCopy());
	light->setDiffuseColour(parameters_.SceneAmbientLightColor);
	light->setSpecularColour(parameters_.SceneAmbientLightColor);
	Ogre::SceneNode* light_node = parent_scene_node_->createChildSceneNode(parameters_.SceneAmbientLightPosition);
	light_node->attachObject(light);

	// ambient light
	//scene_manager_->setAmbientLight(parameters_.SceneAmbientLightColor);
	scene_manager_->setAmbientLight(Ogre::ColourValue(0.6f,0.6f,0.6f));
	scene_manager_->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);

}

/*
 * the cleanup method must dispose of objects and resources in a very specific sequence.  Otherwise a
 * segmentation fault or similar errors will be produced during the cleanup. Always use the sequence shown below.
 */
void StateManager::cleanup()
{
	// destroying all states
	std::cout<<"State Manager: cleanup started"<<"\n";
	while(!managed_states_.empty())
	{
		StateInterface *state = managed_states_.begin()->second;
		state->cleanup();
		delete state;
		managed_states_.erase(managed_states_.begin());
	}

	// cleaning up ogre components
	std::cout<<"State Manager:\t-destroying the ogre scene manager"<<"\n";
	ogre_root_->destroySceneManager(scene_manager_);

	std::cout<<"State Manager:\t-removing and destroying input devices"<<"\n";
	input_manager_.cleanup();

	// removing listeners
	ogre_root_->removeFrameListener(this);
	if(render_window_ != NULL)
	{
		std::cout<<"State Manager:\t-removing window listener and destroying window"<<"\n";
		Ogre::WindowEventUtilities::removeWindowEventListener(render_window_,this);
		render_window_->destroy();
		render_window_ = NULL;
	}

	std::cout<<"State Manager:\t-destroying ogre root instance"<<"\n";
	delete ogre_root_;

	scene_manager_ = 0;
	render_window_ = 0;
	ogre_root_ = 0;

	std::cout<<"State Manager: cleanup finished"<<"\n";
}

void StateManager::start(StateInterface *state)
{
	changeState(state);

	if(render_window_ != NULL)
	{
		if(render_window_->isClosed())
		{
			render_window_->destroy();
			render_window_ = ogre_root_->createRenderWindow(parameters_.WindowName,
					parameters_.WindowWidth,parameters_.WindowHeight,false);
			Ogre::WindowEventUtilities::addWindowEventListener(render_window_,this);
		}
	}
	else
	{
		render_window_ = ogre_root_->createRenderWindow(parameters_.WindowName,
					parameters_.WindowWidth,parameters_.WindowHeight,false);
		Ogre::WindowEventUtilities::addWindowEventListener(render_window_,this);
	}
	ogre_root_->startRendering();
}

void StateManager::changeState(StateInterface *state)
{
	std::cout<<"State Manager: Changing to state "<<state->getStateName()<<"\n";

	// remove and exit current state
	if(!state_stack_.empty())
	{
		removeEventHandler(state_stack_.back());
		state_stack_.back()->exit();
		state_stack_.pop_back();
	}

	// add and start state
	state_stack_.push_back(state);
	addEventHandler(state);
	state_stack_.back()->enter();

	std::cout<<"State Manager: Done changing state instance"<<"\n";
}

bool StateManager::pushState(StateInterface *state)
{
	if(!state_stack_.empty())
	{
		if(!state_stack_.back()->pause())
		{
			return false;
		}
		removeEventHandler(state_stack_.back());
	}

	state_stack_.push_back(state);
	addEventHandler(state);
	state_stack_.back()->enter();

	return true;
}

void StateManager::popState()
{
	if(!state_stack_.empty())
	{
		removeEventHandler(state_stack_.back());
		state_stack_.back()->exit();
		state_stack_.pop_back();
	}

	if(!state_stack_.empty())
	{
		addEventHandler(state_stack_.back());
		state_stack_.back()->resume();
	}
	else
	{
		shutdown();
	}
}

void StateManager::shutdown()
{
	shutdown_issued_ = true;
}

void StateManager::manageState(StateInterface *state)
{
	std::cout<<"State Manager: Registered " + state->getStateName() + "state as a managed state"<<"\n";
	state->setup();
	managed_states_.insert(std::make_pair(state->getStateName(),state));
}

InputManager& StateManager::getInputManager()
{
	return input_manager_;
}

bool StateManager::frameStarted(const Ogre::FrameEvent &evnt)
{
	if(shutdown_issued_)
	{
		removeEventHandler(state_stack_.back());
		while(!state_stack_.empty())
		{
			state_stack_.back()->exit();
			state_stack_.pop_back();
		}
		return false;
	}
	else
	{
		input_manager_.getKeyboard()->capture();
		input_manager_.getMouse()->capture();
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
	input_manager_.getMouse()->getMouseState().width = width;
	input_manager_.getMouse()->getMouseState().height = height;

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
	if(render_window_ == window)
	{
		//Ogre::WindowEventUtilities::removeWindowEventListener(render_window_,this);
		shutdown();
	}
}

void StateManager::addEventHandler(StateInterface *state)
{
	ogre_root_->addFrameListener(state);
	input_manager_.setEventHandler(state);
}

void StateManager::removeEventHandler(StateInterface *state)
{
	ogre_root_->removeFrameListener(state);
	input_manager_.setEventHandler(NULL);
}

Ogre::SceneManager* StateManager::getSceneManager()
{
	return scene_manager_;
}

void StateManager::addChildSceneNode(Ogre::SceneNode *node)
{
	parent_scene_node_->addChild(node);
}

void StateManager::removeChildSceneNode(Ogre::SceneNode *node)
{
	parent_scene_node_->removeChild(node);
}

Ogre::SceneNode* StateManager::getParentSceneNode()
{
	return parent_scene_node_;
}

Ogre::Matrix4 StateManager::getParentTransform()
{
	return parameters_.ParentNodeTransform;
}

Ogre::RenderWindow* StateManager::getRenderWindow()
{
	return render_window_;
}

StateManager* StateManager::getSingleton()
{
	return init(instance_->parameters_);
}

StateManager* StateManager::init(const application_parameters::OgreParameters& parameters)
{
	if(instance_ == NULL)
	{
		instance_ = new StateManager();
		instance_->parameters_ = parameters;
		instance_->setup();
	}

	return instance_;
}

void StateManager::destroySingleton()
{
	if(instance_ != 0)
	{
		std::cout<<"State Manager: destroying singleton"<<"\n";
		delete instance_;
		instance_ = 0;
	}
}
