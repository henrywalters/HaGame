#define DEBUG_MODE true
#include "../Core/Game.h"
#include "../Graphics/Window.h"
#include "../Input/Device.h";
#include "../Input/Devices/KeyboardMouse.h";
#include "../Input/Devices/Gamepad.h";
#include "../Utils/FileSystem.h"
#include "../Utils/Loggers/FileLogger.h"
#include "../Core/ResourceManager.h"
#include "../Graphics/Camera.h"

namespace hagame {

	struct GameObject {
		graphics::Color color;
		graphics::Mesh mesh;
		Transform transform;
	};

	class ECSDemo : public hagame::Game {
	private:
		graphics::Window window;
		utils::FileSystem fs;
		input::devices::KeyboardMouse input;
		input::devices::Gamepad* gamepad;
		ResourceManager assets = ResourceManager("../../../HaGame3D_v1/Assets");
		ResourceManager shaders = ResourceManager("../../../HaGame3D_v1/Assets/Shaders");
		graphics::Camera* camera;
		graphics::ShaderProgram* colorShader;
		Array<GameObject*> gameObjects = Array<GameObject*>();
		float i = 0;

	public:

		ECSDemo() {}

		void onGameStart() {
			window = graphics::Window(Vec2({ 800, 800 }), "Test");
			window.create();

			gamepad = new input::devices::Gamepad(0);

			camera = new graphics::Camera(Vec3({ 800,800 }));
			camera->position = Vec3({ -20, 2, 0 });

			colorShader = shaders.loadShaderProgram("color");
			auto church = graphics::Mesh::FromOBJ(assets.loadFile("church", "Models/Church.obj"));
			auto cube = graphics::Mesh::FromOBJ(assets.loadFile("cube_model", "Models/Cube.obj"));
			auto cone = graphics::Mesh::FromOBJ(assets.loadFile("cone_model", "Models/Cone.obj"));
			auto taurus = graphics::Mesh::FromOBJ(assets.loadFile("taurus_model", "Models/Taurus.obj"));
			auto blender = graphics::Mesh::FromOBJ(assets.loadFile("blender_model", "Models/Blender.obj"));
			input = input::devices::KeyboardMouse();
			
			auto g1 = addGameObject(cube, graphics::Color::blue());
			auto g2 = addGameObject(cube, graphics::Color::red());
			auto g3 = addGameObject(church, graphics::Color::white());

			std::cout << cube.getBoundingCube().toString() << std::endl;

			g3->transform.move(Vec3({ 100, 0, 0 }));

			g2->transform.move(Vec3({ 0, 0, 0}));
		}

		GameObject* addGameObject(graphics::Mesh mesh, graphics::Color color) {
			GameObject* g1 = new GameObject();
			g1->color = color;
			g1->mesh = mesh;
			g1->transform = Transform();
			gameObjects.push_back(g1);
			return g1;
		}

		void onGameUpdate(double dt) {
			window.clear();

			input.pollDevice();
			gamepad->pollDevice();

			if (input.home) running = false;

			camera->position[0] += gamepad->lAxis[0] * dt;
			camera->position[2] += gamepad->lAxis[1] * dt;

			gameObjects[0]->transform.move(gamepad->lAxis.resize<3>() * dt);

			colorShader->use();
			
			gameObjects[0]->transform.rotate(Vec3({ 1.0, 0.0, 1.0 }), dt * 100);
			
			for (auto go : gameObjects) {
				colorShader->setMVP(go->transform.getModelMatrix(), camera->getProjectionMatrix(), camera->getViewMatrix());
				colorShader->setVec4("color", go->color.getVec4());
				go->mesh.draw(colorShader);
			}


			window.render();
		}
	};
}