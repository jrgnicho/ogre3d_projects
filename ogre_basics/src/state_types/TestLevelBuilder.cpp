/*
 * TestLevelBuilder.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: ros developer 
 */

#include <state_types/TestLevelBuilder.h>
#include <utilities/ShapeDrawer.h>
#include <utilities/MaterialCreator.h>
#include <limits>
#include <cmath>

const float TestLevelBuilder::GRID_SIZE = 40.0f;
const int TestLevelBuilder::GRID_SEGMENTS = 80;
const std::string TestLevelBuilder::STATE_NAME = "TestLevelBuilder";
const std::string TestLevelBuilder::GRID_NODE = "GridNode";
const std::string TestLevelBuilder::GRID_ENTITY = "Grid";

const std::tuple<int,int> TestLevelBuilder::PANEL_POSITION = std::make_tuple(10,10);
const std::tuple<int,int> TestLevelBuilder::TEXT_POSITION = std::make_tuple(0,0);
const std::tuple<int,int> TestLevelBuilder::TEXT_DIMENSIONS = std::make_tuple(100,100);
const unsigned int TestLevelBuilder::CHAR_HEIGHT = 16;
const std::string TestLevelBuilder::FONT_NAME = "StarWars";
const std::string TestLevelBuilder::TEXT_AREA_NAME = "LevelBuilderTextArea";
const std::string TestLevelBuilder::PANEL_NAME = "LevelBuilderPanel";
const std::string TestLevelBuilder::OVERLAY_NAME = "LevelBuilderOverlay";

TestLevelBuilder::TestLevelBuilder():
		grid_width_(GRID_SIZE),
		grid_length_(GRID_SIZE),
		grid_segments_(GRID_SEGMENTS),
		voxel_size_(GRID_SIZE/GRID_SEGMENTS),
		ray_scene_query_(0),
		pointer_entity_name_("pointer_voxel"),
		create_new_voxel_(false),
		text_area_(0)
	{
	// TODO Auto-generated constructor stub
	_StateName = STATE_NAME;

}

TestLevelBuilder::~TestLevelBuilder() {
	// TODO Auto-generated destructor stub
}

StateInterface* TestLevelBuilder::getSingleton()
{
	if(_Instance == NULL)
	{
		TestLevelBuilder *state = new TestLevelBuilder();
		_Instance = state;
		StateManager::getSingleton()->manageState(_Instance);
		//state->setup(); // called by SceneManager
	}

	return _Instance;
}

void TestLevelBuilder::setupSceneComponents()
{
	// creating instances of scene nodes
	Ogre::SceneManager* mngr =  StateManager::getSingleton()->getSceneManager();
	_ParentSceneNode = mngr->createSceneNode("World");

	// create grid
	std::string grid_node_name = GRID_NODE;
	Ogre::SceneNode* grid_node = _ParentSceneNode->createChildSceneNode(grid_node_name);
	Ogre::MeshPtr grid_mesh = ShapeDrawer::getSingleton()->create_grid_mesh(grid_width_,grid_length_,grid_segments_,grid_segments_);
	Ogre::Entity* grid_entity = mngr->createEntity(GRID_ENTITY,grid_mesh->getName());
	nodes_names_.push_back(grid_node_name);
	grid_node->attachObject(grid_entity);
	grid_entity->setCastShadows(false);

	// create voxel pointer
	Ogre::MeshPtr cube_mesh = ShapeDrawer::getSingleton()->get_mesh(ShapeDrawer::BOX);
	Ogre::MaterialPtr cube_material= MaterialCreator::get_singleton()->get_material(MaterialCreator::TRANSPARENT_RED);
	//Ogre::MaterialPtr cube_material= MaterialCreator::get_singleton()->get_material(MaterialCreator::WIREFRAME_GRAY);
	Ogre::Entity* entity = mngr->createEntity(pointer_entity_name_,cube_mesh->getName());
	entity->setCastShadows(false);
	entity->setMaterial(cube_material);
	pointer_node_ = _ParentSceneNode->createChildSceneNode("PointerNode",Ogre::Vector3(voxel_size_/2,voxel_size_/2,voxel_size_/2));
	pointer_node_->attachObject(entity);
	pointer_node_->setScale(voxel_size_,voxel_size_,voxel_size_);

	// setting scene size
	Ogre::AxisAlignedBox scene_aabb;
	Ogre::Vector3 bounds = Ogre::Vector3(10000,10000,10000);
	scene_aabb.setExtents(-bounds,bounds);
	mngr->setOption("Size",&scene_aabb);

	// creating ray scene query object
	ray_scene_query_ = mngr->createRayQuery(Ogre::Ray(),Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

	// setup text area
	Ogre::OverlayManager* overlay_mngr = Ogre::OverlayManager::getSingletonPtr();
	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlay_mngr->createOverlayElement("Panel",
			PANEL_NAME));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(std::get<0>(PANEL_POSITION),std::get<1>(PANEL_POSITION));
	panel->setDimensions(std::get<0>(TEXT_DIMENSIONS),std::get<1>(TEXT_DIMENSIONS));
	text_area_ = static_cast<Ogre::TextAreaOverlayElement*>(overlay_mngr->createOverlayElement("TextArea",TEXT_AREA_NAME));
	text_area_->setMetricsMode(Ogre::GMM_PIXELS);
	text_area_->setPosition(std::get<0>(TEXT_POSITION),std::get<1>(TEXT_POSITION));
	text_area_->setDimensions(std::get<0>(TEXT_DIMENSIONS),std::get<1>(TEXT_DIMENSIONS));
	text_area_->setCharHeight(CHAR_HEIGHT);
	text_area_->setFontName(FONT_NAME);
	text_area_->setColourBottom(Ogre::ColourValue(0.3, 0.5, 0.3));
	text_area_->setColourTop(Ogre::ColourValue(0.5, 0.7, 0.5));

	Ogre::Overlay* overlay = overlay_mngr->create(OVERLAY_NAME);
	overlay->add2D(panel);
	panel->addChild(text_area_);
	overlay->show();

}


