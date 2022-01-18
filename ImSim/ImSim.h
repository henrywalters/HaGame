#ifndef IM_SIM_HPP
#define IM_SIM_HPP

#include "../HaGame/HaGame.h"
#include "./Scenes/TestScene.hpp"
#include "./Scenes/LevelEditor.hpp"
#include "./Scenes/Level1.hpp"
#include "./Scenes/Level2.hpp"

class ImSim : public hagame::Game {
public:
	ImSim(hagame::graphics::Window* window) : Game(window) {}

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

		addScene<TestScene>("TestScene");
		addScene<LevelEditor>("LevelEditor");
		addScene<Level1>("Level1");
		addScene<Level2>("Level2");
		scenes.setActive("Level1");
		//scenes.activate("Level1");
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
		window->setTitle("ImSim - " + std::to_string(fps / 1000000));
	}


	void loadResources() {

		resources->loadMesh("cube", "Models/Cube.obj");
		resources->loadMesh("triangle", Triangle(Vec3({ 0, 0, 0 }), Vec3({ 0, 1, 0 }), Vec3({ 1, 0, 0 })).getMesh());
		resources->loadMesh("camera", "Models/camera.obj");
		resources->loadMesh("glock", "Models/glock.obj");

		resources->loadFont("arial", "Fonts/arial.ttf");

		// Textures
		resources->loadTexture("crate", "Textures/crate.jpg");
		resources->loadTexture("proto", "Textures/prototype.jpg");
		resources->loadTexture("chain_base", "Textures/chainwall/basecolor.png");
		resources->loadTexture("chain_normal", "Textures/chainwall/normal.png");
		resources->loadTexture("chain_opacity", "Textures/chainwall/opacity.png");
		resources->loadTexture("red_dot", "Textures/red_dot.png");
		resources->loadTexture("plasma", "Textures/plasma_ball.png");
		resources->loadTexture("cacodemon", "Textures/cacodemon.png");
		resources->loadTexture("rabbit", "Textures/rarerabbit.jpg");

		Array<String> shaders = {
			"color",
			"texture",
			"text",
			"material",
			"sprite",
			"sprite3d",
			"particle"
		};

		for (auto shader : shaders) {
			resources->loadShaderProgram(shader, "Shaders/" + shader + "_vert.glsl", "Shaders/" + shader + "_frag.glsl");
		}
	}
};

#endif
