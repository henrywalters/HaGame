#include "../HaGame/HaGame.h"
#include "Systems/CameraController.h"
#include "Systems/Renderer.h"
#include "Systems/MousePicker.h"
#include "Systems/UI.h"

Vec3 pixelSize = Vec3({ 5, 5, 5 });

struct FramePixel {
	hagame::graphics::Color color;
	Vec2Int idx;
	hagame::graphics::ShaderProgram* shader;
	hagame::graphics::Mesh* mesh;
	Mat4 model;
};

class BadAppleSystem : public hagame::ecs::System {
public:

	Vec2Int videoSize;
	
	float elapsedTime = 0;

	String getSystemName() {
		return "Bad Apple System";
	}

	void onSystemStart() {

		int sceneCount = 0;
		for (String file : game->resources.getFileSystem()->listFiles("../../BadAppleFrames")) {
			auto parts = stringSplit(file, '\\');
			auto name = parts[parts.size() - 1];
			name = stringSplit(name, '.')[0];
			auto image = game->resources.loadImage(std::to_string(sceneCount), file, hagame::graphics::ImageType::RGB);
			sceneCount++;

			std::cout << name << std::endl;

			if (sceneCount > 100) {
				break;
			}
		}

		videoSize = game->resources.getImage("0")->size;

		auto firstFrame = game->resources.getImage("0");

		for (int i = 0; i < firstFrame->size[0]; i++) {
			for (int j = 0; j < firstFrame->size[1]; j++) {
				auto pEnt = scene->addEntity();
				auto pixel = pEnt->addComponent<FramePixel>();
				pixel->idx = Vec2Int({ i, j });
				pixel->color = firstFrame->getColor(pixel->idx);
				pixel->shader = game->resources.getShaderProgram("color");
				pixel->mesh = game->resources.getMesh("cube_mesh");
				pixel->model = Mat4::Translation(Vec3({ 50 , firstFrame->size[1] * pixelSize[1] - pixel->idx[1] * pixelSize[1], pixel->idx[0] * pixelSize[0] }));
			}
		}
	}

	void onSystemUpdate(double dt) {

		if (game->input.keyboardMouse.isKeyDown(SDLK_SPACE)) {
			elapsedTime = 0;
		}
		else {
			elapsedTime += dt;
		}

		int frame = (int) (floor(elapsedTime * 30.0f)) % 6571;
		auto image = game->resources.getImage(std::to_string(frame));
		auto view = registry->view<FramePixel>();

		auto viewMat = scene->activeCamera->getViewMatrix();
		auto projMat = scene->activeCamera->getProjMatrix();

		for (auto pEnt : view) {
			auto pixel = view.get<FramePixel>(pEnt);
			pixel.color = image->getColor(pixel.idx);
			// pixel.color = pixel.color[0] + pixel.color[1] + pixel.color[2] > 2.0f ? hagame::graphics::Color::white() : hagame::graphics::Color::black();
			pixel.shader->use();
			pixel.shader->setMVP(pixel.model, viewMat, projMat);
			pixel.shader->setVec3("color", pixel.color.resize<3>());
			pixel.mesh->draw(pixel.shader);
		}
	}
};

class HaGameEditor : public hagame::Game {
public:
	
	hagame::Transform origin;
	
	hagame::Scene* editor;

	hagame::graphics::PerspectiveCamera editorCamera = hagame::graphics::PerspectiveCamera();

	hagame::graphics::ShaderProgram* materialShader;
	hagame::graphics::ShaderProgram* colorShader;

	String activeGame = "untitled_game.hg";
	String version = "0.0.1";

	hagame::Transform test;
	hagame::graphics::Mesh* cube;
	hagame::graphics::Mesh* sphere;

	MaterialRenderer* renderer;

	Vec2 viewportRatio = Vec2({ 1.0, 1.0 });

	hagame::math::Sample<double, 60> fpsSample;

	Vec3 lightPos = Vec3({ 0.0, 10.0f, 0.0f });
	hagame::graphics::Color lightColor = hagame::graphics::Color::white();

