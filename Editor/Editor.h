#include "../HaGame/HaGame.h"
#include "Systems/CameraController.h"
//#include "Systems/Renderer.h"
//#include "Systems/MousePicker.h"
#include "Systems/UI.h"

class HaGameEditor : public hagame::Game {
public:
	
	hagame::Transform origin;
	
	hagame::Scene* editor;

	Ptr<hagame::graphics::PerspectiveCamera> editorCamera = std::make_shared<hagame::graphics::PerspectiveCamera>();
	String activeGame = "untitled_game.hg";
	String version = "0.0.1";

	Vec2 viewportRatio = Vec2({ 1.0, 1.0 });

	hagame::math::Sample<double, 60> fpsSample;

	hagame::graphics::Plane<100, 100> plane = hagame::graphics::Plane<100, 100>(Vec2({ 300, 300 }));

	HaGameEditor(hagame::graphics::Window* window) : hagame::Game(window) {}

	void loadShaders() {
		resources->loadShaderProgram("color", "Shaders/color_vert.glsl", "Shaders/color_frag.glsl");
		resources->loadMesh("floor", plane.generateMesh());
	}

	void onGameStart() {

		resources->setBasePath("../../../Assets");

		loadShaders();

		fpsSample.onFull = [this]() {
			window->setTitle("HaGame Editor | " + version + " | " + activeGame + " | " + std::to_string(fpsSample.average()));
			fpsSample.clear();
		};

		Vec2 viewportSize = window->size.prod(viewportRatio);
		Vec2 viewportPos = (window->size - viewportSize) * 0.5;
		viewportPos[1] = 0.0f;

		window->setViewport(Rect(viewportPos, viewportSize));

		editor = addScene("editor");
		scenes.activate("editor");

		auto camEntity = editor->addEntity();
		auto cam = camEntity->addComponent<hagame::graphics::CameraComponent>();

		cam->active = true;
		cam->camera = editorCamera.get();

		camEntity->transform->setPosition(Vec3({ 0, 2, 0 }));

		//editor->addSystem<Renderer>();
		editor->addSystem<CameraController>();
		//editor->addSystem<MousePicker>();
		// editor->addSystem<UI>();
		// editor->addSystem<BadAppleSystem>();

		editor->addSystem<hagame::graphics::RenderSystem>();

		auto floorRenderer = editor->addEntity()
			->addComponent<hagame::graphics::MeshRenderer>();
		floorRenderer->mesh = plane.generateMesh().get();
		floorRenderer->shader = resources->getShaderProgram("color");
		floorRenderer->color = hagame::graphics::Color::blue();
		// floorRenderer->texture = resources->getTexture("wood_floor");

	}

	void onGameUpdate(double dt) {
		
		fpsSample.insert(fps);

		if (input.keyboardMouse.startPressed) {
			window->toggleRenderMode();
		}

	}
};