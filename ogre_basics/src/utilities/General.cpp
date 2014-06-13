/*
 * General.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: ros developer 
 */

#include <utilities/General.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options ;
namespace application_parameters
{
namespace utils
{

	bool parse_console_parameters(int argc,char** argv,std::string help_text,OgreParameters default_params,OgreParameters &params)
	{
		// program arguments
		boost::program_options::options_description prog_options_desc("General");
		prog_options_desc.add_options()
				("help",help_text.c_str())
				("config-dir",po::value<std::string>(&params.ParentDirectory)->default_value(default_params.ParentDirectory),
						"path to configuration files directory")
				("resource-file",po::value<std::string>(&params.ResourcesFile)->default_value(default_params.ResourcesFile),
						"name of resource file")
				("plugins-file",po::value<std::string>(&params.PluginsFile)->default_value(default_params.PluginsFile),
						"name of plugins file")
				("ogre-config-file",po::value<std::string>(&params.ConfigFile)->default_value(default_params.ConfigFile),
						"name of ogre config file")
				("console-logging",po::value<bool>(&params.ConsoleLogging)->default_value(default_params.ConsoleLogging),
						"enable console loggin [0 | 1]");

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, prog_options_desc), vm);
		po::notify(vm);

		if(vm.count("help"))
		{
			std::cout<<prog_options_desc<<std::endl;
			return false;
		}

		return true;
	}

}
}

