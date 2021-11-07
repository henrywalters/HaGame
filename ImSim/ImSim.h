#ifndef IM_SIM_HPP
#define IM_SIM_HPP

#include "../HaGame/HaGame.h"
#include "./Scenes/TestScene.hpp"
#include "./Scenes/LevelEditor.hpp"

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
		addScene<LevelEditor>("LevelEditor");

		scenes.activate("LevelEditor");
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
};

#endif
