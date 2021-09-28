#ifndef GOOM_SANDBOX_H
#define GOOM_SANDBOX_H

#include "../../HaGame/HaGame.h"
#include "./Components/PlayerController.h"
#include "./../Systems/PlayerSystem.h"
#include "./../Systems/WeaponSystem.h"
#include "./../Systems/EnemySystem.h"
#include "./../Systems/ActorSystem.h"

class Sandbox : public hagame::Scene {

	void onSceneInit() {
		std::cout << "Initializing Sandbox\n";

		addSystem<PlayerSystem>();
		addSystem<WeaponSystem>();
		addSystem<EnemySystem>();
		addSystem<ActorSystem>();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(game->window->window, game->window->gl);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}
};

#endif