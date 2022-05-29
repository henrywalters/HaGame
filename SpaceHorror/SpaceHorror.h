#ifndef SPACE_HORROR_H
#define SPACE_HORROR_H

#include "../HaGame/HaGame.h"
#include "./Common/RuntimeLevel.cpp";
#include "./Common/Weapons.cpp";
#include "./Scenes/Demo.cpp"
#include "./Scenes/Home.h"
#include "./Scenes/Editor.cpp"
#include "./Systems/PlatformerSystem.cpp"
#include "./Systems/AISystem.cpp"
#include "./../HaGame/Graphics/EXR.h"

class SpaceHorror : public hagame::Game {
public:
	SpaceHorror(hagame::graphics::Window* window) : Game(window) {}

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

		addScene<Demo>("demo");
		addScene<Home>("home");
		addScene<Editor>("editor");

		scenes.setActive("home");

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
			"explosive_barrel.png"
		};

		Array<String> shaders = {
			"color",
			"text",
			"sprite",
			"sprite2d",
			"particle",
			"batch_line",
		};

		resources->loadEXR("test_exr", "Textures/SpaceHorror/Player/0001.exr");

		resources->loadFile("bullets_conf", "../SpaceHorror/Config/Bullets.conf");
		resources->loadFile("weapons_conf", "../SpaceHorror/Config/Weapons.conf");

		resources->loadFont("arial", "Fonts/arial.ttf", 36);
		resources->loadFont("horroroid", "Fonts/horroroid/horroroid.ttf", 64);

		resources->loadAudioSample("creepy_home", "creepy_ambience.wav");
		resources->loadAudioSample("thud", "Audio/thud.wav");

		// These textures are used by the editor
		resources->loadTexture("audio_texture", "Textures/HaGame/ui/audio.png");
		resources->loadTexture("font_texture", "Textures/HaGame/ui/font.png");
		resources->loadTexture("mesh_texture", "Textures/HaGame/ui/mesh.png");
		resources->loadTexture("shader_texture", "Textures/HaGame/ui/shader.png");

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
