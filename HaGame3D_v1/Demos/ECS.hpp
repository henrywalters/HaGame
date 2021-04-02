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
#include "../Graphics/Material.h"
#include "../Graphics/Line.h"
#include "../Physics/RigidBody.h"

namespace hagame {

	struct GameObject {
		graphics::Color color;
		graphics::Mesh mesh;
		Transform transform;
		graphics::Material material;
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
		graphics::ShaderProgram* spriteShader;
		graphics::ShaderProgram* materialShader;
		Array<GameObject*> gameObjects = Array<GameObject*>();
		graphics::Sprite* sprite;
		float i = 0;
		Mat4 projMat;
		Mat4 view;
		float zFar = 200.0f;
		float zNear = 1.0f;
		float fov = 70 * DEG_TO_RAD;
		float pitch, yaw = 0;
		Quat target = Quat(0, Vec3::Top());
		Vec3 pos = Vec3({ 0, 3.0f, 0 });

		GameObject* plane;
		GameObject* player;
		GameObject* floor;
		physics::RigidBody* playerBody;
		physics::RigidBody* planeBody;

		float walkSpeed = 10.0f;
		float strafeSpeed = 20.0f;
		float deaccelForce = 500.0f;
		float xSens = 50.0f;
		float ySens = 100.0f;

	public:

		ECSDemo() {}

		void onGameStart() {
			window = graphics::Window(Vec2({ 1400, 800 }), "HaGame Editor");
			
			//window = graphics::Window::ForMonitor(graphics::MonitorManager::GetMonitor(0));
			window.create();

			gamepad = new input::devices::Gamepad(0);
			//input.captureMouseOn();
			//input.hideCursor();

			camera = new graphics::Camera();

			pos = Vec3({20, 15, 20});

			colorShader = shaders.loadShaderProgram("color");
			spriteShader = shaders.loadShaderProgram("sprite");
			materialShader = shaders.loadShaderProgram("material");

			assets.loadTexture("natalie", "Textures/natalie.jpg");

			auto church = graphics::Mesh::FromOBJ(assets.loadFile("church", "Models/Church.obj"));
			auto cube = graphics::Mesh::FromOBJ(assets.loadFile("cube_model", "Models/Cube.obj"));
			auto cone = graphics::Mesh::FromOBJ(assets.loadFile("cone_model", "Models/Cone.obj"));
			auto taurus = graphics::Mesh::FromOBJ(assets.loadFile("taurus_model", "Models/Taurus.obj"));
			auto blender = graphics::Mesh::FromOBJ(assets.loadFile("blender_model", "Models/Blender.obj"));
			auto shotgun = graphics::Mesh::FromOBJ(assets.loadFile("shotgun_mesh", "Models/Shotgun.obj"));
			auto jet = graphics::Mesh::FromOBJ(assets.loadFile("jet", "Models/boeing747.obj"));
			input = input::devices::KeyboardMouse();
			
			//auto g1 = addGameObject(cube, graphics::Color::blue(), graphics::Material::obsidian());
			//auto g2 = addGameObject(cube, graphics::Color::red(), graphics::Material::obsidian());
			//auto g3 = addGameObject(cube, graphics::Color::green(), graphics::Material::obsidian());
			//plane = addGameObject(jet, graphics::Color::white(), graphics::Material::chrome());
			plane = new GameObject();
			plane->material = graphics::Material::chrome();
			plane->transform = Transform();
			plane->mesh = jet;
			plane->transform.scale = Vec3({0.01, 0.01, 0.01});
			planeBody = new physics::RigidBody(&plane->transform);

			player = new GameObject();
			player->material = graphics::Material::emerald();
			player->transform = Transform();
			player->mesh = cube;
			player->transform.scale = Vec3({ 0.5f, 2.0f, 0.5f });
			playerBody = new physics::RigidBody(&player->transform);

			sprite = new graphics::Sprite(assets.getTexture("natalie"), Rect(Vec2({0, -2.0f}), Vec2({1, 1})));

			for (int i = 0; i < 2000; i++) {
				auto enemy = addGameObject(cube, graphics::Color::green(), graphics::Material::emerald());
				enemy->transform.move(Vec3({ cos((float)i / 20) * 10.0f, sin((float)i / 100) * 10.0f + 5.0f , (float)i / 20 + 5.0f }));
				enemy->transform.scale = Vec3({ 0.1, 0.1, 0.1 });
				enemy->transform.updateModel();
				gameObjects.push_back(enemy);
			}
			
			/*
			for (int i = 0; i < 10; i++) {
				auto gridLine = addGameObject(cube, graphics::Color::green());
				gridLine->transform.position = Vec3({ (float)i * 5, -2.0f, 0});
				gridLine->transform.scale = Vec3({ 0.01f, 0.01f, 100.0f });
			}

			for (int i = 0; i < 10; i++) {
				auto gridLine = addGameObject(cube, graphics::Color::green());
				gridLine->transform.position = Vec3({ 0, -2.0f, (float)i * 5});
				gridLine->transform.scale = Vec3({ 100.0f, 0.01f, 0.01f });
			}
			*/

			floor = addGameObject(cube, graphics::Color("#545454"), graphics::Material::obsidian());
			floor->transform.scale = Vec3({ 10000.0f, 0.1f, 10000.0f });
			floor->transform.position[1] = -5.0f;

			//g3->transform.scale[0] = 0.1f;
			//g3->transform.scale[1] = 20.0f;
			//g3->transform.scale[2] = 0.1f;

			//g1->transform.scale[0] = 20.0f;
			//g1->transform.scale[1] = 0.1f;
			//g1->transform.scale[2] = 0.1f;

			//g2->transform.scale[0] = 0.1f;
			//g2->transform.scale[1] = 0.1f;
			//g2->transform.scale[2] = 20.0f;

			projMat = Mat4::Perspective(PI / 1.5, 1, 0.1f, 100.0f);


			//std::cout << g1->transform.getModelMatrix().toString() << std::endl;
			std::cout << projMat.toString() << std::endl;

			std::cout << camera->getViewMatrix().toString() << std::endl;

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			ImGui_ImplSDL2_InitForOpenGL(window.window, window.gl);
			ImGui_ImplOpenGL3_Init("#version 330 core");
		}

