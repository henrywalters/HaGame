#ifndef SH_CONFIG_H
#define SH_CONFIG_H

#include "./../../HaGame/HaGame.h"

class Config {

public:

	Config():
		resources(std::make_shared<hagame::ResourceManager>(CONFIG_DIR))
	{}

	// Load a config file by path name relative to config directory and without the extension.
	hagame::utils::ConfigFile* load(String path) {
		auto fullPath = path + ".conf";
		if (!resources->hasConfigFile(path)) {
			return resources->loadConfigFile(path, fullPath);
		}
		return resources->getConfigFile(path);
	}

private:

	const String CONFIG_DIR = "../../../SpaceHorror/Config";
	Ptr<hagame::ResourceManager> resources;
};

#endif
