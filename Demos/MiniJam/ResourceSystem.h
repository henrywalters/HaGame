#ifndef RESOURCE_SYSTEM_H
#define RESOURCE_SYSTEM_H

#include "./../../HaGame/HaGame.h"

class ResourceSystem : public hagame::ecs::System {
public:

	String getSystemName() {
		return "ResourceSystem";
	}

	void loadTextures() {
		game->resources->loadTexture("kitty", "Textures/kitty.jpg");
	}

	void loadShaders() {
		Array<String> shaders = {
			"text",
			"sprite",
			"color",
			"drawing"
		};

		for (auto shader : shaders) {
			game->resources->loadShaderProgram(shader, "Shaders/" + shader + "_vert.glsl", "Shaders/" + shader + "_frag.glsl");
		}
	}

	void loadFonts() {
		game->resources->loadFont("roboto", "Fonts/Roboto/Roboto-Regular.ttf", 32);
	}

	void onSystemStart() {
		std::cout << "Loading resources..." << std::endl;
		loadTextures();
		loadShaders();
		loadFonts();
		std::cout << "Complete." << std::endl;
	}
};

#endif
