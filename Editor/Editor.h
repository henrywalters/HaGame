#include "../HaGame/HaGame.h"
#include "Systems/CameraController.h"
#include "Systems/Renderer.h"
#include "Systems/MousePicker.h"

class HaGameEditor : public hagame::Game {
public:
	
	hagame::Transform origin;
	
	hagame::Scene* editor;

	hagame::graphics::PerspectiveCamera editorCamera = hagame::graphics::PerspectiveCamera();
	Vec3 editorLightPos = Vec3({ 10.0f, 50.0f, 10.0f });
	hagame::graphics::Color editorLightColor = hagame::graphics::Color::white();

	hagame::graphics::ShaderProgram* materialShader;
	hagame::graphics::ShaderProgram* colorShader;

	String activeGame = "untitled_game.hg";
	String version = "0.0.1";

	hagame::Transform test;
	hagame::graphics::Mesh cube;

	MaterialRenderer* renderer;

	Vec2 viewportRatio = Vec2({ 0.6, 0.8 });

	HaGameEditor(hagame::graphics::Window* window) : hagame::Game(window) {}

	void onGameStart() {

		Vec2 viewportSize = window->size.prod(viewportRatio);
		Vec2 viewportPos = (window->size - viewportSize) * 0.5;
		viewportPos[1] = 0.0f;

		window->setViewport(Rect(viewportPos, viewportSize));

		editor = addScene("editor");
		editor->activeCamera = &editorCamera;
		editor->addSystem<Renderer>();
		editor->addSystem<CameraController>();
		editor->addSystem<MousePicker>();

		resources.setBasePath("../../../Assets");

		cube = hagame::graphics::Mesh::FromOBJ(resources.loadFile("cube_model", "Models/Blender.obj"));

		materialShader = resources.loadShaderProgram("material", "Shaders/material_vert.glsl", "Shaders/material_frag.glsl");
		colorShader = resources.loadShaderProgram("color", "Shaders/color_vert.glsl", "Shaders/color_frag.glsl");

		editorCamera.aspectRatio = viewportSize[0] / viewportSize[1];
		editorCamera.transform.position = Vec3({ -15.0f, 15.0f, -15.0f });
		editorCamera.transform.rotation = Quat(0.0f, Vec3::Top());
		editorCamera.transform.lookAt(origin.position);

		auto test = editor->addEntity();
		renderer = test->addComponent<MaterialRenderer>();
		renderer->transform.rotation= Quat(0, Vec3::Top());
		renderer->transform.position = Vec3({ 0, 2, 0 });
		renderer->mesh = cube;
		renderer->material = hagame::graphics::Material::emerald();
		renderer->shader = materialShader;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(window->window, window->gl);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	void onGameUpdate(double dt) {
		window->setTitle("HaGame Editor | " + version + " | " + activeGame + " | " + std::to_string(fps));

		if (input.keyboardMouse.home) {
			running = false;
		}

		materialShader->use();

		materialShader->setVec3("lightPos", editorLightPos);
		materialShader->setVec3("lightColor", editorLightColor.getVec3());
		materialShader->setVec3("viewPos", editorCamera.transform.position);
		
		float gridScale = 10.0f;
		int gridLines = 50;
		float half = gridScale / 2 * gridLines;

		for (int i = 0; i < gridLines; i++) {
			drawLine(Vec3({ i * gridScale - half, 0.0f,  -half }), Vec3({ i * gridScale - half , 0.0f, half }), hagame::graphics::Color::blue());
			drawLine(Vec3({ -half, 0.0f, i * gridScale - half }), Vec3({ half, 0.0f, i * gridScale - half }), hagame::graphics::Color::blue());
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->window);
		ImGui::NewFrame();
		bool showDemoWindow = true;
		if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);

		{
			ImGui::Begin("Window Properties");
			ImGui::Text("Adjust common settings of the window");
			ImGui::SliderFloat("FOV", &editorCamera.fov, 0.1f, PI / 2);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void drawAxis(hagame::Transform transform, float size = 1.0f) {
		Vec3 top = transform.rotation.rotatePoint(Vec3::Top());
		Vec3 right = transform.rotation.rotatePoint(Vec3::Right());
		Vec3 face = transform.rotation.rotatePoint(Vec3::Face());
		drawLine(transform.position, transform.position + (top * size), hagame::graphics::Color::red());
		drawLine(transform.position, transform.position + (right * size), hagame::graphics::Color::green());
		drawLine(transform.position, transform.position + (face * size), hagame::graphics::Color::blue());
	}

	void drawLine(Vec3 p1, Vec3 p2, hagame::graphics::Color color) {
		hagame::graphics::Line line = hagame::graphics::Line(p1, p2, color);
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), editorCamera.getViewMatrix(), editorCamera.getProjMatrix());
		colorShader->setVec3("color", color.getVec3());
		line.draw(colorShader);
	}
};