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
	close();
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

	Ogre::ColourValue color = Ogre::ColourValue(0,0,1.0f,1.0f);
	material_map_.insert(std::make_pair(BLUE,build_solid_material(std::string("BlueMaterial"),color,color,color)));

	color = Ogre::ColourValue(1.0f,1.0f,0,1.0f);
	material_map_.insert(std::make_pair(YELLOW,build_solid_material(std::string("YellowMaterial"),color,color,color)));

	color = Ogre::ColourValue(1.0f,0,0,1.0f);
	material_map_.insert(std::make_pair(RED,build_solid_material(std::string("RedMaterial"),color,color,color)));

	color = Ogre::ColourValue(0,1.0f,0,1.0f);
	material_map_.insert(std::make_pair(GREEN,build_solid_material(std::string("GreeMaterial"),color,color,color)));

	color = Ogre::ColourValue(192.0f/255.0f,192.0f/255.0f,192.0f/255.0f,1.0f);
	material_map_.insert(std::make_pair(GRAY,build_solid_material(std::string("GrayMaterial"),color,color,color)));

	color = Ogre::ColourValue(0,0,0,1.0f);
	material_map_.insert(std::make_pair(BLACK,build_solid_material(std::string("BlackMaterial"),color,color,color)));

	color = Ogre::ColourValue(1,1,1,1.0f);
	material_map_.insert(std::make_pair(WIREFRAME_WHITE,
			build_wireframe_material(std::string("WireframeWhiteMaterial"),color,color,color)));

	color = Ogre::ColourValue(192.0f/255.0f,192.0f/255.0f,192.0f/255.0f,1.0f);
	material_map_.insert(std::make_pair(WIREFRAME_GRAY,
			build_wireframe_material(std::string("WireframeGrayMaterial"),color,color,color)));

	std::cout<<"Completed MaterialCreator initialization"<<std::endl;
}

void MaterialCreator::close()
{
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
	first_pass->setDiffuse(diffuse.r,diffuse.g,diffuse.b,diffuse.a);
	first_pass->setAmbient(ambient.r,ambient.g,ambient.b);
	first_pass->setSpecular(specular.r,specular.g,specular.b,specular.a);
	first_pass->setShininess(64.0f);
	first_pass->setSelfIllumination(0.1f,0.1f,0.1f);

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
	first_pass->setSelfIllumination(0.1f,0.1f,0.1f);
	first_pass->setPolygonMode(Ogre::PM_WIREFRAME);

	return m;

}

Ogre::MaterialPtr MaterialCreator::get_material(Colors c)
{
	Ogre::MaterialPtr p;
	if(material_map_.count((int)c) > 0)
	{
		p = material_map_[(int)c];
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

