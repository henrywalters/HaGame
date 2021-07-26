#include "../../HaGame/HaGame.h"

class SpriteSheetHelper : public hagame::Scene {

public:

	float uiPercent = 0.2;

	void handleUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(game->window->window);
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		{
			ImGui::SetNextWindowPos(ImVec2(game->window->size[0] - game->window->size[0] * uiPercent, 0));
			ImGui::SetNextWindowSize(ImVec2(game->window->size[0] * uiPercent, game->window->size[1]));
			ImGui::Begin("SpriteSheetHelper");

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void onSceneActivate() {
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

	void onSceneUpdate(double dt) {
		handleUI();
	}
};