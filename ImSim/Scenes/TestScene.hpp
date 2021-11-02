#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "../../HaGame/HaGame.h"
#include "../Systems/CameraMovement.hpp"

using namespace hagame::graphics;

struct TestComponent {
	Vec3 rotation;
	float rotSpeed = 1.0f;
};

class TestSystem : public hagame::ecs::System {
public:

	String getSystemName() {
		return "TestSystem";
	}

	void onSystemUpdate(double dt) {
		forEach<TestComponent>([this, dt](TestComponent* test, Ptr<hagame::ecs::Entity> entity) {
			entity->transform->rotate(Quat(test->rotSpeed * dt, test->rotation));
		});
	}
};

class TestScene : public hagame::Scene {

	Ptr<PerspectiveCamera> camera;
	Ptr<Quad> quad;
	Ptr<Cylinder> cylinder;
	Ptr<RectPrism> rect;

	void loadResources() {



		// Meshes

		quad = std::make_shared<Quad>(6.0f, 4.0f);
		cylinder = std::make_shared<Cylinder>(1.0f, 2.0f, 10);
		rect = std::make_shared<RectPrism>(Vec3({ 2, 2, 4 }));

		game->resources->loadMesh("cube", "Models/Cube.obj");
		game->resources->loadMesh("triangle", Triangle(Vec3({ 0, 0, 0 }), Vec3({ 0, 1, 0 }), Vec3({1, 0, 0 })).getMesh());

		// Textures
		game->resources->loadTexture("crate", "Textures/crate.jpg");
		game->resources->loadTexture("proto", "Textures/prototype.jpg");
		game->resources->loadTexture("chain_base", "Textures/chainwall/basecolor.png");
		game->resources->loadTexture("chain_normal", "Textures/chainwall/normal.png");
		game->resources->loadTexture("chain_opacity", "Textures/chainwall/opacity.png");

		Array<String> shaders = {
			"color",
			"texture",
		};

		for (auto shader : shaders) {
			game->resources->loadShaderProgram(shader, "Shaders/" + shader + "_vert.glsl", "Shaders/" + shader + "_frag.glsl");
		}
	}

	void addTestCube(Vec3 pos) {
		auto testCube = addEntity();
		auto meshRenderer = testCube->addComponent<MeshRenderer>();

		meshRenderer->mesh = game->resources->getMesh("cube");
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::whiteRubber();

		testCube->transform->move(pos);
	}

	void addTestTriangle(Vec3 pos) {
		auto testTri = addEntity();
		auto meshRenderer = testTri->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("triangle");
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::whiteRubber();

		testTri->transform->move(pos);
	}

	void addTestQuad(Vec3 pos) {
		auto testQuad = addEntity();
		auto meshRenderer = testQuad->addComponent<MeshRenderer>();
		meshRenderer->mesh = quad->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::whiteRubber();

		testQuad->transform->move(pos);
	}

	void addTestCylinder(Vec3 pos) {
		auto testCyl = addEntity();
		testCyl->addComponent<TestComponent>()
			->rotation = Vec3({ 0, 1, 0});
		auto meshRenderer = testCyl->addComponent<MeshRenderer>();
		meshRenderer->mesh = cylinder->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::whiteRubber();

		testCyl->transform->move(pos);
	}

	void addTestRect(Vec3 pos) {
		auto test = addEntity();
		test->addComponent<TestComponent>()
			->rotation = Vec3({ 0, 0, 1 });
		auto meshRenderer = test->addComponent<MeshRenderer>();
		meshRenderer->mesh = rect->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::whiteRubber();

		test->transform->move(pos);
	}

	void onSceneBeforeActivate() {

		loadResources();

		addSystem<CameraMovement>();
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		addSystem<TestSystem>();

		camera = std::make_shared<PerspectiveCamera>();
		camera->setAspectRatio(game->window);

		addEntity()
			->addComponent<CameraComponent>()
			->camera = camera;

		// addTestCube(Vec3({ 2, -3, 5 }));

		
		addTestTriangle(Vec3({ 0, -2, 4 }));

		addTestQuad(Vec3({ 2, 3, 5 }));

		addTestCylinder(Vec3({ 1, 1, 3 }));

		addTestRect(Vec3({ 4, -2, 2 }));

		auto light = addEntity();
		light->transform->setPosition(Vec3({ 10.0, 10.0, 10.0 }));
		auto lightC = light->addComponent<hagame::graphics::PointLightComponent>();
		lightC->light.ambient = Vec3({ 0.4, 0.4, 0.4 });
		lightC->light.diffuse = Vec3({ 0.5, 0.5, 0.5 });
		lightC->light.specular = Vec3({ 0.7, 0.7, 0.7 });

	}

	void onSceneUpdate(double dt) {
		auto shader = game->resources->getShaderProgram("texture");
		shader->use();
		shader->setMVP(Mat4::Identity(), viewMat, projMat);
		//r->shader->setMat4("normal", model.inverted());
		shader->setVec4("color", Color::white());
		shader->setMaterial("material", Material::whiteRubber());
	}
};

#endif
