#ifndef IM_SIM_HPP
#define IM_SIM_HPP

#include "../HaGame/HaGame.h"
#include "./Scenes/TestScene.hpp"

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

		addScene<TestScene>("TestScene");

		scenes.activate("TestScene");
	}
};

#endif
