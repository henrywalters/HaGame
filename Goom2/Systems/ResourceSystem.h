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
		game->resources->loadTexture("crosshair", "Textures/crosshair.png");
		game->resources->loadTexture("skull", "Textures/skull1.png");
		game->resources->loadTexture("circle", "Textures/circle.png");
		game->resources->loadTexture("missle", "Textures/missle.png");
		game->resources->loadTexture("plasma_ball", "Textures/plasma_ball.png");
		game->resources->loadTexture("crate", "Textures/crate.jpg");
		game->resources->loadTexture("bullethole", "Textures/bullethole.png");
	}

	void loadShaders() {
		Array<String> shaders = {
			"text",
			"sprite",
			"color",
			"drawing",
			"pixel"
		};

		for (auto shader : shaders) {
			game->resources->loadShaderProgram(shader, "Shaders/" + shader + "_vert.glsl", "Shaders/" + shader + "_frag.glsl");
		}
	}

	void loadFonts() {
		game->resources->loadFont("roboto", "Fonts/Roboto/Roboto-Regular.ttf", 32);
	}

	void loadMeshes() {
		game->resources->loadMesh("cube", hagame::graphics::CubeMesh);
	}

	void onSystemInit() {
		std::cout << "Loading resources..." << std::endl;
		loadTextures();
		loadShaders();
		loadFonts();
		loadMeshes();
		std::cout << "Complete." << std::endl;
	}
};

#endif