void TestLevelBuilder::setupCameraControllers()
{
	TestState::setupCameraControllers();
	_CameraController.setPosition(Ogre::Vector3(-10,0,2));
}

void TestLevelBuilder::cleanupSceneComponents()
{
	std::cout<< STATE_NAME<<"Destroying scene components"<<std::endl;
	Ogre::SceneManager *sceneManager = StateManager::getSingleton()->getSceneManager();

	for(std::vector<std::string>::iterator iter = nodes_names_.begin();iter != nodes_names_.end();iter++)
	{
		Ogre::SceneNode* node = sceneManager->getSceneNode(*iter);
		Ogre::SceneNode::ObjectIterator i = node->getAttachedObjectIterator();
		while(i.hasMoreElements())
		{
			sceneManager->destroyEntity(i.getNext()->getName());
		}
		node->detachAllObjects();
		sceneManager->destroySceneNode(node);
	}
	sceneManager->destroySceneNode(_ParentSceneNode);

}

bool TestLevelBuilder::mousePressed(const OIS::MouseEvent &evnt,OIS::MouseButtonID id)
{
	create_new_voxel_ = id == OIS::MB_Left;
	return true;
}

bool TestLevelBuilder::frameStarted(const Ogre::FrameEvent &evnt)
{
	const OIS::MouseState &ms = StateManager::getSingleton()->getInputManager().getMouse()->getMouseState();
	move_pointer_node(ms);
	update_text_area();

	return true;
}

void TestLevelBuilder::move_pointer_node(const OIS::MouseState &ms)
{
	Ogre::SceneManager* manager = StateManager::getSingleton()->getSceneManager();

	float x = ms.X.abs/float(_CameraController.getCamera()->getViewport()->getActualWidth());
	float y = ms.Y.abs/float(_CameraController.getCamera()->getViewport()->getActualHeight());

	// creating ray and converting to world coordinates
	Ogre::Ray ray = _CameraController.getCamera()->getCameraToViewportRay(x,y);
	Ogre::Quaternion rot = Ogre::Quaternion(Ogre::Radian(Ogre::Math::HALF_PI),Ogre::Vector3(1,0,0));
	Ogre::Vector3 dir = rot* ray.getDirection();dir.normalise();
	Ogre::Vector3 pos = _CameraController.getPosition();
	ray = Ogre::Ray(pos,dir);


	float closest_distance = std::numeric_limits<float>::max();
	bool  hit_found = false;
	Ogre::Vector3 new_pos;
	for(int i = 0; i < this->nodes_names_.size(); i++)
	{
		Ogre::SceneNode* node = manager->getSceneNode(nodes_names_[i]);
		Ogre::SceneNode::ObjectIterator objs = node->getAttachedObjectIterator();
		while(objs.hasMoreElements())
		{
			Ogre::AxisAlignedBox aabb = ((Ogre::Entity*)objs.getNext())->getBoundingBox();
			Ogre::Matrix4 m = Ogre::Matrix4();
			m.makeTransform(node->getPosition(),node->getScale(),node->getOrientation());
			aabb.transform(m);

			std::pair<bool,Ogre::Real> hit = Ogre::Math::intersects(ray,aabb);
			if(hit.first)
			{
				hit_found = hit.first;
				closest_distance = hit.second < closest_distance ? hit.second : closest_distance;
			}
		}
	}


	if(hit_found)
	{
		new_pos = ray.getPoint(closest_distance);
		new_pos.x = voxel_size_ *std::floor((1/voxel_size_)*new_pos.x) + voxel_size_/2.0f;
		new_pos.y = voxel_size_ *std::floor((1/voxel_size_)*new_pos.y) + voxel_size_/2.0f;
		new_pos.z = voxel_size_ *std::floor((1/voxel_size_)*new_pos.z) + voxel_size_/2.0f;
		//std::cout<<"Pointer position: ["<<new_pos.x<<", "<<new_pos.y<<", "<<new_pos.z<<"]"<<std::endl;
		pointer_node_->setPosition(new_pos);
	}

	if(create_new_voxel_)
	{
		create_voxel(pointer_node_->getPosition());
		create_new_voxel_ = false;
	}

}

void TestLevelBuilder::update_text_area()
{
	std::stringstream ss;
	ss<<"LEVEL BUILDER"<<"\n";
	ss<<"\tVoxel Count: "<<nodes_names_.size()<<"\n";
	text_area_->setCaption(ss.str());

}

void TestLevelBuilder::create_voxel(const Ogre::Vector3& pos)
{
	// create voxel pointer
	Ogre::SceneManager* manager = StateManager::getSingleton()->getSceneManager();
	Ogre::MeshPtr cube_mesh = ShapeDrawer::getSingleton()->get_mesh(ShapeDrawer::BOX);
	Ogre::MaterialPtr cube_material = MaterialCreator::get_singleton()->create_texture_material("linux_logo_1.png",1,1,1);
	Ogre::Entity* entity = manager->createEntity(cube_mesh->getName());
	entity->setMaterial(cube_material);
	Ogre::SceneNode* node = _ParentSceneNode->createChildSceneNode(pos);
	node->attachObject(entity);
	node->setScale(voxel_size_,voxel_size_,voxel_size_);
	nodes_names_.push_back(node->getName());
}