	hagame::utils::FileSystem fs = hagame::utils::FileSystem("D:/Development/BadAppleFrames");

	hagame::graphics::Plane<1000, 1000> plane = hagame::graphics::Plane<1000, 1000>(Vec2({ 500, 500 }));

	HaGameEditor(hagame::graphics::Window* window) : hagame::Game(window) {}

	void onGameStart() {

		fpsSample.onFull = [this]() {
			window->setTitle("HaGame Editor | " + version + " | " + activeGame + " | " + std::to_string(fpsSample.average()));
			fpsSample.clear();
		};

		Vec2 viewportSize = window->size.prod(viewportRatio);
		Vec2 viewportPos = (window->size - viewportSize) * 0.5;
		viewportPos[1] = 0.0f;

		window->setViewport(Rect(viewportPos, viewportSize));

		editor = addScene("editor");
		editor->activeCamera = &editorCamera;
		editor->addSystem<Renderer>();
		editor->addSystem<CameraController>();
		editor->addSystem<MousePicker>();
		// editor->addSystem<UI>();
		// editor->addSystem<BadAppleSystem>();

		resources.setBasePath("../../../Assets");


		cube = resources.loadMesh("cube_mesh", "Models/Cube.obj");
		sphere = resources.loadMesh("sphere_mesh", "Models/Sphere.obj");

		materialShader = resources.loadShaderProgram("material", "Shaders/material_vert.glsl", "Shaders/material_frag.glsl");
		materialShader->use();
		materialShader->setVec3("lightPos", lightPos);
		materialShader->setVec3("lightColor", lightColor.resize<3>());
		colorShader = resources.loadShaderProgram("color", "Shaders/color_vert.glsl", "Shaders/color_frag.glsl");

		editorCamera.aspectRatio = viewportSize[0] / viewportSize[1];
		editorCamera.transform.position = Vec3({ -5.0f, 50.0f, -5.0f });
		editorCamera.transform.rotation = Quat(0.0f, Vec3::Top());
		editorCamera.transform.lookAt(origin.position);

		/*auto test = editor->addEntity();
		renderer = test->addComponent<MaterialRenderer>();
		renderer->transform.rotation= Quat(0, Vec3::Top());
		renderer->transform.position = Vec3({ 0, 2, 0 });
		renderer->mesh = cube;
		renderer->material = hagame::graphics::Material::emerald();
		renderer->shader = colorShader;
		renderer->displayBounds = true;
		*/

		auto perlin = hagame::math::Perlin<16, 16, float>();

		for (int i = 0; i < plane.divisions[0]; i++) {
			for (int j = 0; j < plane.divisions[1]; j++) {
				plane.points[i][j][1] = perlin.compute(Vec2({ plane.points[i][j][0], plane.points[i][j][2] }));
				//std::cout << plane.points[i][j][1];
			}
		}

		auto floor = editor->addEntity();
		auto floorRenderer = floor->addComponent<MaterialRenderer>();
		floorRenderer->transform.rotation = Quat(0, Vec3::Top());
		floorRenderer->transform.position = Vec3({ 0, 2, 0 });
		floorRenderer->mesh = plane.generateMesh();
		floorRenderer->material = hagame::graphics::Material::obsidian();
		floorRenderer->shader = materialShader;
		
	}

	void onGameUpdate(double dt) {
		
		fpsSample.insert(fps);

		if (input.keyboardMouse.startPressed) {
			window->toggleRenderMode();
		}
		
		/*float gridScale = 10.0f;
		int gridLines = 50;
		float half = gridScale / 2 * gridLines;

		for (int i = 0; i < gridLines; i++) {
			drawLine(Vec3({ i * gridScale - half, 0.0f,  -half }), Vec3({ i * gridScale - half , 0.0f, half }), hagame::graphics::Color::blue());
			drawLine(Vec3({ -half, 0.0f, i * gridScale - half }), Vec3({ half, 0.0f, i * gridScale - half }), hagame::graphics::Color::blue());
		}
		*/
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
		colorShader->setVec3("color", color.resize<3>());
		line.draw(colorShader);
	}
};