#ifndef UI_H
#define UI_H

#include "../../HaGame/HaGame.h"

class UI : public hagame::ecs::System {

public:

	Vec3 editorLightPos = Vec3({ 15.0f, 15.0f, 15.0f });
	hagame::graphics::Color editorLightColor = hagame::graphics::Color::white();

	String getSystemName() {
		return "UI System";
	}

	void onSystemStart() {
		std::cout << "Starting UI System\n";
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

	void onSystemBeforeUpdate() {
		auto shader = game->resources->getShaderProgram("material");
		shader->use();
		shader->setVec3("lightPos", editorLightPos);
		shader->setVec3("lightColor", editorLightColor.resize<3>());
	}

	void onSystemUpdate(double dt) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(game->window->window);
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		{
			ImGui::Begin("Editor Properties");
			ImGui::Text("Adjust common settings of the window");
			ImGui::DragFloat3("Light Position", editorLightPos.vector, 1.0f, -100.0f, 100.0f);
			ImGui::DragFloat4("Light Color", editorLightColor.vector, 0.01f, 0.0f, 1.0f);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

#endif
