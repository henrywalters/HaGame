#ifndef SPACE_HORROR_H
#define SPACE_HORROR_H

#include "../HaGame/HaGame.h"
#include "./Common/RuntimeLevel.cpp";
#include "./Scenes/Demo.cpp"
#include "./Systems/PlatformerSystem.cpp"

class SpaceHorror : public hagame::Game {
public:
	SpaceHorror(hagame::graphics::Window* window) : Game(window) {}

	void onGameStart() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(window->window, window->gl);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		resources->setBasePath("../../../Assets");

		loadResources();

		addScene<Demo>("demo");

		scenes.setActive("demo");

	}

	void onGameBeforeUpdate() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->window);
		ImGui::NewFrame();
	}

	void onGameAfterUpdate() {
		// ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window->setTitle("SpaceHorror - " + std::to_string(fps / 1000000));
	}


	void loadResources() {

		Array<String> textures = {
			"henry.png",
			"cacodemon.png",
			"crosshair.png",
			"scifi_block.png",
			"prototype.jpg",
		};

		Array<String> shaders = {
			"color",
			"text",
			"sprite",
			"particle",
			"batch_line",
		};

		resources->loadFont("arial", "Fonts/arial.ttf");

		for (auto texture : textures) {
			auto parts = stringSplit(texture, '.');
			resources->loadTexture(parts[0], "Textures/" + texture);
		}

		for (auto shader : shaders) {
			resources->loadShaderProgram(shader, "Shaders/" + shader + "_vert.glsl", "Shaders/" + shader + "_frag.glsl");
		}
	}
};

#endif
