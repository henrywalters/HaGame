#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "../../HaGame/HaGame.h"
#include "../Systems/CameraMovement.hpp"

using namespace hagame::physics;
using namespace hagame::graphics;
using namespace hagame::utils;


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
	Ptr<OrthographicCamera> orth;
	Ptr<Quad> quad;
	Ptr<Cylinder> cylinder;
	Ptr<RectPrism> rect;
	Ptr<hagame::graphics::Sphere> sphere;
	Ptr<hagame::Transform> orthTrans;

	Ptr<Entity> light;
	Ptr<Entity> cam;

	int cylinderDivisions = 10;
	int lastCylinderDivisions = 10;

	float height = 2.0f;
	float radius = 1.0f;

	float sRadius = 2.0f;
	int sMeridians = 10;
	int sParallels = 10;

	std::optional<Ptr<Entity>> picking;
	Vec3 pickInitPos;
	std::optional<Vec3> initMousePos;

	std::optional<Ptr<Entity>> selected;

	void loadResources() {

		// Meshes

		quad = std::make_shared<Quad>(100.0f, 10.0f);
		cylinder = std::make_shared<Cylinder>(1.0f, 2.0f, 10);
		rect = std::make_shared<RectPrism>(Vec3({ 1, 2, 1 }));
		sphere = std::make_shared<hagame::graphics::Sphere>(sRadius, sMeridians, sParallels);

		game->resources->loadMesh("cube", "Models/Cube.obj");
		game->resources->loadMesh("triangle", Triangle(Vec3({ 0, 0, 0 }), Vec3({ 0, 1, 0 }), Vec3({1, 0, 0 })).getMesh());

		// Fonts

		game->resources->loadFont("arial", "Fonts/arial.ttf");

		// Textures
		game->resources->loadTexture("crate", "Textures/crate.jpg");
		game->resources->loadTexture("proto", "Textures/prototype.jpg");
		game->resources->loadTexture("chain_base", "Textures/chainwall/basecolor.png");
		game->resources->loadTexture("chain_normal", "Textures/chainwall/normal.png");
		game->resources->loadTexture("chain_opacity", "Textures/chainwall/opacity.png");

		Array<String> shaders = {
			"color",
			"texture",
			"text",
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

		auto collider = testCube->addComponent<Collider>();
		collider->boundingCube = Cube(Vec3::Zero(), meshRenderer->mesh->getBoundingCube().size);

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

	Ptr<Entity> addTestQuad(Vec3 pos) {
		auto testQuad = addEntity();
		auto meshRenderer = testQuad->addComponent<MeshRenderer>();
		meshRenderer->mesh = quad->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::whiteRubber();

		testQuad->transform->move(pos);

		return testQuad;
	}

	void addTestCylinder(Vec3 pos) {
		auto testCyl = addEntity();
		testCyl->addComponent<TestComponent>()
			->rotation = Vec3({ 0, 1, 0});
		auto meshRenderer = testCyl->addComponent<MeshRenderer>();
		meshRenderer->mesh = cylinder->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::red();
		meshRenderer->material = Material::redRubber();

		testCyl->transform->move(pos);

	}

	void addTestRect(Vec3 pos) {
		auto test = addEntity();
		auto meshRenderer = test->addComponent<MeshRenderer>();
		meshRenderer->mesh = rect->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::green();
		meshRenderer->material = Material::greenRubber();
		auto collider = test->addComponent<Collider>();
		collider->boundingCube = Cube(rect->getSize() * -0.5f, rect->getSize());

		test->transform->move(pos);
	}

	void addTestSphere(Vec3 pos) {
		auto test = addEntity();
		test->addComponent<TestComponent>();
		auto meshRenderer = test->addComponent<MeshRenderer>();
		meshRenderer->mesh = sphere->getMesh();
		meshRenderer->texture = game->resources->getTexture("proto");
		meshRenderer->shader = game->resources->getShaderProgram("texture");
		meshRenderer->color = Color::green();
		meshRenderer->material = Material::greenRubber();

		test->transform->move(pos);
	}

	void onSceneBeforeActivate() {

		loadResources();

		addSystem<CameraMovement>();
		addSystem<RenderSystem>();
		addSystem<LightingSystem>();
		addSystem<TestSystem>();
		addSystem<CollisionSystem>();

		orth = std::make_shared<OrthographicCamera>(Vec2(1.0f));
		orthTrans = std::make_shared<hagame::Transform>();

		camera = std::make_shared<PerspectiveCamera>();
		camera->setAspectRatio(game->window);

		cam = addEntity();
		cam->addComponent<CameraComponent>()
			->camera = camera;

		// addTestCube(Vec3({ 2, -3, 5 }));

		
		addTestTriangle(Vec3({ 0, -2, 4 }));

		addTestQuad(Vec3({ 0, 3, 5 }));

		addTestQuad(Vec3({ 0, -2, 0 }))
			->transform->rotate(Quat(PI / 2, Vec3::Right()));

		addTestCylinder(Vec3({ 1, 1, 3 }));

		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				addTestRect(Vec3({ -100 + i * 3.0f, 2.0f, -100 + j * 3.0f }));
			}
			
		}

		// addTestSphere(Vec3({ 1, -4, 4 }));

		light = addEntity();
		light->transform->setScale(Vec3(0.5f));
		light->transform->setPosition(Vec3({ 0, 5.0, 0 }));
		auto lightC = light->addComponent<hagame::graphics::PointLightComponent>();
		lightC->light.ambient = Vec3({ 0.4, 0.4, 0.4 });
		lightC->light.diffuse = Vec3({ 0.5, 0.5, 0.5 });
		lightC->light.specular = Vec3({ 0.7, 0.7, 0.7 });

		auto meshRenderer = light->addComponent<MeshRenderer>();
		meshRenderer->mesh = game->resources->getMesh("cube");
		meshRenderer->shader = game->resources->getShaderProgram("color");
		meshRenderer->color = Color::white();
		meshRenderer->material = Material::greenRubber();

	}

	void onSceneUpdate(double dt) {
		ImGui::Begin("Geometry settings :p");

		auto shader = game->resources->getShaderProgram("texture");
		shader->use();
		shader->setMVP(Mat4::Identity(), viewMat, projMat);
		//r->shader->setMat4("normal", model.inverted());
		shader->setVec4("color", Color::white());
		shader->setMaterial("material", Material::whiteRubber());

		auto mousePos = game->input.keyboardMouse.mouse.position;
		
		auto mouseWorldPos = MouseToWorld(viewMat, projMat, mousePos, game->window->size);

		shader = game->resources->getShaderProgram("color");
		shader->use();
		shader->setMVP(Mat4::Identity(), viewMat, projMat);
		auto mouseRay = hagame::math::Ray(cam->transform->getPosition(), (mouseWorldPos - cam->transform->getPosition()).normalized() * 1000);
		float t;
		auto hit = game->collisions.raycast(cam, mouseRay, t, {});

		if (hit.has_value()) {
			//drawCubeOutline(Cube(mouseRay.getPointOnLine(t), Vec3(0.1f)), Color::red(), shader);

			drawLine(hagame::graphics::Line(hit.value().point, hit.value().point + hit.value().normal * 0.5f, Color::blue()), shader);

			if (game->input.keyboardMouse.mouse.leftPressed) {
				selected = hit.value().entity;
				light->transform->setPosition(hit.value().entity->transform->getPosition() + Vec3({ 0, 1.5f, 0 }));
			}

			auto mesh = hit.value().entity->getComponent<MeshRenderer>();
			Rect bounds;
			float minX, minY, maxX, maxY;
			bool initialized = false;
			for (auto vertex : mesh->mesh->vertices) {

				auto screenPos = WorldToScreen(hit.value().entity->transform->getModelMatrix(), viewMat, projMat, vertex.position);
				if (!initialized) {
					minX = screenPos[0];
					minY = screenPos[1];
					maxX = screenPos[0];
					maxY = screenPos[1];
					initialized = true;
				}

				if (screenPos[0] < minX) { minX = screenPos[0]; }
				else if (screenPos[0] > maxX) { maxX = screenPos[0]; }

				if (screenPos[1] < minY) { minY = screenPos[1]; }
				else  if (screenPos[1] > maxY) { maxY = screenPos[1]; }
			}

			minX = mapToDomain<float>(-1, 1, 0, game->window->size[0], minX);
			minY = mapToDomain<float>(-1, 1, 0, game->window->size[1], minY);
			maxX = mapToDomain<float>(-1, 1, 0, game->window->size[0], maxX);
			maxY = mapToDomain<float>(-1, 1, 0, game->window->size[1], maxY);

			int padding = 10;

			bounds = Rect(Vec2({ minX - padding, minY - padding }), Vec2({ maxX - minX + padding * 2, maxY - minY + padding * 2 }));

			ImGui::Text(bounds.toString().c_str());

			shader = game->resources->getShaderProgram("color");
			shader->use();
			shader->setMVP(Mat4::Identity(), Mat4::Identity(), Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));
			drawRect(bounds, Color::blue(), shader);

			shader = game->resources->getShaderProgram("text");
			shader->use();
			shader->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));
			drawText(shader, game->resources->getFont("arial"), hit.value().entity->name, Color::red(), (bounds.pos + Vec2({5, bounds.size[1] - 20})).resize<3>());
		}

		// light->transform->setPosition(cam->transform->getPosition());

		

		ImGui::Text("Cylinder settings");

		if (ImGui::SliderInt("Divisions", &cylinderDivisions, 3, 300))
			cylinder->setDivisions(cylinderDivisions);

		if (ImGui::SliderFloat("height", &height, 0, 10))
			cylinder->setHeight(height);

		if (ImGui::SliderFloat("radius", &radius, 0, 10))
			cylinder->setRadius(radius);

		ImGui::End();

	}
};

#endif
