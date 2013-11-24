/*
 * MaterialCreator.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: ros developer 
 */

#include <utilities/MaterialCreator.h>

const std::string MaterialCreator::RESOURCE_GROUP = "UserMaterials";
const std::string MaterialCreator::RESOURCE_DIRECTORY="user_media/textures";
MaterialCreator::Ptr MaterialCreator::single_instance_;

MaterialCreator::MaterialCreator() {
	// TODO Auto-generated constructor stub
	init();

}

MaterialCreator::~MaterialCreator() {
	// TODO Auto-generated destructor stub
	std::cout<<"MaterialCreator Destructor called"<<std::endl;
	//close();
}

void MaterialCreator::init()
{
	// create resource group
	std::cout<<"Creating MaterialCreator Resource Group"<<std::endl;
	Ogre::ResourceGroupManager& r = Ogre::ResourceGroupManager::getSingleton();
	r.createResourceGroup(RESOURCE_GROUP);
	r.addResourceLocation(RESOURCE_DIRECTORY,"FileSystem",RESOURCE_GROUP);
	r.initialiseResourceGroup(RESOURCE_GROUP);
	r.loadResourceGroup(RESOURCE_GROUP);

	std::cout<<"Creating Materials"<<std::endl;
	// create all materials here
	material_map_ = MaterialMap();

	float brigtness_reduction = 0.8f;
	Ogre::ColourValue diffuse_color = Ogre::ColourValue(0,0,0.8f,1);
	Ogre::ColourValue ambient_color = brigtness_reduction*diffuse_color;
	Ogre::ColourValue specular_color = Ogre::ColourValue(1,1,1,1);
	material_map_.insert(std::make_pair(BLUE,
			build_solid_material(std::string("BlueMaterial"),diffuse_color,ambient_color,specular_color)->getName()));

	diffuse_color = Ogre::ColourValue(1.0f,1.0f,0,1.0f);
	ambient_color = brigtness_reduction*diffuse_color;
	material_map_.insert(std::make_pair(YELLOW,
			build_solid_material(std::string("YellowMaterial"),diffuse_color,ambient_color,specular_color)->getName()));

	diffuse_color = Ogre::ColourValue(1.0f,0,0,1.0f);
	ambient_color = brigtness_reduction*diffuse_color;
	material_map_.insert(std::make_pair(RED,
			build_solid_material(std::string("RedMaterial"),diffuse_color,ambient_color,specular_color)->getName()));

	diffuse_color = Ogre::ColourValue(0,1.0f,0,1.0f);
	ambient_color = brigtness_reduction*diffuse_color;
	material_map_.insert(std::make_pair(GREEN,
			build_solid_material(std::string("GreeMaterial"),diffuse_color,ambient_color,specular_color)->getName()));

	diffuse_color = Ogre::ColourValue(192.0f/255.0f,192.0f/255.0f,192.0f/255.0f,1.0f);
	ambient_color = brigtness_reduction*diffuse_color;
	material_map_.insert(std::make_pair(GRAY,
			build_solid_material(std::string("GrayMaterial"),diffuse_color,ambient_color,specular_color)->getName()));

	diffuse_color = Ogre::ColourValue(0,0,0,1.0f);
	ambient_color = brigtness_reduction*diffuse_color;
	material_map_.insert(std::make_pair(BLACK,
			build_solid_material(std::string("BlackMaterial"),diffuse_color,ambient_color,specular_color)->getName()));

	diffuse_color = Ogre::ColourValue(1,1,1,1.0f);
	material_map_.insert(std::make_pair(WIREFRAME_WHITE,
			build_wireframe_material(std::string("WireframeWhiteMaterial"),diffuse_color,diffuse_color,diffuse_color)->getName()));

	diffuse_color = Ogre::ColourValue(192.0f/255.0f,192.0f/255.0f,192.0f/255.0f,1.0f);
	material_map_.insert(std::make_pair(WIREFRAME_GRAY,
			build_wireframe_material(std::string("WireframeGrayMaterial"),diffuse_color,diffuse_color,diffuse_color)->getName()));

	std::cout<<"Completed MaterialCreator initialization"<<std::endl;
}

void MaterialCreator::close()
{
	//if(Ogre::Root::getSingletonPtr() == NULL)return;

	std::cout<<"Destroying Material Resource Group "<<RESOURCE_GROUP<<"\n";
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(RESOURCE_GROUP);
}

Ogre::MaterialPtr MaterialCreator::build_solid_material(std::string name,Ogre::ColourValue &diffuse,
		Ogre::ColourValue &ambient, Ogre::ColourValue &specular)
{

	Ogre::MaterialManager& material_mngr = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialPtr m = material_mngr.create(name,RESOURCE_GROUP);

	Ogre::Technique* first_technique = m->getTechnique(0);
	Ogre::Pass* first_pass = first_technique->getPass(0);
	first_pass->setLightingEnabled(true);
	first_pass->setDiffuse(diffuse.r,diffuse.g,diffuse.b,diffuse.a);
	first_pass->setAmbient(ambient.r,ambient.g,ambient.b);
	first_pass->setSpecular(specular.r,specular.g,specular.b,specular.a);
	//first_pass->setShininess(64.0f);
	first_pass->setSelfIllumination(Ogre::ColourValue(0,0,0,1));

	return m;

}

Ogre::MaterialPtr MaterialCreator::build_wireframe_material(std::string name,Ogre::ColourValue &diffuse,
		Ogre::ColourValue &ambient, Ogre::ColourValue &specular)
{

	Ogre::MaterialManager& material_mngr = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialPtr m = material_mngr.create(name,RESOURCE_GROUP);

	Ogre::Technique* first_technique = m->getTechnique(0);
	Ogre::Pass* first_pass = first_technique->getPass(0);
	first_pass->setDiffuse(diffuse.r,diffuse.g,diffuse.b,diffuse.a);
	first_pass->setAmbient(ambient.r,ambient.g,ambient.b);
	first_pass->setSpecular(specular.r,specular.g,specular.b,specular.a);
	first_pass->setShininess(64.0f);
	first_pass->setSelfIllumination(diffuse);
	first_pass->setPolygonMode(Ogre::PM_WIREFRAME);

	return m;

}

Ogre::MaterialPtr MaterialCreator::get_material(Colors c)
{
	Ogre::MaterialPtr p;
	if(material_map_.count((int)c) > 0)
	{
		p = Ogre::MaterialManager::getSingleton().getByName(material_map_[(int)c],RESOURCE_GROUP);
	}

	return p;
}

MaterialCreator::Ptr MaterialCreator::get_singleton()
{
	if(!single_instance_)
	{
		single_instance_ = MaterialCreator::Ptr(new MaterialCreator());
	}
	return single_instance_;
}

