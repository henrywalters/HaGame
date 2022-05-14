#ifndef PARTICLE_EDITOR_H
#define PARTICLE_EDITOR_H

#define USE_QUICK_PATH_SELECT

#include "../../HaGame/HaGame.h"

class ParticleEditorScene : public hagame::Scene {
public:

	float scale = 0.001;

	RawPtr<hagame::ecs::Entity> particle;

	Optional<hagame::utils::File> lastSaveFile = std::nullopt;

	void onSceneInit() {

		addSystem<hagame::graphics::RenderSystem>();

		particle = addEntity();
		auto renderer = particle->addComponent<hagame::graphics::ParticleEmitterRenderer>();
		renderer->shader = game->resources->getShaderProgram("particle");

		auto camera = addEntity();
		auto cameraComp = camera->addComponent<hagame::graphics::CameraComponent>();
		auto orth = std::make_shared<hagame::graphics::OrthographicCamera>(game->window->size * scale);
		cameraComp->camera = orth;
	}

	void onSceneUpdate(double dt) {

		// particle->setPos(game->input.keyboardMouse.mouse.position);

		auto pRenderer = particle->getComponent<hagame::graphics::ParticleEmitterRenderer>();

		bool open;

		ImGui::Begin("Particle Editor", &open, ImGuiWindowFlags_MenuBar);

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		Optional<String> error = std::nullopt;

		if (io.KeyCtrl && io.KeysDown[22]) {
			if (lastSaveFile.has_value()) {
				auto config = particle->getComponent<hagame::graphics::ParticleEmitterRenderer>()->emitter->saveToConfig();
				config.writeToFile(&lastSaveFile.value());
			}
			else {
				ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".conf", ".");
			}
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					std::cout << "SAVE\n";
					if (lastSaveFile.has_value()) {
						std::cout << "SAVE\n";
						auto config = particle->getComponent<hagame::graphics::ParticleEmitterRenderer>()->emitter->saveToConfig();
						config.writeToFile(&lastSaveFile.value());
					}
					else {
						ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".conf", ".");
					}
				}

				if (ImGui::MenuItem("Save As")) {
					ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".conf", ".");
				}

				if (ImGui::MenuItem("Load")) {
					ImGuiFileDialog::Instance()->OpenDialog("Load Particle File", "Choose File", ".conf", ".");
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::CollapsingHeader("General Settings")) {
			ImGui::ColorEdit4("Background Color", game->window->clearColor.vector);
		}

		if (ImGui::CollapsingHeader("Emitter Settings")) {
			pRenderer->emitter->drawUI();
			if (pRenderer->emitter->settings.singleShot) {
				if (ImGui::Button("Fire!")) {
					pRenderer->emitter->fire(game->secondsElapsed);
				}
			}
		}

		if (ImGuiFileDialog::Instance()->Display("Load Particle File"))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action

				try {
					auto filePathParts = stringSplit(filePathName, '\\');
					std::cout << filePathParts[filePathParts.size() - 1] << "\n";
					lastSaveFile = hagame::utils::File(filePath, filePathParts[filePathParts.size() - 1]);
					auto config = hagame::utils::ConfigFile(&lastSaveFile.value());
					particle->getComponent<hagame::graphics::ParticleEmitterRenderer>()->emitter->loadFromConfig(config);
					
				}
				catch (...) {
					error = "Unknown";
				}
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}

		

		if (ImGuiFileDialog::Instance()->Display("Save As"))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action

				auto filePathParts = stringSplit(filePathName, '\\');
				std::cout << filePathParts[filePathParts.size() - 1] << "\n";
				auto file = hagame::utils::File(filePath, filePathParts[filePathParts.size() - 1]);
				auto config = particle->getComponent<hagame::graphics::ParticleEmitterRenderer>()->emitter->saveToConfig();
				config.writeToFile(&file);
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (error.has_value()) {
			ImGui::OpenPopup("Error");
		}

		if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("An error occured :( Check the console for more information");
			ImGui::Separator();

			ImGui::SetItemDefaultFocus();
			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

class ParticleEditor : public hagame::Game {
public:
	ParticleEditor(hagame::graphics::Window* window) : Game(window) {}

	void onGameStart() {

		// window->turnOnVSync();

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

	
		addScene<ParticleEditorScene>("editor");

		scenes.activate("editor");
	}

	void onGameBeforeUpdate() {
		bool uiHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
		if (!uiHovered) {
			// ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		}
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->window);
		ImGui::NewFrame();
	}

	void onGameUpdate() {
		input.keyboardMouse.showCursor();
	}

	void onGameAfterUpdate() {
		// ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window->setTitle("ParticleEditor - " + std::to_string(fps / 1000000));
	}


	void loadResources() {

		Array<String> textures = {

		};

		Array<String> shaders = {
			"particle"
		};

		resources->loadFont("arial", "Fonts/arial.ttf", 36);

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