		GameObject* addGameObject(graphics::Mesh mesh, graphics::Color color, graphics::Material mat) {
			GameObject* g1 = new GameObject();
			g1->color = color;
			g1->mesh = mesh;
			g1->transform = Transform();
			g1->material = mat;
			return g1;
		}

		void drawAxis(GameObject* go, float size = 1.0f) {
			Vec3 top = go->transform.rotation.rotatePoint(Vec3::Top());
			Vec3 right = go->transform.rotation.rotatePoint(Vec3::Right());
			Vec3 face = go->transform.rotation.rotatePoint(Vec3::Face());
			drawLine(go->transform.position, go->transform.position + (top * size), graphics::Color::red());
			drawLine(go->transform.position, go->transform.position + (right * size), graphics::Color::green());
			drawLine(go->transform.position, go->transform.position + (face * size), graphics::Color::blue());
		}

		void drawLine(Vec3 p1, Vec3 p2, graphics::Color color) {
			graphics::Line line = graphics::Line(p1, p2, color);
			colorShader->use();
			colorShader->setMVP(Mat4::Identity(), view, projMat);
			colorShader->setVec3("color", color.getVec3());
			line.draw(colorShader);
		}

		void onGameUpdate(double dt) {
			window.clear();

			input.pollDevice();
			gamepad->pollDevice();

			if (input.home) running = false;

			//gameObjects[3]->transform.rotate(Quat(dt, Vec3::Right()));

			sprite->rect.pos += (gamepad->lAxis * -dt);



			//std::cout << input.mouse.right << std::endl;

			if (input.mouse.right) {
				pitch = clamp<float>(pitch + input.rAxis[1] * dt * -ySens, -90 * DEG_TO_RAD, 90 * DEG_TO_RAD);
				yaw += -input.rAxis[0] * dt * xSens;
			}

			if (yaw < 0) {
				yaw = 360 * DEG_TO_RAD;
			}

			if (yaw > 360) {
				yaw = 0;
			}

			float yawCos = cos(yaw);
			float yawSin = sin(yaw);


			//playerBody->applyForce(Vec3({ yawSin, 0, yawCos }) * input.lAxis[1] * dt * walkForce);
			//playerBody->applyForce(cross(Vec3({ yawSin, 0, yawCos }), Vec3::Top()) * input.lAxis[0] * -dt * strafeForce);

			//playerBody->update(dt);

			player->transform.position += Vec3({ yawSin, 0, yawCos }) * input.lAxis[1] * dt * walkSpeed;
			player->transform.position += cross(Vec3({ yawSin, 0, yawCos }), Vec3::Top()) * input.lAxis[0] * -dt * strafeSpeed;

			// std::cout << fps << std::endl;

			pos = player->transform.position + Vec3({0, 0, 1});



			Vec3 targetPos = pos + (plane->transform.face());

			//std::cout << pos.toString() << std::endl;
			//std::cout << (pos + targetPos).toString() << std::endl;
			//projMat = Mat4::Orthographic(-10, 10, -10, 10, zNear, zFar);
			projMat = Mat4::Perspective(fov, window.size[0] / window.size[1], zNear, zFar);
			//Mat4 view = Mat4::LookAt(pos, gameObjects[0]->transform.position + (gameObjects[0]->mesh.getBoundingCube().size / 2), Vec3::Top());

			view = Mat4::FPSView(pos, pitch, yaw);

			//view = Mat4::LookAt(pos, targetPos, plane->transform.top());

			camera->rotation = Quat(input.rAxis[1] * dt * 10, Vec3({ 1, 0, 0 })) * camera->rotation;
			camera->rotation = Quat(-input.rAxis[0] * dt * 100, Vec3({ 0, 1, 0 })) * camera->rotation;
			
			materialShader->setVec3("lightPos", pos);
			materialShader->setVec3("viewPos", pos);
			materialShader->setVec3("lightColor", graphics::Color(1.0f, 1.0f, 1.0f, 1.0f).getVec3());

			for (auto go : gameObjects) {
				materialShader->use();
				go->transform.rotate(Quat(dt, Vec3({ 1.0, 0.0, 1.0 })));
				go->transform.updateModel();

				// std::cout << (camera->getPerspectiveMatrix() * camera->getViewMatrix() * go->transform.getModelMatrix()).toString() << std::endl;
				materialShader->setMVP(go->transform.model, view, projMat);

				materialShader->setMaterial("material", go->material);

				go->mesh.draw(materialShader);
				//drawAxis(go, 2);
			}

			materialShader->setMVP(floor->transform.getModelMatrix(), view, projMat);
			materialShader->setMaterial("material", floor->material);
			floor->mesh.draw(materialShader);

			/*
			plane->transform.rotate(Quat(gamepad->lAxis[1] * -dt, Vec3::Right()));
			plane->transform.rotate(Quat(gamepad->lAxis[0] * dt, Vec3::Face()));

			if (gamepad->rTrigger) {
				planeBody->applyForce(plane->transform.face() * dt * 1000);
			}

			plane->transform.position[1] = plane->transform.position[1] < 0 ? 0 : plane->transform.position[1];

			planeBody->applyGlobalGravity();
			planeBody->update(dt);
			*/
			materialShader->use();
			//std::cout << (camera->getPerspectiveMatrix() * camera->getViewMatrix() * go->transform.getModelMatrix()).toString() << std::endl;
			materialShader->setMVP(player->transform.getModelMatrix(), view, projMat);
			materialShader->setVec3("lightPos", pos);
			materialShader->setVec3("viewPos", pos);
			materialShader->setMaterial("material", player->material);
			materialShader->setVec3("lightColor", graphics::Color(1.0f, 1.0f, 1.0f, 1.0f).getVec3());
			//player->mesh.draw(materialShader);
			//drawAxis(player);

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(window.window);
			ImGui::NewFrame();

			bool showDemoWindow = true;
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			
			//if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
			
			{
				ImGui::Begin("Window Properties");
				ImGui::Text("Adjust common settings of the window");
				ImGui::SliderFloat("FOV", &fov, 0.1f, PI / 2);
				ImGui::SliderFloat("X Sens", &xSens, 0.001f, 20.0f);
				ImGui::SliderFloat("Y Sens", &ySens, 0.001f, 20.0f);
				ImGui::End();
			}
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			window.render();
		}
	};
}